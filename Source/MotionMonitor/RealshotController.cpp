
#include "stdafx.h"
#include "RealshotController.h"

#include "MotionMonitor.h"
#include "MotionWaveView.h"
#include "LauncherView.h"
#include "MainFrm.h"
#include "resource.h"
#include "MotionOutputView.h"
#include "UDPParseView.h"
#include "MixingView.h"
#include "MotionWaveView.h"
#include "JoystickView.h"
#include "RealShotView.h"



cRealShotController::cRealShotController() :
m_isPlayGame(false)
, m_gameState(0)
, m_gameMission(-1)
, m_playTime(0)
, m_lastUDPUpdateTime(0)
, m_activeGunFire1(false)
{
}

cRealShotController::~cRealShotController()
{
	// 종료될 때, 기록들을 저장한다.
	using namespace std;

}


// UDP 통신은 프로그램이 시작된 이후 부터 계속 처리한다.
bool cRealShotController::Init()
{
	const string fileName = "../media/realshot/realshot_controller.ini";
	std::ifstream ifs(fileName);
	if (!ifs.is_open())
		AfxMessageBox(formatw("Not Found %s file", fileName.c_str()).c_str());

	const int receivePort = uiutil::GetProfileInt("MainBoard UDP", "ReceivePort", 10000, fileName);
	const string mainBoardIp = uiutil::GetProfileString("MainBoard UDP", "MainBoardIP", "192.168.0.90", fileName);
	const int mainBoardPort = uiutil::GetProfileInt("MainBoard UDP", "MainBoardPort", 10000, fileName);

	const string proxyIp = uiutil::GetProfileString("MainBoard UDP", "ProxyIP", "127.0.0.1", fileName);
	const int proxyPort = uiutil::GetProfileInt("MainBoard UDP", "ProxyPort", 10000, fileName);

	const int cameraPort = uiutil::GetProfileInt("Camera UDP", "ReceivePort", 8888, fileName);
	const string gameClientIp = uiutil::GetProfileString("Game UDP", "ClientIP", "127.0.0.1", fileName);
	const int gameClientPort = uiutil::GetProfileInt("Game UDP", "ClientPort", 8889, fileName);

	m_cameraUDPReceiver.Init(1, cameraPort);
	m_gameClientSender.Init(gameClientIp, gameClientPort, 10);

	return true;
}


// 매 프레임마다 호출.
void cRealShotController::Update(const float deltaSeconds)
{
	RET(!m_isPlayGame);

	char camBuff[256];
	const int camBuffLen = m_cameraUDPReceiver.GetRecvData(camBuff, sizeof(camBuff));
	if (camBuffLen > 0)
	{
		// 총 좌표에 관련된 정보는 카메라 프로그램으로부터 받는다.
		SMGCameraData *rcvPacket = (SMGCameraData*)camBuff;

		SMGCameraData sndPacket;
		ZeroMemory(&sndPacket, sizeof(sndPacket));
		sndPacket.x1 = rcvPacket->x1;
		sndPacket.y1 = rcvPacket->y1;
		sndPacket.x2 = rcvPacket->x2;
		sndPacket.y2 = rcvPacket->y2;

		sndPacket.fire1 = ((GetKeyState(VK_LBUTTON) & 0x100) != 0);

		// 마우스 이동 처리
		const int screenWidth = GetSystemMetrics(SM_CXSCREEN);
		const int screenHeight = GetSystemMetrics(SM_CYSCREEN);
		
		const float x = rcvPacket->x1 * screenWidth;
		const float y = (1-rcvPacket->y1) * screenHeight;

		POINT mousePos;
		GetCursorPos(&mousePos);
		const float oldX = (float)mousePos.x;
		const float oldY = (float)mousePos.y;

		const int dx = (int)(x - oldX);
		const int dy = (int)(y - oldY);

		if (CMainFrame *pFrm = dynamic_cast<CMainFrame*>(AfxGetMainWnd()))
		{
			pFrm->m_realShotView->m_PrintStr.SetWindowTextW(common::formatw("%.1f, %.1f, %.1f, %.1f, %d, %d", 
				x, y, oldX, oldY, screenWidth, screenHeight).c_str());
		}

		if ((abs(dx) > 0) && (abs(dy) > 0))
		{
			static INPUT input;
			input.type = INPUT_MOUSE;
			//input.mi.dwFlags = MOUSEEVENTF_MOVE;
// 			input.mi.dx = dx;
// 			input.mi.dy = dy;
			input.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;
			input.mi.dx = (int)x;
			input.mi.dy = (int)y;
			input.mi.mouseData = 0;
			//SendInput(1, &input, sizeof(INPUT));

			SetCursorPos((int)x, (int)y); 
		}

		m_gameClientSender.SendData((BYTE*)&sndPacket, sizeof(sndPacket));
	}

}


// INIT -> ORIGIN
void cRealShotController::StartMotionSim(const string &configFileName, const bool isStartMotionSimOut)
{
	if (CMainFrame *pFrm = dynamic_cast<CMainFrame*>(AfxGetMainWnd()))
	{
		// UDP View, Mixing View, Output View Start
		// 안전을 위해 순서를 지키자.
		//pFrm->m_udpInputView->GetChildView()->Start();
		if (pFrm->m_udpParseView)
			pFrm->m_udpParseView->Start();
		if (pFrm->m_mixingView)
			pFrm->m_mixingView->Start();
		if (pFrm->m_motionWaveView)
			pFrm->m_motionWaveView->Start();

		if (isStartMotionSimOut)
			if (pFrm->m_motionOutputView)
				pFrm->m_motionOutputView->Start();

		m_configFileName = configFileName;

		m_isPlayGame = true;
	}
}


// END -> STOP
void cRealShotController::StopMotionSim()
{
	if (CMainFrame *pFrm = dynamic_cast<CMainFrame*>(AfxGetMainWnd()))
	{
		// 안전을 위해 순서를 지키자.
		//pFrm->m_udpInputView->GetChildView()->Stop();
		if (pFrm->m_udpParseView)
			pFrm->m_udpParseView->Stop();
		if (pFrm->m_mixingView)
			pFrm->m_mixingView->Stop();
		if (pFrm->m_joystickView)
			pFrm->m_joystickView->Stop();
		if (pFrm->m_motionWaveView)
			pFrm->m_motionWaveView->Stop();

		m_isPlayGame = false;
	}
}


// 모션 시뮬레이션을 시작한다.
// 모션웨이브를 로딩하고, 모션 시뮬레이터를 작동시킨다.
void cRealShotController::GameStart(const int mission)
{
	m_isPlayGame = true;
	m_gameState = 1;
	//	m_changeInformationPulse = true;
	m_gameMission = mission;
	m_playTime = 0;
}


// 모션 시뮬레이터를 대기상태로 만든다.
void cRealShotController::GameStop()
{
	m_isPlayGame = false;
	m_gameState = 0;
	//	m_changeInformationPulse = true;
}


// 게임 상태가 바뀌면, true를 리턴하고, false 로 설정된다.
bool cRealShotController::CheckChangeState()
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


// 매인보드로부터 오는 신호를 처리한다.
void cRealShotController::MainBoardProcess(const char *buff, const int size)
{
	sMGDevicePacket *packet = (sMGDevicePacket*)buff;
}



// CString cRealShotController::GetInnerStateName()
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
