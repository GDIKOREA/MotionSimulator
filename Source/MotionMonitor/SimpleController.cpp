
#include "stdafx.h"
#include "SimpleController.h"
#include "MainFrm.h"


cSimpleController::cSimpleController()
{

}

cSimpleController::~cSimpleController()
{

}


// 1. ���� ȯ�漳�� ������ �д´�.
// 2. ��� �並 �ʱ�ȭ �ϰ�, Start ���·� �����ϰ� �Ѵ�.
void cSimpleController::StartMotionSim(const string &configFileName)
{
	if (CMainFrame *pFrm = dynamic_cast<CMainFrame*>(AfxGetMainWnd()))
	{
		// UDP View, Mixing View, Output View Start
		// ������ ���� ������ ��Ű��.
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
		// ������ ���� ������ ��Ű��.
		pFrm->m_motionOutputView->GetChildView()->Stop();
		pFrm->m_udpInputView->GetChildView()->Stop();
		pFrm->m_mixingView->GetChildView()->Stop();
		pFrm->m_joystickView->GetChildView()->Stop();
		pFrm->m_motionWaveView->GetChildView()->Stop();
	}
}


void cSimpleController::Update(const float deltaSeconds)
{
	// ���� �ƹ��� ����
}


void cSimpleController::UpdateUDP(const sMotionPacket &packet)
{
	// ���� �ƹ��� ����
}

