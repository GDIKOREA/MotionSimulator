// MotionOutputView.cpp : implementation file
//

#include "stdafx.h"
#include "MotionMonitor.h"
#include "MotionOutputView.h"
#include "afxdialogex.h"


const static CString g_outputViewPlotCommand = L"\r\n\
plot1 = 0, 0, 0, 0, 0\r\n\
string1 = %f;\r\n\
name1 = Yaw\r\n\
linewidth1 = 2\r\n\
plot2 = 0, 0, 0, 0, 0\r\n\
string2 = %*f; %f;\r\n\
name2 = Pitch\r\n\
linewidth2 = 2\r\n\
plot3 = 0, 0, 0, 0, 0\r\n\
string3 = %*f; %*f; %f;\r\n\
name3 = Roll\r\n\
linewidth3 = 2\r\n\
plot4 = 0, 0, 0, 0, 0\r\n\
string4 = %*f; %*f; %*f; %f;\r\n\
name4 = Heave\r\n\
linewidth4 = 2\r\n\
";


// CMotionOutputView dialog

CMotionOutputView::CMotionOutputView(CWnd* pParent /*=NULL*/)
: CDockablePaneChildView(CMotionOutputView::IDD, pParent)
	, m_isStartSendMotion(false)
	, m_incTime(0)
	, m_totalIncTime(0)
	, m_incSerialTime(0)
	, m_multiPlotWindows(NULL)
	, m_IsOnlyEmergency(FALSE)
{
}

CMotionOutputView::~CMotionOutputView()
{
}

void CMotionOutputView::DoDataExchange(CDataExchange* pDX)
{
	CDockablePaneChildView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_COM_PORTS, m_COMPortComboBox);
	DDX_Control(pDX, IDC_COMBO_COM_BAUDRATE, m_BaudRateComboBox);
	DDX_Control(pDX, IDC_BUTTON_CONNECT, m_ConnectButton);
	DDX_Control(pDX, IDC_BUTTON_START_SEND_MOTION, m_StartSendMotionButton);
	DDX_Control(pDX, IDC_RICHEDIT2_LOG, m_Log);
	DDX_Control(pDX, IDC_RICHEDIT2_OUTPUT_LOG, m_OutputLog);
	DDX_Check(pDX, IDC_CHECK1, m_IsOnlyEmergency);
}


BEGIN_ANCHOR_MAP(CMotionOutputView)
	ANCHOR_MAP_ENTRY(IDC_STATIC_GRAPH, ANF_LEFT | ANF_TOP | ANF_RIGHT | ANF_BOTTOM)
	ANCHOR_MAP_ENTRY(IDC_RICHEDIT2_LOG, ANF_TOP | ANF_LEFT | ANF_RIGHT)
	ANCHOR_MAP_ENTRY(IDC_RICHEDIT2_OUTPUT_LOG, ANF_TOP | ANF_LEFT | ANF_RIGHT)
END_ANCHOR_MAP()

BEGIN_MESSAGE_MAP(CMotionOutputView, CDockablePaneChildView)
	ON_BN_CLICKED(IDOK, &CMotionOutputView::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CMotionOutputView::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_CONNECT, &CMotionOutputView::OnBnClickedButtonConnect)
	ON_BN_CLICKED(IDC_BUTTON_START_SEND_MOTION, &CMotionOutputView::OnBnClickedButtonStartSendMotion)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_START, &CMotionOutputView::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CMotionOutputView::OnBnClickedButtonStop)
	ON_BN_CLICKED(IDC_BUTTON_ORIGIN, &CMotionOutputView::OnBnClickedButtonOrigin)
	ON_BN_CLICKED(IDC_BUTTON_EMERGENCY, &CMotionOutputView::OnBnClickedButtonEmergency)
	ON_BN_CLICKED(IDC_BUTTON_SERVO_ON, &CMotionOutputView::OnBnClickedButtonServoOn)
	ON_BN_CLICKED(IDC_BUTTON_SERVO_OFF, &CMotionOutputView::OnBnClickedButtonServoOff)
	ON_BN_CLICKED(IDC_CHECK1, &CMotionOutputView::OnBnClickedCheck1)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CMotionOutputView message handlers


