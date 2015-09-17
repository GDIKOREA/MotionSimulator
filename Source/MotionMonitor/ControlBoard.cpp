// ControlBoard.cpp : implementation file
//

#include "stdafx.h"
#include "MotionMonitor.h"
#include "ControlBoard.h"
#include "afxdialogex.h"
#include "MachineGunController.h"
#include "Dirt3Controller.h"
#include "SimpleController.h"
#include "MainFrm.h"


// CControlBoard dialog
const static string g_configFileNames[] =
{
	"../media/machinegun/motionmonitor_dirt3.json",
	"../media/machinegun/motionmonitor_mg.json",
	"../media/machinegun/motionmonitor_joystick.json",
	"../media/machinegun/motionmonitor_mwave.json",
};


CControlBoard::CControlBoard(CWnd* pParent /*=NULL*/)
	: CDockablePaneChildView(CControlBoard::IDD, pParent)
	, m_GameType(1)
	, m_GameState(_T(""))
	, m_GameTime(_T(""))
	, m_GameMission(_T(""))
	, m_UDPUpdateTime(_T(""))
	, m_InnerState(_T(""))
	, m_state(STOP)
	, m_IsPlayMotionSim(FALSE)
{
}

CControlBoard::~CControlBoard()
{
	cMachineGunController::Release();
	cDirt3Controller::Release();
	cSimpleController::Release();
}

void CControlBoard::DoDataExchange(CDataExchange* pDX)
{
	CDockablePaneChildView::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_DIRT3, m_GameType);
	DDX_Text(pDX, IDC_STATIC_GAMESTATE, m_GameState);
	DDX_Text(pDX, IDC_STATIC_GAMETIME, m_GameTime);
	DDX_Text(pDX, IDC_STATIC_GAMESTAGE, m_GameMission);
	DDX_Text(pDX, IDC_STATIC_UDPUPDATE_TIME, m_UDPUpdateTime);
	DDX_Text(pDX, IDC_STATIC_INSTATE, m_InnerState);
	DDX_Control(pDX, IDC_BUTTON_START, m_StartButton);
	DDX_Control(pDX, IDC_RADIO_DIRT3, m_Dirt3RadioButton);
	DDX_Check(pDX, IDC_CHECK_PLAY_MOTIONSIM, m_IsPlayMotionSim);
}


BEGIN_MESSAGE_MAP(CControlBoard, CDockablePaneChildView)
	ON_BN_CLICKED(IDOK, &CControlBoard::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CControlBoard::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_RADIO_DIRT3, &CControlBoard::OnBnClickedRadioDirt3)
	ON_BN_CLICKED(IDC_RADIO_MG, &CControlBoard::OnBnClickedRadioMg)
	ON_BN_CLICKED(IDC_BUTTON_EMERGENCY, &CControlBoard::OnBnClickedButtonEmergency)
	ON_BN_CLICKED(IDC_RADIO_JOYSTICK, &CControlBoard::OnBnClickedRadioJoystick)
	ON_BN_CLICKED(IDC_RADIO_MWAVE, &CControlBoard::OnBnClickedRadioMwave)
	ON_BN_CLICKED(IDC_BUTTON_START, &CControlBoard::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_CHECK_PLAY_MOTIONSIM, &CControlBoard::OnBnClickedCheckPlayMotionsim)
END_MESSAGE_MAP()


// CControlBoard message handlers
void CControlBoard::OnBnClickedOk()
{
}
void CControlBoard::OnBnClickedCancel()
{
}


BOOL CControlBoard::OnInitDialog()
{
	CDockablePaneChildView::OnInitDialog();

	return TRUE;
}


void CControlBoard::OnBnClickedRadioDirt3()
{
	UpdateData();
	g_gameType = GAME_TYPE::DIRT3;

	if (CMainFrame *pFrm = dynamic_cast<CMainFrame*>(AfxGetMainWnd()))
		pFrm->LoadConfigFile(g_configFileNames[g_gameType]);
}


void CControlBoard::OnBnClickedRadioMg()
{
	UpdateData();
	g_gameType = GAME_TYPE::MACHINEGUN;

	if (CMainFrame *pFrm = dynamic_cast<CMainFrame*>(AfxGetMainWnd()))
		pFrm->LoadConfigFile(g_configFileNames[g_gameType]);
}


void CControlBoard::OnBnClickedRadioJoystick()
{
	UpdateData();
	g_gameType = GAME_TYPE::JOYSTICK;

	if (CMainFrame *pFrm = dynamic_cast<CMainFrame*>(AfxGetMainWnd()))
		pFrm->LoadConfigFile(g_configFileNames[g_gameType]);
}


void CControlBoard::OnBnClickedRadioMwave()
{
	UpdateData();
	g_gameType = GAME_TYPE::MWAVE;

	if (CMainFrame *pFrm = dynamic_cast<CMainFrame*>(AfxGetMainWnd()))
		pFrm->LoadConfigFile(g_configFileNames[g_gameType]);
}


// UI정보를 업데이트 한다.

