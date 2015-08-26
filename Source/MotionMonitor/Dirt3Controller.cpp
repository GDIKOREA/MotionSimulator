
#include "stdafx.h"
#include "Dirt3Controller.h"
#include "MainFrm.h"


cDirt3Controller::cDirt3Controller()
{

}

cDirt3Controller::~cDirt3Controller()
{

}


// 1. 게임 환경설정 파일을 읽는다.
// 2. 모든 뷰를 초기화 하고, Start 상태로 동작하게 한다.
void cDirt3Controller::StartMotionSim(const string &configFileName)
{
	if (CMainFrame *pFrm = dynamic_cast<CMainFrame*>(AfxGetMainWnd()))
	{
		// UDP View, Mixing View, Output View Start
		// 안전을 위해 순서를 지키자.
		pFrm->m_mixingView->GetChildView()->Start();
		pFrm->m_motionOutputView->GetChildView()->Start();
		pFrm->m_udpInputView->GetChildView()->Start();
		pFrm->m_udpParseView->GetChildView()->Start();
	}
}


void cDirt3Controller::StopMotionSim()
{
	if (CMainFrame *pFrm = dynamic_cast<CMainFrame*>(AfxGetMainWnd()))
	{
		// 안전을 위해 순서를 지키자.
		pFrm->m_motionOutputView->GetChildView()->Stop();
		pFrm->m_udpInputView->GetChildView()->Stop();
		pFrm->m_udpParseView->GetChildView()->Stop();
		pFrm->m_mixingView->GetChildView()->Stop();
	}
}


void cDirt3Controller::Update(const float deltaSeconds)
{
	// 아직 아무일 없음
}


void cDirt3Controller::UpdateUDP(const sMotionPacket &packet)
{
	// 아직 아무일 없음
}

