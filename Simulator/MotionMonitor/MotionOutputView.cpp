// MotionOutputView.cpp : implementation file
//

#include "stdafx.h"
#include "MotionMonitor.h"
#include "MotionOutputView.h"
#include "afxdialogex.h"
#include "MotionController.h"


// CMotionOutputView dialog

CMotionOutputView::CMotionOutputView(CWnd* pParent /*=NULL*/)
: CDockablePaneChildView(CMotionOutputView::IDD, pParent)
	, m_isStartSendMotion(false)
	, m_incTime(0)
	, m_incSerialTime(0)
	, m_incNoiseTime(0)
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



	m_COMPortComboBox.InitList();

	const int baudRate[] = { 9600, 14400, 19200, 38400, 56000, 57600, 115200 };
	for (int i = 0; i < ARRAYSIZE(baudRate); ++i)
	{
		TCHAR brateName[32];
		wsprintf(brateName, L"%d", baudRate[i]);
		m_BaudRateComboBox.InsertString(m_BaudRateComboBox.GetCount(), brateName);
	}
	m_BaudRateComboBox.SetCurSel(0);

	m_Log.SetBackgroundColor(FALSE, RGB(0, 0, 0));
	m_OutputLog.SetBackgroundColor(FALSE, RGB(0, 0, 0));

	return TRUE;
}


