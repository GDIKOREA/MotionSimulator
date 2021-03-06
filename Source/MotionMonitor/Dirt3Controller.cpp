
#include "stdafx.h"
#include "Dirt3Controller.h"
#include "MainFrm.h"
#include "resource.h"
#include "MotionWaveView.h"
#include "MixingView.h"
#include "MotionOutputView.h"
#include "UDPInputView.h"
#include "UDPParseView.h"
#include "JoystickView.h"
#include "VarModulationView.h"


cDirt3Controller::cDirt3Controller() :
	m_oldState(cVitconMotionSim2::OFF)
	, m_lastUDPUpdateTime(0)
	, m_state(OFF)
	, m_isLapTimeProgress(false)
	, m_lastLapTime(0)
	, m_timeUpCount(0)
{
}

cDirt3Controller::~cDirt3Controller()
{
}


// 1. 게임 환경설정 파일을 읽는다.
// 2. 모든 뷰를 초기화 하고, Start 상태로 동작하게 한다.
void cDirt3Controller::StartMotionSim(const string &configFileName, const bool isStartMotionSimOut)
{
	if (CMainFrame *pFrm = dynamic_cast<CMainFrame*>(AfxGetMainWnd()))
	{
		// UDP View, Mixing View, Output View Start
		// 안전을 위해 순서를 지키자.
		if (pFrm->m_mixingView)
			pFrm->m_mixingView->Start();
		if (pFrm->m_varModulationView)
			pFrm->m_varModulationView->Start();
		if (pFrm->m_udpInputView)
			pFrm->m_udpInputView->Start();
		if (pFrm->m_udpParseView)
			pFrm->m_udpParseView->Start();

		if (isStartMotionSimOut)
			if (pFrm->m_motionOutputView)
				pFrm->m_motionOutputView->Start();

		m_state = READY;
		m_vitconMotionSim.On();

		// UDP 정보가 오기전에 기본값이 설정되어 있어야, 머신이 기본자세를 취할 수 있다.
		script::sFieldData data;
		data.fVal = 1.55f;
		data.type = script::FEILD_TYPE::T_FLOAT;
		script::g_symbols["$7"] = data;
// 		data.fVal = 0.f;
// 		data.type = script::FEILD_TYPE::T_FLOAT;
// 		script::g_symbols["$13"] = data;
	}
}


void cDirt3Controller::StopMotionSim()
{
	if (CMainFrame *pFrm = dynamic_cast<CMainFrame*>(AfxGetMainWnd()))
	{
		// 안전을 위해 순서를 지키자.
// 		if (pFrm->m_motionOutputView)
// 			pFrm->m_motionOutputView->Stop();
		if (pFrm->m_udpInputView)
			pFrm->m_udpInputView->Stop();
		if (pFrm->m_udpParseView)
			pFrm->m_udpParseView->Stop();
		if (pFrm->m_varModulationView)
			pFrm->m_varModulationView->Stop();
		if (pFrm->m_mixingView)
			pFrm->m_mixingView->Stop();

		m_state = OFF_PROCESS;
		m_vitconMotionSim.Off();
	}
}


