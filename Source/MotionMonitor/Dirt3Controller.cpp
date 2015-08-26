
#include "stdafx.h"
#include "Dirt3Controller.h"
#include "MainFrm.h"


cDirt3Controller::cDirt3Controller()
{

}

cDirt3Controller::~cDirt3Controller()
{

}


// 1. ���� ȯ�漳�� ������ �д´�.
// 2. ��� �並 �ʱ�ȭ �ϰ�, Start ���·� �����ϰ� �Ѵ�.
void cDirt3Controller::StartMotionSim(const string &configFileName)
{
	if (CMainFrame *pFrm = dynamic_cast<CMainFrame*>(AfxGetMainWnd()))
	{
		// UDP View, Mixing View, Output View Start
		// ������ ���� ������ ��Ű��.
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
		// ������ ���� ������ ��Ű��.
		pFrm->m_motionOutputView->GetChildView()->Stop();
		pFrm->m_udpInputView->GetChildView()->Stop();
		pFrm->m_udpParseView->GetChildView()->Stop();
		pFrm->m_mixingView->GetChildView()->Stop();
	}
}


void cDirt3Controller::Update(const float deltaSeconds)
{
	// ���� �ƹ��� ����
}


void cDirt3Controller::UpdateUDP(const sMotionPacket &packet)
{
	// ���� �ƹ��� ����
}