// 시리얼 포트와 연결한다.
void CMotionOutputView::OnBnClickedButtonConnect()
{
	const bool isConnect = cController::Get()->GetSerialComm().IsConnect();

	if (isConnect)
	{
		cController::Get()->CloseSerial();
		m_ConnectButton.SetWindowTextW(L"Start");
	}
	else
	{
		{
			CString plotCommand = L"plot1 = 0, 0, 0, 0, 0\r\n\
string1 = %f;\r\n\
plot2 = 0, 0, 0, 0, 0\r\n\
string2 = %*f; %f;\r\n\
plot3 = 0, 0, 0, 0, 0\r\n\
string3 = %*f; %*f; %f;\r\n";
			m_multiPlotWindows->ProcessPlotCommand(plotCommand, 2);
			m_multiPlotWindows->SetFixedWidthMode(true);

			vector<string> names;
			names.push_back("Yaw");
			names.push_back("Pitch");
			names.push_back("Roll");
			m_multiPlotWindows->SetPlotName(names);
		}


		const int portNumber = m_COMPortComboBox.GetPortNum();
		CString baudRate;
		m_BaudRateComboBox.GetWindowTextW(baudRate);

		if (cController::Get()->ConnectSerial(portNumber, _wtoi(baudRate)))
		{
			m_ConnectButton.SetWindowTextW(L"Stop");
		}
		else
		{

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
		m_incNoiseTime += deltaSeconds;

		if (m_incTime > 0.1f)
		{
			float roll, pitch, yaw;
			cMotionController::Get()->GetMotion(yaw, pitch, roll);

			float roll_scale, pitch_scale, yaw_scale;
			cMotionController::Get()->GetScale(yaw_scale, pitch_scale, roll_scale);

			float roll_scale2, pitch_scale2, yaw_scale2;
			cMotionController::Get()->GetScale2(yaw_scale2, pitch_scale2, roll_scale2);

//			CString str;
// 			str.Format(L"roll=%f, pitch=%f, yaw=%f\n", roll, pitch, yaw);
// 			AppendToLogAndScroll(&m_Log, str, RGB(200, 200, 200));
			//m_multiPlotWindows->SetString(common::format("%f;%f;%f;", yaw, pitch, roll).c_str(), 0);

			const int MAX_NOISE = 20;
			const float ct1 = 20.f;
			const float st1 = 20.f;
			const float amp1 = MAX_NOISE;
			const float amp2 = MAX_NOISE;
			const float noise1 = cos(m_incNoiseTime * ct1) * amp1;
			const float noise2 = sin(m_incNoiseTime * st1) * amp2;

			int out_pitch = 256;
			int out_roll = 256;
			if (1)
			{ // Dirt3
 				out_pitch = (int)(( (pitch*pitch*pitch_scale2 + pitch*pitch_scale) * 255.f) + 255);
 				out_roll = (int)( ( (roll*roll*roll_scale2 + roll*roll_scale) *  255.f) + 255);
			}
			else
			{
				out_pitch = (int)RAD2ANGLE(pitch) + 360;
				out_roll = (int)RAD2ANGLE(roll) + 360;
				out_pitch = out_pitch % 360;
				out_roll = out_roll % 360;
			}

			int out_yaw = 256;
			const int out_sway = 256;
			const int out_surge = 256;
			const int out_heave = 512;
			const int out_switch = 0;

			if (out_pitch > (512 - MAX_NOISE))
				out_pitch = (512 - MAX_NOISE);
			if (out_pitch < MAX_NOISE)
				out_pitch = MAX_NOISE;

			if (out_roll > (512 - MAX_NOISE))
				out_roll = (512 - MAX_NOISE);
			if (out_roll < MAX_NOISE)
				out_roll = MAX_NOISE;

			if (out_yaw > (512 - MAX_NOISE))
				out_yaw = (512 - MAX_NOISE);
			if (out_yaw < MAX_NOISE)
				out_yaw = MAX_NOISE;


			out_pitch += (int)noise1;
			out_roll += (int)noise2;


			if (out_pitch > 512)
				out_pitch = 512;
			if (out_pitch < 0)
				out_pitch = 0;

			if (out_roll > 512)
				out_roll = 512;
			if (out_roll < 0)
				out_roll = 0;

			if (out_yaw > 512)
				out_yaw = 512;
			if (out_yaw < 0)
				out_yaw = 0;



			m_multiPlotWindows->SetString(common::format("%d;%d;%d;", out_yaw, out_pitch, out_roll).c_str(), 0);

			m_multiPlotWindows->DrawGraph(m_incTime);

			if (m_isStartSendMotion && !m_IsOnlyEmergency)
			{
				const string out = common::format(
					"A%3d%3d%3d%3d%3d%3d0Z", out_roll, out_pitch, out_yaw,
					out_sway, out_surge, out_heave, out_switch);

				AppendToLogAndScroll(&m_OutputLog, common::str2wstr(out+"\n").c_str(), RGB(200, 200, 200));
				cController::Get()->GetSerialComm().GetSerial().SendData(out.c_str(), out.size());
			}

			m_incTime = 0;
		}
	}
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
	SendMotionControllSwitchMessage(1);
}


void CMotionOutputView::OnBnClickedButtonStop()
{
	SendMotionControllSwitchMessage(2);
}


void CMotionOutputView::OnBnClickedButtonOrigin()
{
	SendMotionControllSwitchMessage(5);
}


void CMotionOutputView::OnBnClickedButtonEmergency()
{
	SendMotionControllSwitchMessage(6);
}


void CMotionOutputView::OnBnClickedButtonServoOn()
{
	SendMotionControllSwitchMessage(7);
}


void CMotionOutputView::OnBnClickedButtonServoOff()
{
	SendMotionControllSwitchMessage(8);
}


// 모션 컨트롤러 상태 메세지를 보낸다.
void CMotionOutputView::SendMotionControllSwitchMessage(const int state)
{
	if (!m_isStartSendMotion)
		return;

	float roll, pitch, yaw;
	cMotionController::Get()->GetMotion(yaw, pitch, roll);

	const int out_pitch = 256;// (int)(pitch * 255) + 255;
	const int out_roll = 256;
	const int out_yaw = 256;
	const int out_sway = 256;
	const int out_surge = 256;
	const int out_heave = 512;
	const int out_switch = state;

	const string out = common::format(
		"A%3d%3d%3d%3d%3d%3d%dZ", out_roll, out_pitch, out_yaw,
		out_sway, out_surge, out_heave, out_switch);

	AppendToLogAndScroll(&m_OutputLog, common::str2wstr(out + "\n").c_str(), RGB(200, 200, 200));
	
	cController::Get()->GetSerialComm().GetSerial().SendData(out.c_str(), out.size());

	m_incTime = 0;
}


void CMotionOutputView::OnBnClickedCheck1()
{
	UpdateData();	
}
