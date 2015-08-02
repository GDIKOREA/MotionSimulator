// OutputView.cpp : implementation file
//

#include "stdafx.h"
#include "UDPAnalyzer.h"
#include "OutputView.h"
#include "afxdialogex.h"


// COutputView dialog
COutputView::COutputView(CWnd* pParent /*=NULL*/)
	: CDockablePaneChildView(COutputView::IDD, pParent)
	, m_UDPPort(8888)
	, m_RollCommand(_T(""))
	, m_PitchCommand(_T(""))
	, m_YawCommand(_T(""))
	, m_multiPlotWindows(NULL)
	, m_incTime(0)
	, m_isStart(false)
	, m_SendType(2)
	, m_SendFormat(_T(""))
	, m_incSerialTime(0)
	, m_incUDPTime(0)
{

}

COutputView::~COutputView()
{
}

void COutputView::DoDataExchange(CDataExchange* pDX)
{
	CDockablePaneChildView::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_PORT, m_UDPPort);
	DDX_Control(pDX, IDC_IPADDRESS_IP, m_UDPIP);
	DDX_Control(pDX, IDC_COMBO_COM, m_ComPort);
	DDX_Control(pDX, IDC_COMBO_BAUDRATE, m_BaudRateCombobox);
	DDX_Text(pDX, IDC_EDIT_ROLL, m_RollCommand);
	DDX_Text(pDX, IDC_EDIT_PITCH, m_PitchCommand);
	DDX_Text(pDX, IDC_EDIT_YAW, m_YawCommand);
	DDX_Control(pDX, IDC_EDIT_PLOT_COMMAND, m_PlotCommand);
	DDX_Radio(pDX, IDC_RADIO_SERIAL, m_SendType);
	DDX_Text(pDX, IDC_EDIT_SENDCOMMAND, m_SendFormat);
	DDX_Control(pDX, IDC_STATIC_SENDDATA, m_SendString);
	DDX_Control(pDX, IDC_BUTTON_CONNECT, m_ConnectButton);
}


BEGIN_ANCHOR_MAP(COutputView)
	ANCHOR_MAP_ENTRY(IDC_STATIC_PLOT, ANF_LEFT | ANF_RIGHT | ANF_TOP | ANF_BOTTOM)
	ANCHOR_MAP_ENTRY(IDC_EDIT_PLOT_COMMAND, ANF_LEFT | ANF_RIGHT | ANF_TOP)
END_ANCHOR_MAP()

BEGIN_MESSAGE_MAP(COutputView, CDockablePaneChildView)
	ON_BN_CLICKED(IDOK, &COutputView::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &COutputView::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_RADIO_SERIAL, &COutputView::OnBnClickedRadioSerial)
	ON_BN_CLICKED(IDC_RADIO_UDP, &COutputView::OnBnClickedRadioUdp)
	ON_BN_CLICKED(IDC_BUTTON_CONNECT, &COutputView::OnBnClickedButtonConnect)
	ON_BN_CLICKED(IDC_BUTTON_3DUPDATE, &COutputView::OnBnClickedButton3dupdate)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_PLOT_UPDATE, &COutputView::OnBnClickedButtonPlotUpdate)
	ON_BN_CLICKED(IDC_BUTTON_UPDATE_SENDFORMAT, &COutputView::OnBnClickedButtonUpdateSendformat)
	ON_BN_CLICKED(IDC_RADIO_NONE, &COutputView::OnBnClickedRadioNone)
END_MESSAGE_MAP()


