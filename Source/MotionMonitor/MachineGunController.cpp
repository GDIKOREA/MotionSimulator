
#include "stdafx.h"
#include "MachineGunController.h"

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



cMachineGunController::cMachineGunController() :
	m_isPlayGame(false)
	, m_gameState(0)
	, m_gameMission(-1)
	, m_playTime(0)
	, m_lastUDPUpdateTime(0)
	, m_oldState(cVitconMotionSim::OFF)
	, m_credit(0)
	, m_coin(0)
	, m_coinPerGame(2)
	, m_checkCoin(false)
	, m_coinDownEdgeTime(0)
	, m_coinUpEdgeTime(0)
	, m_checkCreditDown1(false)
	, m_creditUpEdgeTime1(0)
	, m_checkCreditDown2(false)
	, m_creditUpEdgeTime2(0)
	, m_activeGunFire1(false)
	, m_activeGunFire2(false)
	, m_totalGameCount(0)
	, m_totalCredit(0)
	, m_totalInputCredit(0)

{
	ZeroMemory(&m_devicePacket, sizeof(m_devicePacket));
}

cMachineGunController::~cMachineGunController()
{
	// 종료될 때, 기록들을 저장한다.
	using namespace std;

	ofstream ofs("gameinfo.txt", ios::app); // 파일에 정보를 추가한다.
	if (ofs.is_open())
	{
		SYSTEMTIME sysTime;
		GetLocalTime(&sysTime);
		ofs << sysTime.wYear << "-" << sysTime.wMonth << "-" << sysTime.wDay << " " << sysTime.wHour << ":" << sysTime.wMinute << ":" << sysTime.wSecond << "\t";
		ofs << m_totalGameCount << "\t" << m_totalCredit << "\t" << m_totalInputCredit << endl;
	}

}


// UDP 통신은 프로그램이 시작된 이후 부터 계속 처리한다.
bool cMachineGunController::Init()
{
	const string fileName = "../media/machinegun/mg_controller.ini";
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


	m_mainBoardInput.Init(0, receivePort);
	m_mainBoardSender.Init(mainBoardIp, mainBoardPort, 10);
	m_mainBoardProxy.Init(proxyIp, proxyPort, 10); // send to camera

	m_cameraUDPReceiver.Init(1, cameraPort);
	m_gameClientSender.Init(gameClientIp, gameClientPort, 10);

	ActiveMainBoard();

	return true;
}


// 매 프레임마다 호출.
void cMachineGunController::Update(const float deltaSeconds)
{
	m_vitconMotionSim.Update(deltaSeconds);

	const int motionSimState = m_vitconMotionSim.GetState();
	if (motionSimState != m_oldState)
	{
		switch (motionSimState)
		{
		case cVitconMotionSim::OFF:
			// ServoOff 상태로 바뀔 때, MotionOutputView도 같이 Stop 된다.
			if (CMainFrame *pFrm = dynamic_cast<CMainFrame*>(AfxGetMainWnd()))
			{
				pFrm->m_motionOutputView->Stop();
				pFrm->m_motionWaveView->Stop();
			}
			break;

		case cVitconMotionSim::READY:
			break;

		default:
			break;
		}

		m_oldState = m_vitconMotionSim.GetState();
	}

	
	BYTE hwBuff[256];
	const int devBuffLen = m_mainBoardInput.GetRecvData(hwBuff, sizeof(hwBuff));
	if (devBuffLen > 0)
	{
		memcpy(&m_devicePacket, hwBuff, sizeof(m_devicePacket));
		m_mainBoardProxy.SendData((BYTE*)hwBuff, devBuffLen);

		MainBoardProcess(hwBuff, devBuffLen);
	}

	BYTE camBuff[256];
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
		
		sndPacket.fire1 = (m_devicePacket.player1Fire == '1') ? 1 : 0;
		sndPacket.start1 = (m_devicePacket.player1Start == '1') ? 1 : 0;
		sndPacket.reload1 = (m_devicePacket.player1Reload == '1') ? 1 : 0;

		sndPacket.fire2 = (m_devicePacket.player2Fire == '1') ? 1 : 0;
		sndPacket.start2 = (m_devicePacket.player2Start == '1') ? 1 : 0;
		sndPacket.reload2 = (m_devicePacket.player2Reload == '1') ? 1 : 0;

		sndPacket.credit = m_credit;
		sndPacket.coinCount = m_coin;
		sndPacket.coinPerGame = m_coinPerGame;

		m_gameClientSender.SendData((BYTE*)&sndPacket, sizeof(sndPacket));
	}

}


