
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


// 1. ���� ȯ�漳�� ������ �д´�.
// 2. ��� �並 �ʱ�ȭ �ϰ�, Start ���·� �����ϰ� �Ѵ�.
void cSimpleController::StartMotionSim(const string &configFileName, const bool isStartMotionSimOut)
{
	if (CMainFrame *pFrm = dynamic_cast<CMainFrame*>(AfxGetMainWnd()))
	{
		// UDP View, Mixing View, Output View Start
		// ������ ���� ������ ��Ű��.
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
		// ������ ���� ������ ��Ű��.
		pFrm->m_udpInputView->Stop();
		pFrm->m_mixingView->Stop();
		pFrm->m_joystickView->Stop();
		pFrm->m_motionWaveView->Stop();

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
				pFrm->m_motionOutputView->Stop();
				pFrm->m_motionWaveView->Stop();
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

