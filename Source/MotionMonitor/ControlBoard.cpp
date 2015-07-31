// ControlBoard.cpp : implementation file
//

#include "stdafx.h"
#include "MotionMonitor.h"
#include "ControlBoard.h"
#include "afxdialogex.h"
#include "MachineGunController.h"


// CControlBoard dialog

CControlBoard::CControlBoard(CWnd* pParent /*=NULL*/)
	: CDockablePaneChildView(CControlBoard::IDD, pParent)
	, m_GameType(1)
	, m_GameState(_T(""))
	, m_GameTime(_T(""))
	, m_GameMission(_T(""))
	, m_IsMotionSimulation(FALSE)
	, m_UDPUpdateTime(_T(""))
	, m_InnerState(_T(""))
{

}

CControlBoard::~CControlBoard()
{
}

void CControlBoard::DoDataExchange(CDataExchange* pDX)
{
	CDockablePaneChildView::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_DIRT3, m_GameType);
	DDX_Text(pDX, IDC_STATIC_GAMESTATE, m_GameState);
	DDX_Text(pDX, IDC_STATIC_GAMETIME, m_GameTime);
	DDX_Text(pDX, IDC_STATIC_GAMESTAGE, m_GameMission);
	DDX_Check(pDX, IDC_CHECK_MOTION_SIMULATION, m_IsMotionSimulation);
	DDX_Text(pDX, IDC_STATIC_UDPUPDATE_TIME, m_UDPUpdateTime);
	DDX_Text(pDX, IDC_STATIC_INSTATE, m_InnerState);
}


BEGIN_MESSAGE_MAP(CControlBoard, CDockablePaneChildView)
	ON_BN_CLICKED(IDOK, &CControlBoard::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CControlBoard::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_RADIO_DIRT3, &CControlBoard::OnBnClickedRadioDirt3)
	ON_BN_CLICKED(IDC_RADIO_MG, &CControlBoard::OnBnClickedRadioMg)
	ON_BN_CLICKED(IDC_CHECK_MOTION_SIMULATION, &CControlBoard::OnBnClickedCheckMotionSimulation)
	ON_BN_CLICKED(IDC_BUTTON_EMERGENCY, &CControlBoard::OnBnClickedButtonEmergency)
END_MESSAGE_MAP()


// CControlBoard message handlers
void CControlBoard::OnBnClickedOk()
{
//	CDockablePaneChildView::OnOK();
}


void CControlBoard::OnBnClickedCancel()
{
//	CDockablePaneChildView::OnCancel();
}


void CControlBoard::OnBnClickedRadioDirt3()
{
	UpdateData();
	g_gameType = GAME_TYPE::DIRT3;
}


void CControlBoard::OnBnClickedRadioMg()
{
	UpdateData();
	g_gameType = GAME_TYPE::MACHINEGUN;
}


// 매 프레임마다 호출된다.
void CControlBoard::Update(const float deltaSeconds)
{

	if (GAME_TYPE::MACHINEGUN == g_gameType) // MachineGun Motion Simulation
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

			m_InnerState = cMachineGunController::Get()->GetInnerStateName();
			m_GameMission.Format(L"%d", cMachineGunController::Get()->m_gameMission);
			UpdateData(FALSE);
		}

		m_GameTime.Format(L"%.1f", cMachineGunController::Get()->m_playTime);
		UpdateData(FALSE);
	}

}


// 머신건에서 UDP 로 패킷을 전송하면, 호출된다.
void CControlBoard::UpdateUDP(const char *buffer, const int bufferLen)
{
	RET(!m_IsMotionSimulation);
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


void CControlBoard::OnBnClickedCheckMotionSimulation()
{
	UpdateData();

	if (m_IsMotionSimulation)
	{
		cMachineGunController::Get()->StartMotionSim();
	}
	else
	{
		cMachineGunController::Get()->StopMotionSim();
	}

}


void CControlBoard::OnBnClickedButtonEmergency()
{
	
}