BOOL COutputView::OnInitDialog()
{
	CDockablePaneChildView::OnInitDialog();

	InitAnchors();

	m_ComPort.InitList();

	const int baudRate[] = { 9600, 14400, 19200, 38400, 56000, 57600, 115200 };
	for (int i = 0; i < ARRAYSIZE(baudRate); ++i)
	{
		TCHAR brateName[32];
		wsprintf(brateName, L"%d", baudRate[i]);
		m_BaudRateCombobox.InsertString(m_BaudRateCombobox.GetCount(), brateName);
	}
	m_BaudRateCombobox.SetCurSel(0);


	m_UDPIP.SetAddress(127, 0, 0, 1);

	m_SendFormat = L"$yaw;$pitch;$roll;";
	m_RollCommand = L"$8 - 1.55";
	m_PitchCommand = L"$7";
	m_YawCommand = L"$6";

	// Plot창 생성.
	CRect rect;
	GetClientRect(rect);

	m_multiPlotWindows = new CMultiPlotWindow();
	BOOL result = m_multiPlotWindows->Create(NULL, NULL, WS_VISIBLE | WS_CHILD,
		CRect(0, 0, rect.Width(), 400), this, AFX_IDW_PANE_FIRST);

	m_multiPlotWindows->SetScrollSizes(MM_TEXT, CSize(rect.Width() - 30, 100));
	m_multiPlotWindows->ShowWindow(SW_SHOW);

	
	CString command =
		L"plot1 = 0, 0, 0, 0, 0\r\n"
		L"string1 = %f;\r\n"
		L"name1 = Yaw\r\n"
		L"plot2 = 0, 0, 0, 0, 0\r\n"
		L"string2 = %*f; %f;\r\n"
		L"name2 = Pitch\r\n"
		L"plot3 = 0, 0, 0, 0, 0\r\n"
		L"string3 = %*f; %*f; %f; \r\n"
		L"name3 = Roll\r\n"
		L"plot4 = 0, 0, 0, 0, 0\r\n"
		L"string4 = %*f; %*f; %*f; %f;\r\n"
		L"name4 = Heave\r\n";
	m_PlotCommand.SetWindowTextW(command);
	
	m_rollParser.ParseStr(wstr2str((LPCTSTR)m_RollCommand));
	m_pitchParser.ParseStr(wstr2str((LPCTSTR)m_PitchCommand));
	m_yawParser.ParseStr(wstr2str((LPCTSTR)m_YawCommand));

	m_sendFormatParser.ParseStr(wstr2str((LPCTSTR)m_SendFormat));

	UpdateData(FALSE);
	return TRUE;
}


// COutputView message handlers
void COutputView::OnBnClickedOk()
{
}

void COutputView::OnBnClickedCancel()
{
}


void COutputView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePaneChildView::OnSize(nType, cx, cy);

	CRect rcWnd;
	GetWindowRect(&rcWnd);
	HandleAnchors(&rcWnd);

	if (m_multiPlotWindows && m_multiPlotWindows->GetSafeHwnd())
	{
		CRect pwr;
		GetDlgItem(IDC_STATIC_PLOT)->GetWindowRect(pwr);
		ScreenToClient(pwr);
		m_multiPlotWindows->MoveWindow(pwr);
	}
}


void COutputView::Update(const float deltaSeconds)
{
	RET(!m_isStart);

	const float elapseT = 0.033f;
	m_incTime += deltaSeconds;

	if (m_incTime > elapseT)
	{
		const string sendStr = m_sendFormatParser.Execute();
		m_SendString.SetWindowTextW(str2wstr(sendStr).c_str());

		// 3D View
		const float roll = m_interpreter.Excute(m_rollParser.m_stmt);
		const float pitch = m_interpreter.Excute(m_pitchParser.m_stmt);
		const float yaw = m_interpreter.Excute(m_yawParser.m_stmt);
		cController::Get()->GetCubeFlight().SetEulerAngle(roll, yaw, pitch);
		//

		m_multiPlotWindows->SetString(sendStr.c_str());
		m_multiPlotWindows->DrawGraph(m_incTime);

		m_incTime = 0;
	}

	// 시리얼 통신을 통해, 데이타를 전송한다.
	if (0 == m_SendType) // Serial
	{
		m_incSerialTime += deltaSeconds;

		const bool isSerialConnect = cController::Get()->GetSerialComm().IsConnect();
		if (isSerialConnect)
		{
			if (m_incSerialTime > elapseT)
			{
				const string sendStr = m_sendFormatParser.Execute();

				// 시리얼 포트로 모션 시뮬레이터 장비에 모션 정보를 전송한다.
				cController::Get()->GetSerialComm().GetSerial().SendData(sendStr.c_str(), sendStr.size());

				m_incSerialTime = 0;
			}
		}
	}
	else if (1 == m_SendType) // UDP
	{
		m_incUDPTime += deltaSeconds;

		if (m_incUDPTime > elapseT)
		{
			const string sendStr = m_sendFormatParser.Execute();
			m_udpSendClient.SendData(sendStr.c_str(), sendStr.size());

			m_incUDPTime = 0;
		}
	}

}