void CControlBoard::Update(const float deltaSeconds)
{
	switch (g_gameType)
	{
	case GAME_TYPE::DIRT3:
		{
			cDirt3Controller::Get()->Update(deltaSeconds);

// 		if (cMachineGunController::Get()->CheckChangeState())
// 		{
// 			switch (cMachineGunController::Get()->m_gameState)
// 			{
// 			case 0: m_GameState = L"Wait"; break;
// 			case 1: m_GameState = L"Playing"; break;
// 			default:
//				break;
// 			}
// 
// 			m_InnerState = cMachineGunController::Get()->GetInnerStateName();
// 			m_GameMission.Format(L"%d", cMachineGunController::Get()->m_gameMission);
// 			UpdateData(FALSE);
// 		}
//
// 		m_GameTime.Format(L"%.1f", cMachineGunController::Get()->m_playTime);
// 		UpdateData(FALSE);
		}
		break;

	case GAME_TYPE::MACHINEGUN:
		{
			cMachineGunController::Get()->Update(deltaSeconds);

			if (cMachineGunController::Get()->CheckChangeState())
			{
				switch (cMachineGunController::Get()->m_gameState)
				{
				case 0: m_GameState = L"Wait"; break;
				case 1: m_GameState = L"Playing"; break;
				default:
					break;
				}

				//m_InnerState = cMachineGunController::Get()->GetInnerStateName();

				// 속도를 높이기위해 주석
				//m_GameMission.Format(L"%d", cMachineGunController::Get()->m_gameMission);
				//UpdateData(FALSE);
			}

			// 속도를 높이기위해 주석
			//m_GameTime.Format(L"%.1f", cMachineGunController::Get()->m_playTime);
			//UpdateData(FALSE);
		}
		break;
	case GAME_TYPE::JOYSTICK:
	case GAME_TYPE::MWAVE:
		cSimpleController::Get()->Update(deltaSeconds);
		break;
	default:
		break;
	}
}


// 머신건에서 UDP 로 패킷을 전송하면, 호출된다.
void CControlBoard::UpdateUDP(const char *buffer, const int bufferLen)
{
	RET(m_state != START);
	RET(bufferLen < 10);

	if (GAME_TYPE::MACHINEGUN == g_gameType) // MachineGun Motion Simulation
	{
		if (bufferLen < sizeof(sMotionPacket))
			return;

		sMotionPacket *packet = (sMotionPacket*)buffer;
		cMachineGunController::Get()->UpdateUDP(*packet);

 		m_UDPUpdateTime.Format(L"%.1f", cMachineGunController::Get()->m_lastUDPUpdateTime);
 		UpdateData(FALSE);
	}
}


void CControlBoard::Start()
{
	UpdateData();

	m_state = START;

	switch (g_gameType)
	{
	case GAME_TYPE::DIRT3: cDirt3Controller::Get()->StartMotionSim(g_configFileNames[m_GameType]); break;
	case GAME_TYPE::MACHINEGUN: cMachineGunController::Get()->StartMotionSim(g_configFileNames[m_GameType], m_IsPlayMotionSim ? true : false); break;
	case GAME_TYPE::JOYSTICK:
	case GAME_TYPE::MWAVE: 
		cSimpleController::Get()->StartMotionSim(g_configFileNames[m_GameType], m_IsPlayMotionSim? true:false);
		break;
	default:
		break;
	}

	GetDlgItem(IDC_RADIO_DIRT3)->EnableWindow(FALSE);
	GetDlgItem(IDC_RADIO_MG)->EnableWindow(FALSE);
	GetDlgItem(IDC_RADIO_JOYSTICK)->EnableWindow(FALSE);
	GetDlgItem(IDC_RADIO_MWAVE)->EnableWindow(FALSE);

	m_StartButton.SetWindowTextW(L"Stop");
	SetBackgroundColor(g_blueColor);
}


void CControlBoard::Stop()
{
	m_state = STOP;

	switch (g_gameType)
	{
	case GAME_TYPE::DIRT3: cDirt3Controller::Get()->StopMotionSim(); break;
	case GAME_TYPE::MACHINEGUN: cMachineGunController::Get()->StopMotionSim(); break;
	case GAME_TYPE::JOYSTICK:
	case GAME_TYPE::MWAVE: 
		cSimpleController::Get()->StopMotionSim();
		break;
	default:
		break;
	}

	GetDlgItem(IDC_RADIO_DIRT3)->EnableWindow(TRUE);
	GetDlgItem(IDC_RADIO_MG)->EnableWindow(TRUE);
	GetDlgItem(IDC_RADIO_JOYSTICK)->EnableWindow(TRUE);
	GetDlgItem(IDC_RADIO_MWAVE)->EnableWindow(TRUE);

	m_StartButton.SetWindowTextW(L"Start");
	SetBackgroundColor(g_grayColor);
}


void CControlBoard::OnBnClickedButtonEmergency()
{
	Stop();
}


void CControlBoard::OnBnClickedButtonStart()
{
	switch (m_state)
	{
	case CControlBoard::START: Stop(); break;
	case CControlBoard::STOP: Start(); break;
	default:
		break;
	}
}


void CControlBoard::OnBnClickedCheckPlayMotionsim()
{
	UpdateData();
}