void cDirt3Controller::Update(const float deltaSeconds)
{
	const float elapseUDPTime = cController::Get()->GetGlobalSeconds() - m_lastUDPUpdateTime;

	switch (m_state)
	{
	case cDirt3Controller::OFF:
	case cDirt3Controller::OFF_PROCESS:
	case cDirt3Controller::READY:
		break;

	case cDirt3Controller::TIMEUPSTOP:
		//if (elapseUDPTime > 5.f)
		if (script::g_symbols["@laptime"].fVal == 0.f)
		{
			// 게임이 끝나거나면,  대기 상태로 바꾼다.
			m_vitconMotionSim.ReadyNoOrigin();
			m_state = READY;
		}
		break;

	case cDirt3Controller::PLAY:
		if (elapseUDPTime > 0.5f)
		{
			// 게임이 시작된 후, $laptime 값이 증가하다가, 
			// UDP 패킷이 더이상 오지 않는다면, 게임을 Ready 상태로 바꾼다.
			if (m_isLapTimeProgress && (script::g_symbols["@laptime"].fVal != 0.f))
			{
				m_vitconMotionSim.ReadyNoOrigin();
				m_state = READY;
			}
		}

		if (m_vitconMotionSim.GetPlayTime() > cMotionController::Get()->m_config.m_dirt3ViewPlayTime)
		{
			// 플레이할 수 있는 게임 시간을 넘었다면, 게임을 종료한다.
			// UDP 전송이 완전히 끝 난 후, 
			m_vitconMotionSim.ReadyNoOrigin();
			m_state = TIMEUPSTOP;
		}
		break;
	}

	m_vitconMotionSim.Update(deltaSeconds);

	const cVitconMotionSim2::STATE motionSimState = m_vitconMotionSim.GetState();
	if (motionSimState != m_oldState)
	{
		switch (motionSimState)
		{
		case cVitconMotionSim2::OFF:
			m_state = OFF;

			// ServoOff 상태로 바뀔 때, MotionOutputView도 같이 Stop 된다.
			if (CMainFrame *pFrm = dynamic_cast<CMainFrame*>(AfxGetMainWnd()))
			{
				pFrm->m_motionOutputView->Stop();
				pFrm->m_motionWaveView->Stop();
			}
			break;

		case cVitconMotionSim2::READY:
			//m_vitconMotionSim.Play();
			break;

		default:
			break;
		}

		m_oldState = m_vitconMotionSim.GetState();
	}
}


void cDirt3Controller::UpdateUDP(const char *buffer, const int bufferLen)
{
	m_lastUDPUpdateTime = cController::Get()->GetGlobalSeconds();

	switch (m_state)
	{
	case cDirt3Controller::OFF:
	case cDirt3Controller::TIMEUPSTOP:
		break;

	case cDirt3Controller::READY:
	{
		// UDP 정보가 들어오고,
		// labtime이 일정시간 이상 증가될 때,  
		const float curLabTime = script::g_symbols["@laptime"].fVal;
		if (curLabTime > m_lastLapTime)
		{
			++m_timeUpCount;
			
			// 5 frame 동안 시간이 증가 되었을 때, 게임을 시작한다.
			if (m_timeUpCount > 5)
			{
				if (m_vitconMotionSim.Play())
				{
					m_state = PLAY;
					m_lastLapTime = 0;
					m_timeUpCount = 0;
					m_isLapTimeProgress = false;
				}
			}

		}
	}
	break;

	case cDirt3Controller::PLAY:
	{
		const float curLapTime = script::g_symbols["@laptime"].fVal;
		const float distance = script::g_symbols["@distance"].fVal;
		if ((m_lastLapTime == curLapTime) && (m_lastLapTime == 0.f) && (distance != 0.f))
		{
			// 레이스가 끝났다면, 게임을 Ready 상태로 바꾼다.
			m_vitconMotionSim.ReadyNoOrigin();
			m_state = READY;
		}

		if (!m_isLapTimeProgress)
		{
			if (curLapTime > m_lastLapTime)
			{
				m_isLapTimeProgress = true;
			}
			else
			{
				m_lastLapTime = curLapTime;
			}
		}

	}
	break;
	}

	m_lastLapTime = script::g_symbols["@laptime"].fVal;
}


string cDirt3Controller::GetStateStr() const
{
	switch (m_state)
	{
	case cDirt3Controller::OFF: return "Off";
	case cDirt3Controller::OFF_PROCESS: return "Off Process";
	case cDirt3Controller::READY: return "Ready";
	case cDirt3Controller::PLAY: return "Play";
	case cDirt3Controller::TIMEUPSTOP: return "TimeUpStop";
	default: return "None";
	}	
}