void COutputView::OnBnClickedRadioSerial()
{
	UpdateData();	
}


void COutputView::OnBnClickedRadioUdp()
{
	UpdateData();	
}


void COutputView::OnBnClickedRadioNone()
{
	UpdateData();
}


void COutputView::OnBnClickedButtonConnect()
{
	UpdateData();

	if (0 == m_SendType) // Serial
	{
		const bool isConnect = cController::Get()->GetSerialComm().IsConnect();

		if (isConnect)
		{
			cController::Get()->CloseSerial();
			m_ConnectButton.SetWindowTextW(L"Start");
			m_isStart = false;
		}
		else
		{
			const int portNumber = m_ComPort.GetPortNum();
			CString baudRate;
			m_BaudRateCombobox.GetWindowTextW(baudRate);

			if (cController::Get()->ConnectSerial(portNumber, _wtoi(baudRate)))
			{
				m_ConnectButton.SetWindowTextW(L"Stop");
				m_isStart = true;
			}
		}
	}
	else if (1 == m_SendType) // UDP
	{
		if (m_isStart)
		{
			m_isStart = false;
			m_udpSendClient.Close();
			m_ConnectButton.SetWindowTextW(L"Start");
		}
		else
		{
			const string ip = GetSendIP();

			if (!m_udpSendClient.Init(ip, m_UDPPort))
			{
 				::AfxMessageBox(L"클라이언트 접속 에러 !!");
				return;
			}

			m_isStart = true;
			m_ConnectButton.SetWindowTextW(L"Stop");
		}
	}
	else
	{
		// Nothing~
	}

}


void COutputView::OnBnClickedButton3dupdate()
{
	UpdateData();

	m_rollParser.ParseStr(wstr2str((LPCTSTR)m_RollCommand));
	m_pitchParser.ParseStr(wstr2str((LPCTSTR)m_PitchCommand));
	m_yawParser.ParseStr(wstr2str((LPCTSTR)m_YawCommand));
}


// 송신 클라이언트가 접속할 IP 주소를 리턴한다.
string COutputView::GetSendIP()
{
	DWORD address;
	m_UDPIP.GetAddress(address);
	std::stringstream ss;
	ss << ((address & 0xff000000) >> 24) << "."
		<< ((address & 0x00ff0000) >> 16) << "."
		<< ((address & 0x0000ff00) >> 8) << "."
		<< (address & 0x000000ff);
	const string ip = ss.str();
	return ip;
}


// 그래프  명령어 업데이트
void COutputView::OnBnClickedButtonPlotUpdate()
{
	CString command;
	m_PlotCommand.GetWindowTextW(command);
	m_multiPlotWindows->ProcessPlotCommand(command);
	m_multiPlotWindows->SetFixedWidthMode(true);

	m_isStart = true;
}


// 전송될 스트링 포맷 업데이트
void COutputView::OnBnClickedButtonUpdateSendformat()
{
	UpdateData();
	m_sendFormatParser.ParseStr(wstr2str((LPCTSTR)m_SendFormat));
}