// 패킷이 올때마다 호출된다.
// 머신건 게임 모션 시뮬레이터를 위한 작업을 한다.
void cMachineGunController::UpdateUDP(const sMotionPacket &packet)
{
	// 게임중일 때
	if (m_isPlayGame)
	{
		m_lastUDPUpdateTime = cController::Get()->GetGlobalSeconds();

		// 게임 종료.
		if (0 == packet.gamestate)
		{
			GameStop();

			// 총,모터를 활성 상태로 만든다.
			ActiveMainBoard();

			// 총 게임횟수 증가
			++m_totalGameCount;

			g_launcherView->UpdateGameInfo(m_totalCredit, m_totalGameCount);
		}
	}

	CheckCreditPulse(packet);
	CheckReload(packet);

	if (m_vitconMotionSim.GetState() == cVitconMotionSim::READY)
	{
		// 게임중이 아닐 때, gamestate == 1이 되면, 게임이 시작된다.
		if (!m_isPlayGame && (1 == packet.gamestate))
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
		m_vitconMotionSim.On();
	}
}


// END -> STOP
void cMachineGunController::StopMotionSim()
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

		m_vitconMotionSim.Off();
	}
}


// 모션 시뮬레이션을 시작한다.
// 모션웨이브를 로딩하고, 모션 시뮬레이터를 작동시킨다.
void cMachineGunController::GameStart(const int mission)
{
	m_isPlayGame = true;
	m_gameState = 1;
//	m_changeInformationPulse = true;
	m_gameMission = mission;
	m_playTime = 0;

	if (g_mwaveView)
		g_mwaveView->LoadandPlayMotionWave("../media/machinegun/motion wave/scene1.mwav");

	m_vitconMotionSim.Play();
}


// 모션 시뮬레이터를 대기상태로 만든다.
void cMachineGunController::GameStop()
{
	m_isPlayGame = false;
	m_gameState = 0;
	//	m_changeInformationPulse = true;

	m_vitconMotionSim.Ready();
}


// 게임 상태가 바뀌면, true를 리턴하고, false 로 설정된다.
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