void CMotionOutputView::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//CDockablePaneChildView::OnOK();
}


void CMotionOutputView::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	//CDockablePaneChildView::OnCancel();
}


BOOL CMotionOutputView::OnInitDialog()
{
	CDockablePaneChildView::OnInitDialog();

	InitAnchors();

	// Plot창 생성.
	CRect rect;
	GetClientRect(rect);

	m_multiPlotWindows = new CMultiPlotWindow();
	BOOL result = m_multiPlotWindows->Create(NULL, NULL, WS_VISIBLE | WS_CHILD,
		CRect(0, 0, rect.Width(), 400), this, AFX_IDW_PANE_FIRST);

	m_multiPlotWindows->SetScrollSizes(MM_TEXT, CSize(rect.Width() - 30, 100));
	m_multiPlotWindows->ShowWindow(SW_SHOW);

	const int baudRate[] = { 9600, 14400, 19200, 38400, 56000, 57600, 115200 };
	for (int i = 0; i < ARRAYSIZE(baudRate); ++i)
	{
		TCHAR brateName[32];
		wsprintf(brateName, L"%d", baudRate[i]);
		m_BaudRateComboBox.InsertString(m_BaudRateComboBox.GetCount(), brateName);
	}

	UpdateConfig();

	m_Log.SetBackgroundColor(FALSE, RGB(0, 0, 0));
	m_OutputLog.SetBackgroundColor(FALSE, RGB(0, 0, 0));

	return TRUE;
}


void CMotionOutputView::UpdateConfig(bool IsSaveAndValidate) //IsSaveAndValidate=true
{
	if (IsSaveAndValidate)
	{
		m_COMPortComboBox.InitList(cMotionController::Get()->m_config.m_motionOutputComPort);
		m_BaudRateComboBox.SetCurSel(cMotionController::Get()->m_config.m_motionOutputBaudRate);
		UpdateData(FALSE);
	}
	else
	{
		UpdateData();

		cMotionController::Get()->m_config.m_motionOutputComPort = m_COMPortComboBox.GetPortNum();
		cMotionController::Get()->m_config.m_motionOutputBaudRate = m_BaudRateComboBox.GetCurSel();
	}
}


// 시리얼 포트와 연결한다.
void CMotionOutputView::OnBnClickedButtonConnect()
{
	const bool isConnect = cController::Get()->GetSerialComm().IsConnect();

	if (isConnect)
	{
		cController::Get()->CloseSerial();
		m_ConnectButton.SetWindowTextW(L"Start");

		SetBackgroundColor(g_grayColor);
	}
	else
	{
		m_multiPlotWindows->ProcessPlotCommand(g_outputViewPlotCommand, 2);
		m_multiPlotWindows->SetFixedWidthMode(true);

		const int portNumber = m_COMPortComboBox.GetPortNum();
		CString baudRate;
		m_BaudRateComboBox.GetWindowTextW(baudRate);

		if (cController::Get()->ConnectSerial(portNumber, _wtoi(baudRate)))
		{
			m_ConnectButton.SetWindowTextW(L"Stop");

			SetBackgroundColor(g_blueColor);
		}
		else
		{
			// error
		}
	}
}


