
#include "stdafx.h"
#include "MachineGunController.h"

#include "MotionMonitor.h"
#include "MotionWaveView.h"
#include "MainFrm.h"



cMachineGunController::cMachineGunController() :
	m_isPlayGame(false)
	, m_gameState(0)
	, m_gameMission(-1)
	, m_playTime(0)
	, m_lastUDPUpdateTime(0)
	, m_oldState(cVitconMotionSim::OFF)
{

}

cMachineGunController::~cMachineGunController()
{

}


// �� �����Ӹ��� ȣ��.
void cMachineGunController::Update(const float deltaSeconds)
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
			break;

		default:
			break;
		}

		m_oldState = m_vitconMotionSim.GetState();
	}

	char buff[256];
	if (m_hardwareInput.GetRecvData(buff, sizeof(buff)) > 0)
	{

		int a = 0;

	}

}


// ��Ŷ�� �ö����� ȣ��ȴ�.
// �ӽŰ� ���� ��� �ùķ����͸� ���� �۾��� �Ѵ�.
void cMachineGunController::UpdateUDP(const sMotionPacket &packet)
{
	// �������� ��
	if (m_isPlayGame)
	{
		m_lastUDPUpdateTime = cController::Get()->GetGlobalSeconds();

		// ���� ����.
		if (0 == packet.gamestate)
		{
			GameStop();
		}
	}

	if (m_vitconMotionSim.GetState() == cVitconMotionSim::READY)
	{
		// �������� �ƴ� ��, gamestate == 1�� �Ǹ�, ������ ���۵ȴ�.
		if (1 == packet.gamestate)
		{
			GameStart(packet.mission);
		}
	}

}


// INIT -> ORIGIN
void cMachineGunController::StartMotionSim(const string &configFileName, const bool isStartMotionSimOut)
{
	if (CMainFrame *pFrm = dynamic_cast<CMainFrame*>(AfxGetMainWnd()))
	{
		// UDP View, Mixing View, Output View Start
		// ������ ���� ������ ��Ű��.
		//pFrm->m_udpInputView->GetChildView()->Start();
		if (pFrm->m_udpParseView)
			pFrm->m_udpParseView->GetChildView()->Start();
		if (pFrm->m_mixingView)
			pFrm->m_mixingView->GetChildView()->Start();
		if (pFrm->m_motionWaveView)
			pFrm->m_motionWaveView->GetChildView()->Start();

		if (isStartMotionSimOut)
			if (pFrm->m_motionOutputView)
				pFrm->m_motionOutputView->GetChildView()->Start();

		m_configFileName = configFileName;
		m_vitconMotionSim.On();
		m_hardwareInput.Init(0, 20590);

	}
}


// END -> STOP
void cMachineGunController::StopMotionSim()
{
	if (CMainFrame *pFrm = dynamic_cast<CMainFrame*>(AfxGetMainWnd()))
	{
		// ������ ���� ������ ��Ű��.
		//pFrm->m_udpInputView->GetChildView()->Stop();
		if (pFrm->m_udpParseView)
			pFrm->m_udpParseView->GetChildView()->Stop();
		if (pFrm->m_mixingView)
			pFrm->m_mixingView->GetChildView()->Stop();
		if (pFrm->m_joystickView)
			pFrm->m_joystickView->GetChildView()->Stop();
		if (pFrm->m_motionWaveView)
			pFrm->m_motionWaveView->GetChildView()->Stop();

		m_vitconMotionSim.Off();
		m_hardwareInput.Close();
	}
}


// ��� �ùķ��̼��� �����Ѵ�.
// ��ǿ��̺긦 �ε��ϰ�, ��� �ùķ����͸� �۵���Ų��.
void cMachineGunController::GameStart(const int mission)
{
	m_isPlayGame = true;
	m_gameState = 1;
//	m_changeInformationPulse = true;
	m_gameMission = mission;
	m_playTime = 0;

	g_mwaveView->LoadandPlayMotionWave("../media/machinegun/motion wave/scene1.mwav");

	m_vitconMotionSim.Play();
}


// ��� �ùķ����͸� �����·� �����.
void cMachineGunController::GameStop()
{
	m_isPlayGame = false;
	m_gameState = 0;
	//	m_changeInformationPulse = true;

	m_vitconMotionSim.Ready();
}


// ���� ���°� �ٲ��, true�� �����ϰ�, false �� �����ȴ�.
bool cMachineGunController::CheckChangeState()
{
	if (m_changeInformationPulse)
	{
		m_changeInformationPulse = false;
		return true;
	}
	else
	{
		return false;
	}
}



// CString cMachineGunController::GetInnerStateName()
// {
// 	switch (m_state)
// 	{
// 	case MOTION_SIM_STATE::NONE: return L"NONE";
// 	case MOTION_SIM_STATE::START_MOTION: return L"";
// 	case MOTION_SIM_STATE::INIT: return L"INIT";
// 	case MOTION_SIM_STATE::START: return L"START";
// 	case MOTION_SIM_STATE::ORIGIN: return L"ORIGIN";
// 	case MOTION_SIM_STATE::ORIGIN_STOP: return L"ORIGIN_STOP";
// 	case MOTION_SIM_STATE::STOP_READY: return L"STOP_READY";
// 	case MOTION_SIM_STATE::READY: return L"READY";
// 	case MOTION_SIM_STATE::LOAD: return L"LOAD";
// 	case MOTION_SIM_STATE::PLAYING: return L"PLAYING";
// 	case MOTION_SIM_STATE::END: return L"END";
// 	case MOTION_SIM_STATE::END_STOP: return L"END_STOP";
// 	case MOTION_SIM_STATE::RESTART: return L"RESTART";
// 	case MOTION_SIM_STATE::STOP_MOTION: return L"STOP_MOTION";
// 	case MOTION_SIM_STATE::STOP: return L"STOP";
// 	case MOTION_SIM_STATE::DELAY: return L"DELAY";
// 	default: return L"DEFAULT";
// 	}
// }
