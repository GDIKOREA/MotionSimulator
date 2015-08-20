
#include "stdafx.h"
#include "MachineGunController.h"

#include "MotionMonitor.h"
#include "MotionWaveView.h"



cMachineGunController::cMachineGunController() :
	m_isPlayGame(false)
	, m_state(MOTION_SIM_STATE::NONE)
	, m_gameState(0)
	, m_gameMission(-1)
	, m_playTime(0)
	, m_lastUDPUpdateTime(0)
	, m_changeInformationPulse(true)
	, m_delayTime(0)
{

}

cMachineGunController::~cMachineGunController()
{

}


// 매 프레임마다 호출.
void cMachineGunController::Update(const float deltaSeconds)
{

	switch (m_state)
	{
	case MOTION_SIM_STATE::NONE: break;
	case MOTION_SIM_STATE::INIT: State_Init(deltaSeconds); break;
	case MOTION_SIM_STATE::START: State_Start(deltaSeconds); break;
	case MOTION_SIM_STATE::ORIGIN: State_Origin(deltaSeconds); break;
	case MOTION_SIM_STATE::ORIGIN_STOP: State_OriginStop(deltaSeconds); break;
	case MOTION_SIM_STATE::STOP_READY: State_StopReady(deltaSeconds); break;
	case MOTION_SIM_STATE::READY: State_Ready(deltaSeconds); break;		
	case MOTION_SIM_STATE::LOAD: State_Load(deltaSeconds); break;
	case MOTION_SIM_STATE::PLAYING: State_Playing(deltaSeconds); break;
	case MOTION_SIM_STATE::END: State_End(deltaSeconds); break;
	case MOTION_SIM_STATE::END_STOP: State_EndStop(deltaSeconds); break;		
	case MOTION_SIM_STATE::RESTART: State_Restart(deltaSeconds); break;
	case MOTION_SIM_STATE::STOP: State_Stop(deltaSeconds); break;
	case MOTION_SIM_STATE::DELAY: State_Delay(deltaSeconds); break;

	case MOTION_SIM_STATE::START_MOTION: m_state = MOTION_SIM_STATE::INIT; break;
	case MOTION_SIM_STATE::STOP_MOTION: m_state = MOTION_SIM_STATE::END; break;
	default:
		break;
	}

}


// 패킷이 올때마다 호출된다.
// 머신건 게임 모션 시뮬레이터를 위한 작업을 한다.
void cMachineGunController::UpdateUDP(const sMotionPacket &packet)
{
	// 게임중일 때
	if (m_isPlayGame)
	{
		m_lastUDPUpdateTime = cController::Get()->GetGlobalSeconds();

		// 게임 종료.
		if (0 == packet.gamestate)
		{
			GameStop();
		}
	}

	if (MOTION_SIM_STATE::READY == m_state)
	{
		// 게임중이 아닐 때, gamestate == 1이 되면, 게임이 시작된다.
		if (1 == packet.gamestate)
		{
			GameStart(packet.mission);
		}
	}

}


// 모션 시뮬레이터를 대기상태로 만든다.
void cMachineGunController::GameStop()
{
	m_isPlayGame = false;
	m_gameState = 0;
	m_changeInformationPulse = true;

	m_state = MOTION_SIM_STATE::ORIGIN_STOP;
}


// 모션 시뮬레이션을 시작한다.
// 모션웨이브를 로딩하고, 모션 시뮬레이터를 작동시킨다.
void cMachineGunController::GameStart(const int mission)
{
	m_isPlayGame = true;
	m_gameState = 1;
	m_changeInformationPulse = true;
	m_gameMission = mission;
	m_playTime = 0;

	m_state = MOTION_SIM_STATE::LOAD;
}


// 게임 상태가 바뀌면, true를 리턴하고, false 로 설정된다.
bool cMachineGunController::CheckChangeState()
{
	if (m_changeInformationPulse)
	{
		m_changeInformationPulse = false;
		return true;
	}
	else
	{
		return false;
	}
}


// INIT -> ORIGIN
void cMachineGunController::StartMotionSim()
{
	m_state = MOTION_SIM_STATE::START_MOTION;
}


// END -> STOP
void cMachineGunController::StopMotionSim()
{
	m_state = MOTION_SIM_STATE::STOP_MOTION;
}


// Init State
void cMachineGunController::State_Init(const float deltaSeconds)
{
	m_changeInformationPulse = true;
	SendMotionSimMessage(BITCON_SER::SERVOON);
	Delay(4, MOTION_SIM_STATE::ORIGIN);
}


void cMachineGunController::State_Start(const float deltaSeconds)
{
	m_changeInformationPulse = true;
	SendMotionSimMessage(BITCON_SER::START);
	Delay(0.1f, MOTION_SIM_STATE::PLAYING);
}


// Origin State
void cMachineGunController::State_Origin(const float deltaSeconds)
{
	m_changeInformationPulse = true;
	g_mwaveView->LoadandPlayMotionWave("기본위치.mwav");
	Delay(3, MOTION_SIM_STATE::READY);
}


