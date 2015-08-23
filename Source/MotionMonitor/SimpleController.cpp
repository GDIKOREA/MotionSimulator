
#include "stdafx.h"
#include "SimpleController.h"
#include "MainFrm.h"


cSimpleController::cSimpleController()
{

}

cSimpleController::~cSimpleController()
{

}


// 1. 게임 환경설정 파일을 읽는다.
// 2. 모든 뷰를 초기화 하고, Start 상태로 동작하게 한다.
void cSimpleController::StartMotionSim(const string &configFileName)
{
	if (CMainFrame *pFrm = dynamic_cast<CMainFrame*>(AfxGetMainWnd()))
	{
		// UDP View, Mixing View, Output View Start
		// 안전을 위해 순서를 지키자.
		pFrm->m_mixingView->GetChildView()->Start();
		pFrm->m_motionOutputView->GetChildView()->Start();

		switch (g_gameType)
		{
		case GAME_TYPE::DIRT3: break;
		case GAME_TYPE::MACHINEGUN: break;
		case GAME_TYPE::JOYSTICK: pFrm->m_joystickView->GetChildView()->Start(); break;
		case GAME_TYPE::MWAVE: pFrm->m_motionWaveView->GetChildView()->Start(); break;
		default:
			break;
		}
	}
}


void cSimpleController::StopMotionSim()
{
	if (CMainFrame *pFrm = dynamic_cast<CMainFrame*>(AfxGetMainWnd()))
	{
		// 안전을 위해 순서를 지키자.
		pFrm->m_motionOutputView->GetChildView()->Stop();
		pFrm->m_udpInputView->GetChildView()->Stop();
		pFrm->m_mixingView->GetChildView()->Stop();
		pFrm->m_joystickView->GetChildView()->Stop();
		pFrm->m_motionWaveView->GetChildView()->Stop();
	}
}


void cSimpleController::Update(const float deltaSeconds)
{
	// 아직 아무일 없음
}


void cSimpleController::UpdateUDP(const sMotionPacket &packet)
{
	// 아직 아무일 없음
}