// 매 프레임마다 호출된다.
void CMotionOutputView::Update(const float deltaSeconds)
{
	const bool isSerialConnect = cController::Get()->GetSerialComm().IsConnect();
	if (isSerialConnect)
	{
		//Quaternion quat = cController::Get()->GetCubeFlight().GetRotation();
		m_incTime += deltaSeconds;
		m_incSerialTime += deltaSeconds;
		m_totalIncTime += deltaSeconds;

		//const int out_yaw = 256;
		// machinegun
// 		const int out_sway = 256;
// 		const int out_surge = 256;
		// dirt3
		const int out_sway = 10000;
		const int out_surge = 10000;
		const int out_switch = 0;
		const int out_yaw = (int)cMotionController::Get()->m_yaw;
		const int out_pitch = (int)cMotionController::Get()->m_pitch;
		const int out_roll = (int)cMotionController::Get()->m_roll;
		const int out_heave = (int)cMotionController::Get()->m_heave;

		if (m_incTime > 0.033f)
		{
			m_multiPlotWindows->SetString(m_totalIncTime, common::format("%d;%d;%d;%d", out_yaw, out_pitch, out_roll, out_heave).c_str(), 0);
			m_multiPlotWindows->DrawGraph(m_incTime, false);
			m_incTime = 0;
		}

		if (m_incSerialTime > 0.07f) // 15 frame, machine gun
		//if (m_incSerialTime > 0.033f) // 30 frame, dirt3
		{
			if (m_isStartSendMotion && !m_IsOnlyEmergency)
			{
				// machinegun version
// 				const string out = common::format(
// 					"A%3d%3d%3d%3d%3d%3d0Z", out_roll, out_pitch, out_yaw,
// 					out_sway, out_surge, out_heave, out_switch);
				const int spareSpeed = 100;
				string out = common::format(
					"A%5d%5d%5d%5d%5d%5d%3d%3d%3d%3d%3d%3d0Z", out_roll, out_pitch, out_yaw,
					out_sway, out_surge, out_heave,  
					spareSpeed, spareSpeed, spareSpeed, spareSpeed, spareSpeed, spareSpeed,
					out_switch);
				SpaceCharToZeroChar(out);

				AppendToLogAndScroll(&m_OutputLog, common::str2wstr(out+"\n").c_str(), RGB(200, 200, 200));

				// 시리얼 포트로 모션 시뮬레이터 장비에 모션 정보를 전송한다.
				cController::Get()->GetSerialComm().GetSerial().SendData(out.c_str(), out.size());

				m_multiPlotWindows->SetString(m_totalIncTime, common::format("%d;%d;%d;%d", out_yaw, out_pitch, out_roll, out_heave).c_str(), 1);
			}

			m_incSerialTime = 0;
		}
	}
}


// 시리얼통신 정보를 받아서 처리한다.
void CMotionOutputView::UpdateSerial(char *buffer)
{
	AppendToLogAndScroll(&m_Log, common::str2wstr(string(buffer) + "\n").c_str(), RGB(200, 200, 200));
}


void CMotionOutputView::OnBnClickedButtonStartSendMotion()
{
	if (m_isStartSendMotion)
	{
		m_isStartSendMotion = false;
		m_StartSendMotionButton.SetWindowTextW(L"Start Send Motion");
	}
	else
	{
		m_isStartSendMotion = true;
		m_StartSendMotionButton.SetWindowTextW(L"Stop!! ");
	}
}


void CMotionOutputView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePaneChildView::OnSize(nType, cx, cy);

	CRect rcWnd;
	GetWindowRect(&rcWnd);
	HandleAnchors(&rcWnd);

	if (m_multiPlotWindows && m_multiPlotWindows->GetSafeHwnd())
	{
		CRect pwr;
		GetDlgItem(IDC_STATIC_GRAPH)->GetWindowRect(pwr);
		ScreenToClient(pwr);
		m_multiPlotWindows->MoveWindow(pwr);
	}
}


void CMotionOutputView::OnBnClickedButtonStart()
{
	SendMotionControllSwitchMessage(BITCON_SER::START);
}
void CMotionOutputView::OnBnClickedButtonStop()
{
	SendMotionControllSwitchMessage(BITCON_SER::STOP);
}
void CMotionOutputView::OnBnClickedButtonOrigin()
{
	SendMotionControllSwitchMessage(BITCON_SER::ORIGIN);
}
void CMotionOutputView::OnBnClickedButtonEmergency()
{
	SendMotionControllSwitchMessage(BITCON_SER::EMERGENCY);
}
void CMotionOutputView::OnBnClickedButtonServoOn()
{
	SendMotionControllSwitchMessage(BITCON_SER::SERVOON);
}
void CMotionOutputView::OnBnClickedButtonServoOff()
{
	SendMotionControllSwitchMessage(BITCON_SER::SERVOOFF);
}


