
#include "stdafx.h"
#include "vitconmotionsim.h"
#include "MotionMonitor.h"
#include "MotionWaveView.h"
#include "MainFrm.h"



cVitconMotionSim::cVitconMotionSim() :
	m_state(OFF)
	, m_subState(SUBSTATE_OFF)
	, m_playTime(0)
	, m_lastUDPUpdateTime(0)
	, m_changeInformationPulse(true)
	, m_delayTime(0)
{

}

cVitconMotionSim::~cVitconMotionSim()
{

}


void cVitconMotionSim::Update(const float deltaSeconds)
{

	switch (m_subState)
	{
	case SUBSTATE_OFF: State_Off(deltaSeconds); break;
	case SUBSTATE_INIT: State_Init(deltaSeconds); break;
	case SUBSTATE_START: State_Start(deltaSeconds); break;
	case SUBSTATE_ORIGIN: State_Origin(deltaSeconds); break;
	case SUBSTATE_ORIGIN_STOP: State_OriginStop(deltaSeconds); break;
	case SUBSTATE_STOP_READY: State_StopReady(deltaSeconds); break;
	case SUBSTATE_READY: State_Ready(deltaSeconds); break;
	case SUBSTATE_LOAD: State_Load(deltaSeconds); break;
	case SUBSTATE_PLAYING: State_Playing(deltaSeconds); break;
	case SUBSTATE_END: State_End(deltaSeconds); break;
	case SUBSTATE_END_STOP: State_EndStop(deltaSeconds); break;
	case SUBSTATE_RESTART: State_Restart(deltaSeconds); break;
	case SUBSTATE_STOP: State_Stop(deltaSeconds); break;
	case SUBSTATE_DELAY: State_Delay(deltaSeconds); break;

	case SUBSTATE_START_MOTION: m_subState = SUBSTATE_INIT; break;
	case SUBSTATE_STOP_MOTION: m_subState = SUBSTATE_END; break;
	default:
		break;
	}

}


// 모션시뮬레이터를 켜고, ServoOn -> Origin -> Stop 상태로 만든다.
// INIT -> ORIGIN -> READY
void cVitconMotionSim::On()
{
	m_state = INIT;
	m_subState = SUBSTATE_START_MOTION;
}


// 플레이를 종료하고, Stop 상태에서 대기한다.
// ORIGIN -> STOP -> READY
void cVitconMotionSim::Ready()
{
	m_subState = SUBSTATE_ORIGIN_STOP;
}


// 모션시뮬레이터를 START 상태로 만든다.
void cVitconMotionSim::Play()
{
	m_subState = SUBSTATE_LOAD;
}


// 모션시뮬레이터를 종료하고 끈다. Origin -> Stop -> ServoOff
// END -> END_STOP
void cVitconMotionSim::Off()
{
	m_state = END;
	m_subState = SUBSTATE_STOP_MOTION;
}


void cVitconMotionSim::State_Off(const float deltaSeconds)
{
	m_state = OFF;
}


// Init State
// 1. 게임 환경설정 파일을 읽는다.
// 2. 모든 뷰를 초기화 하고, Start 상태로 동작하게 한다.
void cVitconMotionSim::State_Init(const float deltaSeconds)
{
	m_changeInformationPulse = true;
	SendMotionSimMessage(BITCON_SER::SERVOON);
	Delay(4, SUBSTATE_ORIGIN);
}


void cVitconMotionSim::State_Start(const float deltaSeconds)
{
	m_changeInformationPulse = true;
	SendMotionSimMessage(BITCON_SER::START);
	Delay(0.1f, SUBSTATE_PLAYING);
}


// Origin State
void cVitconMotionSim::State_Origin(const float deltaSeconds)
{
	m_changeInformationPulse = true;
	g_mwaveView->LoadandPlayMotionWave("../media/machine gun/motion wave/기본위치.mwav");
	Delay(3, SUBSTATE_READY);
}


void cVitconMotionSim::State_OriginStop(const float deltaSeconds)
{
	m_changeInformationPulse = true;
	g_mwaveView->LoadandPlayMotionWave("../media/machine gun/motion wave/기본위치.mwav");
	Delay(3, SUBSTATE_STOP_READY);
}


void cVitconMotionSim::State_StopReady(const float deltaSeconds)
{
	SendMotionSimMessage(BITCON_SER::STOP);
	Delay(0.1f, SUBSTATE_READY);
}


void cVitconMotionSim::State_Ready(const float deltaSeconds)
{
	// 아무일 없이 대기
	m_state = READY;
}


// Load State
void cVitconMotionSim::State_Load(const float deltaSeconds)
{
	m_changeInformationPulse = true;
	//g_mwaveView->LoadandPlayMotionWave("../media/machine gun/motion wave/scene1.mwav");
	Delay(0.1f, SUBSTATE_START);
}


// Playing State
void cVitconMotionSim::State_Playing(const float deltaSeconds)
{
// 	m_changeInformationPulse = true;
// 	const float elapsUDPTime = cController::Get()->GetGlobalSeconds() - m_lastUDPUpdateTime;
// 
// 	// UDP 패킷이 더이상 오지 않는다면, 게임이 종료된 것으로 판단한다.
// 	if (elapsUDPTime > 0.1f)
// 	{
// 		return;
// 	}

	m_state = PLAY;
	m_playTime += deltaSeconds;
}


// End State
void cVitconMotionSim::State_End(const float deltaSeconds)
{
	// origin motionwave
	g_mwaveView->LoadandPlayMotionWave("../media/machine gun/motion wave/기본위치.mwav");
	m_changeInformationPulse = true;
	Delay(5, SUBSTATE_END_STOP);
}


// EndStop State
void cVitconMotionSim::State_EndStop(const float deltaSeconds)
{
	m_changeInformationPulse = true;
	SendMotionSimMessage(BITCON_SER::STOP);
	Delay(1, SUBSTATE_STOP);
}


// Restart State
void cVitconMotionSim::State_Restart(const float deltaSeconds)
{
	m_changeInformationPulse = true;
}


// Stop State
void cVitconMotionSim::State_Stop(const float deltaSeconds)
{
	m_changeInformationPulse = true;
	SendMotionSimMessage(BITCON_SER::SERVOOFF);
	Delay(1, SUBSTATE_OFF);
}


// Delay State
void cVitconMotionSim::State_Delay(const float deltaSeconds)
{
	m_changeInformationPulse = true;
	m_delayTime -= deltaSeconds;

	if (m_delayTime < 0)
	{
		m_subState = m_delayNextState;
	}
}


// Delay State
void cVitconMotionSim::Delay(const float delaySeconds, const SUB_STATE next)
{
	if (next == SUBSTATE_DELAY)
		return;

	m_subState = SUBSTATE_DELAY;
	m_delayTime = delaySeconds;
	m_delayNextState = next;
}
