
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
		pFrm->m_mixingView->Start();
		pFrm->m_motionOutputView->Start();
		pFrm->m_udpInputView->Start();
		pFrm->m_udpParseView->Start();
	}
}


void cDirt3Controller::StopMotionSim()
{
	if (CMainFrame *pFrm = dynamic_cast<CMainFrame*>(AfxGetMainWnd()))
	{
		// ������ ���� ������ ��Ű��.
		pFrm->m_motionOutputView->Stop();
		pFrm->m_udpInputView->Stop();
		pFrm->m_udpParseView->Stop();
		pFrm->m_mixingView->Stop();
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