void cMachineGunController::State_OriginStop(const float deltaSeconds)
{
	m_changeInformationPulse = true;
	g_mwaveView->LoadandPlayMotionWave("기본위치.mwav");
	Delay(3, MOTION_SIM_STATE::STOP_READY);
}

void cMachineGunController::State_StopReady(const float deltaSeconds)
{
	SendMotionSimMessage(BITCON_SER::STOP);
	Delay(0.1f, MOTION_SIM_STATE::READY);
}


void cMachineGunController::State_Ready(const float deltaSeconds)
{
	// 아무일 없음.
}


// Load State
void cMachineGunController::State_Load(const float deltaSeconds)
{
	m_changeInformationPulse = true;
	g_mwaveView->LoadandPlayMotionWave("../media/machine gun/motion wave/scene1.mwav");
	Delay(0.1f, MOTION_SIM_STATE::START);
}


// Playing State
void cMachineGunController::State_Playing(const float deltaSeconds)
{
	m_changeInformationPulse = true;
	const float elapsUDPTime = cController::Get()->GetGlobalSeconds() - m_lastUDPUpdateTime;

	// UDP 패킷이 더이상 오지 않는다면, 게임이 종료된 것으로 판단한다.
	if (elapsUDPTime > 0.1f)
	{
		return;
	}

	m_playTime += deltaSeconds;

}


// End State
void cMachineGunController::State_End(const float deltaSeconds)
{
	// origin motionwave
	g_mwaveView->LoadandPlayMotionWave("기본위치.mwav");
	m_changeInformationPulse = true;
	Delay(5, MOTION_SIM_STATE::END_STOP);
}


// EndStop State
void cMachineGunController::State_EndStop(const float deltaSeconds)
{
	m_changeInformationPulse = true;
	SendMotionSimMessage(BITCON_SER::STOP);
	Delay(1, MOTION_SIM_STATE::STOP);
}


// Restart State
void cMachineGunController::State_Restart(const float deltaSeconds)
{
	m_changeInformationPulse = true;
}


// Stop State
void cMachineGunController::State_Stop(const float deltaSeconds)
{
	m_changeInformationPulse = true;
	SendMotionSimMessage(BITCON_SER::SERVOOFF);
	Delay(1, MOTION_SIM_STATE::NONE);
}


// Delay State
void cMachineGunController::State_Delay(const float deltaSeconds)
{
	m_changeInformationPulse = true;
	m_delayTime -= deltaSeconds;

	if (m_delayTime < 0)
	{
		m_state = m_delayNextState;
	}
}


// Delay State
void cMachineGunController::Delay(const float delaySeconds, const MOTION_SIM_STATE::TYPE next)
{
	if (next == MOTION_SIM_STATE::DELAY)
		return;

	m_state = MOTION_SIM_STATE::DELAY;
	m_delayTime = delaySeconds;
	m_delayNextState = next;
}


// 모션 컨트롤러 상태 메세지를 보낸다.
void cMachineGunController::SendMotionSimMessage(const int state)
{
	const int out_pitch = 256;
	const int out_roll = 256;
	const int out_yaw = 256;
	const int out_sway = 256;
	const int out_surge = 256;
	const int out_heave = 512;
	const int out_switch = state;

	const string out = common::format(
		"A%3d%3d%3d%3d%3d%3d%dZ", out_roll, out_pitch, out_yaw,
		out_sway, out_surge, out_heave, out_switch);

 	for (int i = 0; i < 5; ++i)
	{
		cController::Get()->GetSerialComm().GetSerial().SendData(out.c_str(), out.size());
		Sleep(50);
	}
}


CString cMachineGunController::GetInnerStateName()
{
	switch (m_state)
	{
	case MOTION_SIM_STATE::NONE: return L"NONE";
	case MOTION_SIM_STATE::START_MOTION: return L"";
	case MOTION_SIM_STATE::INIT: return L"INIT";
	case MOTION_SIM_STATE::START: return L"START";
	case MOTION_SIM_STATE::ORIGIN: return L"ORIGIN";
	case MOTION_SIM_STATE::ORIGIN_STOP: return L"ORIGIN_STOP";
	case MOTION_SIM_STATE::STOP_READY: return L"STOP_READY";
	case MOTION_SIM_STATE::READY: return L"READY";
	case MOTION_SIM_STATE::LOAD: return L"LOAD";
	case MOTION_SIM_STATE::PLAYING: return L"PLAYING";
	case MOTION_SIM_STATE::END: return L"END";
	case MOTION_SIM_STATE::END_STOP: return L"END_STOP";
	case MOTION_SIM_STATE::RESTART: return L"RESTART";
	case MOTION_SIM_STATE::STOP_MOTION: return L"STOP_MOTION";
	case MOTION_SIM_STATE::STOP: return L"STOP";
	case MOTION_SIM_STATE::DELAY: return L"DELAY";
	default: return L"DEFAULT";
	}
}