// 매인보드로부터 오는 신호를 처리한다.
void cMachineGunController::MainBoardProcess(const BYTE *buff, const int size)
{
	sMGDevicePacket *packet = (sMGDevicePacket*)buff;

	if (m_checkCoin)
	{
		if ('0' == packet->coin)
		{
			m_checkCoin = false;
			m_coinDownEdgeTime = timeGetTime();

			++m_coin;

			if (m_coin >= m_coinPerGame)
			{
				++m_credit;
				m_coin %= m_coinPerGame;
				++m_totalInputCredit;
			}

			// UI를 갱신한다.
			g_launcherView->UpdateCoin(m_credit, m_coin, m_coinPerGame);
		}
	}
	else
	{
		// 코인 신호가 들어오면~
		if ('1' == packet->coin)
		{
			// 전 입력과 1초 이상 차이가 났을 때만, 코인입력으로 간주한다.
			if ((timeGetTime() - m_coinUpEdgeTime) > 200)
			{
				m_checkCoin = true;
				m_coinUpEdgeTime = timeGetTime();
			}
		}
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


// 머신건 장치를 모두 사용할 수 있도록, 활성화 한다.
void cMachineGunController::ActiveMainBoard()
{
	sMGDevicePacket sndPacket;
	ZeroMemory(&sndPacket, sizeof(sndPacket));
	sndPacket.header = '$';
	sndPacket.comma1 = ',';
	sndPacket.deviceNumber = '9';
	sndPacket.zero1 = '0';
	sndPacket.comma2 = ',';

	sndPacket.player1Fire = '1';
	sndPacket.player1FireEvent = '1';
	sndPacket.player1Reload = '1';
	sndPacket.player1Start = '1';
	sndPacket.player1UpMotor = '1';
	sndPacket.player1DownMotor = '1';
	sndPacket.player1UpSensor = '1';
	sndPacket.player1DownSensor = '1';
	sndPacket.player1EmergencySwitch = '0';
	sndPacket.coin = '0';
	sndPacket.space1 = '0';

	sndPacket.player2Fire = '1';
	sndPacket.player2FireEvent = '1';
	sndPacket.player2Reload = '1';
	sndPacket.player2Start = '1';
	sndPacket.player2UpMotor = '1';
	sndPacket.player2DownMotor = '1';
	sndPacket.player2UpSensor = '1';
	sndPacket.player2DownSensor = '1';
	sndPacket.player2EmergencySwitch = '0';
	sndPacket.space3 = '0';
	sndPacket.space4 = '0';

	sndPacket.comma3 = ',';
	sndPacket.at = '@';

	sndPacket.cr[0] = (char)0x0d;
	sndPacket.cr[1] = (char)0x0a;

	m_mainBoardSender.SendData((BYTE*)&sndPacket, sizeof(sndPacket));
}


// 머신건 총 활성/비활성 상태 설정
void cMachineGunController::ActiveGunFire(const bool active1, const bool active2)
{
	sMGDevicePacket sndPacket;
	ZeroMemory(&sndPacket, sizeof(sndPacket));
	sndPacket.header = '$';
	sndPacket.comma1 = ',';
	sndPacket.deviceNumber = '9';
	sndPacket.zero1 = '0';
	sndPacket.comma2 = ',';

	sndPacket.player1Fire = (active1)? '1' : '0';
	sndPacket.player1FireEvent = '1';
	sndPacket.player1Reload = '1';
	sndPacket.player1Start = '1';
	sndPacket.player1UpMotor = '1';
	sndPacket.player1DownMotor = '1';
	sndPacket.player1UpSensor = '1';
	sndPacket.player1DownSensor = '1';
	sndPacket.player1EmergencySwitch = '0';
	sndPacket.coin = '0';
	sndPacket.space1 = '0';

	sndPacket.player2Fire = (active2) ? '1' : '0';
	sndPacket.player2FireEvent = '1';
	sndPacket.player2Reload = '1';
	sndPacket.player2Start = '1';
	sndPacket.player2UpMotor = '1';
	sndPacket.player2DownMotor = '1';
	sndPacket.player2UpSensor = '1';
	sndPacket.player2DownSensor = '1';
	sndPacket.player2EmergencySwitch = '0';
	sndPacket.space3 = '0';
	sndPacket.space4 = '0';

	sndPacket.comma3 = ',';
	sndPacket.at = '@';

	sndPacket.cr[0] = (char)0x0d;
	sndPacket.cr[1] = (char)0x0a;

	m_mainBoardSender.SendData((BYTE*)&sndPacket, sizeof(sndPacket));
}


// Credit 신호를 검사해서 처리한다.
void cMachineGunController::CheckCreditPulse(const sMotionPacket &packet)
{
	// Player1 Credit Pulse
	if (m_checkCreditDown1) // 신호가 발생된 후, 하강에지를 기다린다.
	{
		// Credit 하강에지
		if (packet.creditDown1 == 0)
		{
			m_checkCreditDown1 = false;

			--m_credit;
			if (m_credit < 0)
				m_credit = 0;

			// 총 크래딧 증가
			++m_totalCredit;

			// UI를 갱신한다.
			g_launcherView->UpdateCoin(m_credit, m_coin, m_coinPerGame);
			g_launcherView->UpdateGameInfo(m_totalCredit, m_totalGameCount);
		}
	}
	else
	{
		// Credit 상승에지 
		if (packet.creditDown1 == 1)
		{
			// 최소 1초 이상의 간격이 있을 때만 인식하게 한다.
			if (timeGetTime()-m_creditUpEdgeTime1 > 1000)
			{
				m_creditUpEdgeTime1 = timeGetTime();
				m_checkCreditDown1 = true;
			}
		}
	}


	// Player2 Credit Pulse
	if (m_checkCreditDown2) // 신호가 발생된 후, 하강에지를 기다린다.
	{
		// Credit 하강에지
		if (packet.creditDown2 == 0)
		{
			m_checkCreditDown2 = false;

			--m_credit;
			if (m_credit < 0)
				m_credit = 0;

			// 총 크래딧 증가
			++m_totalCredit;

			// UI를 갱신한다.
			g_launcherView->UpdateCoin(m_credit, m_coin, m_coinPerGame);
			g_launcherView->UpdateGameInfo(m_totalCredit, m_totalGameCount);
		}
	}
	else
	{
		// Credit 상승에지 
		if (packet.creditDown2 == 1)
		{
			// 최소 1초 이상의 간격이 있을 때만 인식하게 한다.
			if (timeGetTime() - m_creditUpEdgeTime2 > 1000)
			{
				m_creditUpEdgeTime2 = timeGetTime();
				m_checkCreditDown2 = true;
			}
		}
	}

}


// 총알이 없을 때, 리로드가 될 때까지 총이 발사되지 않게 한다.
void cMachineGunController::CheckReload(const sMotionPacket &packet)
{

	if (m_activeGunFire1 && packet.bulletcount1 <= 0)
	{
		m_activeGunFire1 = false;
		ActiveGunFire(m_activeGunFire1, m_activeGunFire2);
	}
	else if (!m_activeGunFire1 && packet.bulletcount1 > 0)
	{
		m_activeGunFire1 = true;
		ActiveGunFire(m_activeGunFire1, m_activeGunFire2);
	}

	if (m_activeGunFire2 && packet.bulletcount2 <= 0)
	{
		m_activeGunFire2 = false;
		ActiveGunFire(m_activeGunFire1, m_activeGunFire2);
	}
	else if (!m_activeGunFire2 && packet.bulletcount2 > 0)
	{
		m_activeGunFire2 = true;
 		ActiveGunFire(m_activeGunFire1, m_activeGunFire2);
	}

}

