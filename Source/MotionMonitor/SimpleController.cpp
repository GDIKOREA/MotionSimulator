
#include "stdafx.h"
#include "SimpleController.h"
#include "MainFrm.h"
#include "resource.h"
#include "MotionWaveView.h"
#include "MixingView.h"
#include "MotionOutputView.h"
#include "UDPInputView.h"
#include "JoystickView.h"



cSimpleController::cSimpleController() :
	m_oldState(cVitconMotionSim::OFF)
{

}

cSimpleController::~cSimpleController()
{

}


// 1. 게임 환경설정 파일을 읽는다.
// 2. 모든 뷰를 초기화 하고, Start 상태로 동작하게 한다.
void cSimpleController::StartMotionSim(const string &configFileName, const bool isStartMotionSimOut)
{
	if (CMainFrame *pFrm = dynamic_cast<CMainFrame*>(AfxGetMainWnd()))
	{
		// UDP View, Mixing View, Output View Start
		// 안전을 위해 순서를 지키자.
		pFrm->m_mixingView->Start();

		if (isStartMotionSimOut)
			pFrm->m_motionOutputView->Start();

		switch (g_gameType)
		{
		case GAME_TYPE::DIRT3: break;
		case GAME_TYPE::MACHINEGUN: break;
		case GAME_TYPE::JOYSTICK: pFrm->m_joystickView->Start(); break;
		case GAME_TYPE::MWAVE: pFrm->m_motionWaveView->Start(); break;
		default:
			break;
		}

		m_vitconMotionSim.On();
	}
}


void cSimpleController::StopMotionSim()
{
	if (CMainFrame *pFrm = dynamic_cast<CMainFrame*>(AfxGetMainWnd()))
	{
		// 안전을 위해 순서를 지키자.
		pFrm->m_udpInputView->Stop();
		pFrm->m_mixingView->Stop();
		pFrm->m_joystickView->Stop();
		pFrm->m_motionWaveView->Stop();

		m_vitconMotionSim.Off();
	}
}


// 매 프레임 마다 호출됨.
// 모션시뮬레이터 스테이트머신을 동작시킨다.
void cSimpleController::Update(const float deltaSeconds)
{
	m_vitconMotionSim.Update(deltaSeconds);

	const int motionSimState = m_vitconMotionSim.GetState();
	if (motionSimState != m_oldState)
	{
		switch (motionSimState)
		{
		case cVitconMotionSim::OFF:
			// ServoOff 상태로 바뀔 때, MotionOutputView도 같이 Stop 된다.
			if (CMainFrame *pFrm = dynamic_cast<CMainFrame*>(AfxGetMainWnd()))
			{
				pFrm->m_motionOutputView->Stop();
				pFrm->m_motionWaveView->Stop();
			}
			break;

		case cVitconMotionSim::READY:
			// 모션시뮬레이터가 준비상태가 되면, Start 상태로 만든다.
			m_vitconMotionSim.Play();
			break;
			
		default:
			break;
		}

		m_oldState = m_vitconMotionSim.GetState();
	}

}


void cSimpleController::UpdateUDP(const sMotionPacket &packet)
{
	// 아직 아무일 없음
}