// 모션 컨트롤러 상태 메세지를 보낸다.
void CMotionOutputView::SendMotionControllSwitchMessage(const int state)
{
	if (!m_isStartSendMotion)
		return;

// 	float roll, pitch, yaw, heave;
// 	cMotionController::Get()->m_udpMod.GetFinal(yaw, pitch, roll, heave);

	// machinegun
// 	const int out_pitch = 256;// (int)(pitch * 255) + 255;
// 	const int out_roll = 256;
// 	const int out_yaw = 256;
// 	const int out_sway = 256;
// 	const int out_surge = 256;
// 	const int out_heave = 512;

	const int out_pitch = 10000;// (int)(pitch * 255) + 255;
	const int out_roll = 10000;
	const int out_yaw = 10000;
	const int out_sway = 10000;
	const int out_surge = 10000;
	const int out_heave = 10000;
	const int spareSpeed = 100;
	const int out_switch = state;

	// machinegun
// 	const string out = common::format(
// 		"A%3d%3d%3d%3d%3d%3d%dZ", out_roll, out_pitch, out_yaw,
// 		out_sway, out_surge, out_heave, out_switch);

	string out = common::format(
		"A%5d%5d%5d%5d%5d%5d%3d%3d%3d%3d%3d%3d%1dZ", out_roll, out_pitch, out_yaw,
		out_sway, out_surge, out_heave,
		spareSpeed, spareSpeed, spareSpeed, spareSpeed, spareSpeed, spareSpeed,
		out_switch);
	SpaceCharToZeroChar(out);

	AppendToLogAndScroll(&m_OutputLog, common::str2wstr(out + "\n").c_str(), RGB(200, 200, 200));
	
	for (int i = 0; i < 5; ++i)
	{
		cController::Get()->GetSerialComm().GetSerial().SendData(out.c_str(), out.size());
		Sleep(50);
	}

	m_incSerialTime = 0;
}


// Send Only Emergency Message
void CMotionOutputView::OnBnClickedCheck1()
{
	UpdateData();
}


void CMotionOutputView::OnDestroy()
{
	UpdateConfig(false);

	CDockablePaneChildView::OnDestroy();	
}


// 모션 입력을 시리얼포트를 통해 출력으로 보낸다.
void CMotionOutputView::Start()
{
	m_multiPlotWindows->ProcessPlotCommand(g_outputViewPlotCommand, 2);
	m_multiPlotWindows->SetFixedWidthMode(true);

	const int portNumber = m_COMPortComboBox.GetPortNum();
	CString baudRate;
	m_BaudRateComboBox.GetWindowTextW(baudRate);

	cController::Get()->CloseSerial();
	if (cController::Get()->ConnectSerial(portNumber, _wtoi(baudRate)))
	{
		m_ConnectButton.SetWindowTextW(L"Stop");

		m_IsOnlyEmergency = FALSE;
		m_isStartSendMotion = true;
		m_StartSendMotionButton.SetWindowTextW(L"Stop!! ");

		SetBackgroundColor(g_blueColor);
	}
}


// 출력을 내보내는 것을 멈춘다.
void CMotionOutputView::Stop()
{
	cController::Get()->CloseSerial();
	m_ConnectButton.SetWindowTextW(L"Start");

	m_IsOnlyEmergency = TRUE;
	m_isStartSendMotion = false;
	m_StartSendMotionButton.SetWindowTextW(L"Start Send Motion");

	SetBackgroundColor(g_grayColor);
}


// 비상 정지
// 비상 정지 메세지를 보낸 후, 장비와 통신을 끊는다.
// Stop 이나, ServoOff 는 하지 않는다.
void CMotionOutputView::EmergencyStop()
{
	SendMotionControllSwitchMessage(BITCON_SER::EMERGENCY);

	cController::Get()->CloseSerial();
	m_ConnectButton.SetWindowTextW(L"Start");
	SetBackgroundColor(g_grayColor);
}
