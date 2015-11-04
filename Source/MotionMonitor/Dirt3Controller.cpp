
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


cDirt3Controller::cDirt3Controller() :
	m_oldState(cVitconMotionSim2::OFF)
	, m_lastUDPUpdateTime(0)
	, m_state(OFF)
{
}

cDirt3Controller::~cDirt3Controller()
{
}


// 1. ���� ȯ�漳�� ������ �д´�.
// 2. ��� �並 �ʱ�ȭ �ϰ�, Start ���·� �����ϰ� �Ѵ�.
void cDirt3Controller::StartMotionSim(const string &configFileName, const bool isStartMotionSimOut)
{
	if (CMainFrame *pFrm = dynamic_cast<CMainFrame*>(AfxGetMainWnd()))
	{
		// UDP View, Mixing View, Output View Start
		// ������ ���� ������ ��Ű��.
		if (pFrm->m_mixingView)
			pFrm->m_mixingView->Start();
		if (pFrm->m_udpInputView)
			pFrm->m_udpInputView->Start();
		if (pFrm->m_udpParseView)
			pFrm->m_udpParseView->Start();

		if (isStartMotionSimOut)
			if (pFrm->m_motionOutputView)
				pFrm->m_motionOutputView->Start();

		m_state = READY;
		m_vitconMotionSim.On();

		// UDP ������ �������� �⺻���� �����Ǿ� �־��, �ӽ��� �⺻�ڼ��� ���� �� �ִ�.
		script::sFieldData data;
		data.fVal = 1.55f;
		data.type = script::FEILD_TYPE::T_FLOAT;
		script::g_symbols["$7"] = data;
	}
}


void cDirt3Controller::StopMotionSim()
{
	if (CMainFrame *pFrm = dynamic_cast<CMainFrame*>(AfxGetMainWnd()))
	{
		// ������ ���� ������ ��Ű��.
// 		if (pFrm->m_motionOutputView)
// 			pFrm->m_motionOutputView->Stop();
		if (pFrm->m_udpInputView)
			pFrm->m_udpInputView->Stop();
		if (pFrm->m_udpParseView)
			pFrm->m_udpParseView->Stop();
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
		if (elapseUDPTime > 5.f)
		{
			// UDP ��Ŷ�� ���̻� ���� �ʴ´ٸ�, ���� ������ ���� �� �� �ִ� ���·� �ٲ۴�.
			m_vitconMotionSim.Ready();
			m_state = READY;
		}
		break;

	case cDirt3Controller::PLAY:
		if (elapseUDPTime > 5.f)
		{
			// UDP ��Ŷ�� ���̻� ���� �ʴ´ٸ�, ������ Ready ���·� �ٲ۴�.
			m_vitconMotionSim.Ready();
			m_state = READY;
		}
		if (m_vitconMotionSim.GetPlayTime() > cController::Get()->GetPlayTime())
		{
			// �÷����� �� �ִ� ���� �ð��� �Ѿ��ٸ�, ������ �����Ѵ�.
			// UDP ������ ������ �� �� ��, 
			m_vitconMotionSim.Ready();
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

			// ServoOff ���·� �ٲ� ��, MotionOutputView�� ���� Stop �ȴ�.
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
	case cDirt3Controller::PLAY:
	case cDirt3Controller::TIMEUPSTOP:
		break;

	case cDirt3Controller::READY:
		// UDP ������ ������, ������ �����Ѵ�.
		if (m_vitconMotionSim.Play())
			m_state = PLAY;
		break;
	}
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
