
#include "stdafx.h"
#include "SimpleController.h"
#include "MainFrm.h"


cSimpleController::cSimpleController() :
	m_oldState(cVitconMotionSim::OFF)
{

}

cSimpleController::~cSimpleController()
{

}


// 1. ���� ȯ�漳�� ������ �д´�.
// 2. ��� �並 �ʱ�ȭ �ϰ�, Start ���·� �����ϰ� �Ѵ�.
void cSimpleController::StartMotionSim(const string &configFileName, const bool isStartMotionSimOut)
{
	if (CMainFrame *pFrm = dynamic_cast<CMainFrame*>(AfxGetMainWnd()))
	{
		// UDP View, Mixing View, Output View Start
		// ������ ���� ������ ��Ű��.
		pFrm->m_mixingView->GetChildView()->Start();

		if (isStartMotionSimOut)
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

		m_vitconMotionSim.On();
	}
}


void cSimpleController::StopMotionSim()
{
	if (CMainFrame *pFrm = dynamic_cast<CMainFrame*>(AfxGetMainWnd()))
	{
		// ������ ���� ������ ��Ű��.
		pFrm->m_udpInputView->GetChildView()->Stop();
		pFrm->m_mixingView->GetChildView()->Stop();
		pFrm->m_joystickView->GetChildView()->Stop();
		pFrm->m_motionWaveView->GetChildView()->Stop();

		m_vitconMotionSim.Off();
	}
}


// �� ������ ���� ȣ���.
// ��ǽùķ����� ������Ʈ�ӽ��� ���۽�Ų��.
void cSimpleController::Update(const float deltaSeconds)
{
	m_vitconMotionSim.Update(deltaSeconds);

	const int motionSimState = m_vitconMotionSim.GetState();
	if (motionSimState != m_oldState)
	{
		switch (motionSimState)
		{
		case cVitconMotionSim::OFF:
			// ServoOff ���·� �ٲ� ��, MotionOutputView�� ���� Stop �ȴ�.
			if (CMainFrame *pFrm = dynamic_cast<CMainFrame*>(AfxGetMainWnd()))
			{
				pFrm->m_motionOutputView->GetChildView()->Stop();
				pFrm->m_motionWaveView->GetChildView()->Stop();
			}
			break;

		case cVitconMotionSim::READY:
			// ��ǽùķ����Ͱ� �غ���°� �Ǹ�, Start ���·� �����.
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
	// ���� �ƹ��� ����
}

