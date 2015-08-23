// MotionInputView.cpp : implementation file
//

#include "stdafx.h"
#include "MotionMonitor.h"
#include "UDPInputView.h"
#include "afxdialogex.h"


// UDPInputView dialog
CUDPInputView::CUDPInputView(CWnd* pParent /*=NULL*/)
	: CDockablePaneChildView(CUDPInputView::IDD, pParent)
	, m_multiPlotWindows(NULL)
	, m_incTime(0)
	, m_updateIncTime(0)
	, m_ServerPort(0)
	, m_PacketString(_T(""))
	, m_isPause(false)
	, m_PacketRecvCount(0)
	, m_PlotInputString(_T(""))
	, m_state(STOP)
{

}

CUDPInputView::~CUDPInputView()
{
}

void CUDPInputView::DoDataExchange(CDataExchange* pDX)
{
	CDockablePaneChildView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_UPDATE, m_UpdateButton);
	DDX_Control(pDX, IDC_RICHEDIT2_COMMAND, m_EditCommand);
	DDX_Control(pDX, IDC_BUTTON_UDP_SERVER_BIND, m_ServerBindButton);
	DDX_Text(pDX, IDC_EDIT_PORT, m_ServerPort);
	DDX_Text(pDX, IDC_STATIC_PACKET, m_PacketString);
	DDX_Control(pDX, IDC_EDIT_COMMAND, m_PlotCommandEditor);
	DDX_Control(pDX, IDC_CHECK_FIXEDMODE, m_FixedModeButton);
	DDX_Text(pDX, IDC_STATIC_PACKET_COUNT, m_PacketRecvCount);
	DDX_Text(pDX, IDC_STATIC_PLOTINPUT, m_PlotInputString);
}


BEGIN_ANCHOR_MAP(CUDPInputView)
	ANCHOR_MAP_ENTRY(IDC_STATIC_GRAPH, ANF_LEFT | ANF_TOP | ANF_RIGHT | ANF_BOTTOM)
	ANCHOR_MAP_ENTRY(IDC_RICHEDIT2_COMMAND, ANF_LEFT | ANF_TOP | ANF_RIGHT)
	ANCHOR_MAP_ENTRY(IDC_EDIT_COMMAND, ANF_LEFT | ANF_RIGHT | ANF_BOTTOM)
	ANCHOR_MAP_ENTRY(IDC_STATIC1, ANF_LEFT | ANF_BOTTOM)
	ANCHOR_MAP_ENTRY(IDC_BUTTON_PLOTUPDATE, ANF_RIGHT | ANF_BOTTOM)
	ANCHOR_MAP_ENTRY(IDC_BUTTON_HELP, ANF_RIGHT | ANF_BOTTOM)
	ANCHOR_MAP_ENTRY(IDC_BUTTON_PLOTPAUSE, ANF_RIGHT | ANF_BOTTOM)
	ANCHOR_MAP_ENTRY(IDC_CHECK_FIXEDMODE, ANF_LEFT | ANF_BOTTOM)
END_ANCHOR_MAP()


BEGIN_MESSAGE_MAP(CUDPInputView, CDockablePaneChildView)
	ON_BN_CLICKED(IDOK, &CUDPInputView::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CUDPInputView::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_UPDATE, &CUDPInputView::OnBnClickedButtonUpdate)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_UDP_SERVER_BIND, &CUDPInputView::OnBnClickedButtonUdpServerBind)
	ON_BN_CLICKED(IDC_BUTTON_PLOTUPDATE, &CUDPInputView::OnBnClickedButtonPlotupdate)
	ON_BN_CLICKED(IDC_BUTTON_HELP, &CUDPInputView::OnBnClickedButtonHelp)
	ON_BN_CLICKED(IDC_BUTTON_PLOTPAUSE, &CUDPInputView::OnBnClickedButtonPlotpause)
	ON_BN_CLICKED(IDC_CHECK_FIXEDMODE, &CUDPInputView::OnBnClickedCheckFixedmode)
END_MESSAGE_MAP()


// UDPInputView message handlers


void CUDPInputView::OnBnClickedOk()
{
	//CDockablePaneChildView::OnOK();
}


void CUDPInputView::OnBnClickedCancel()
{
	//CDockablePaneChildView::OnCancel();
}


BOOL CUDPInputView::OnInitDialog()
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

	UpdateConfig();

	m_FixedModeButton.SetCheck(1);

	UpdateData(FALSE);

	return TRUE;	
}


// 환경설정파일에 저장된 정보를 읽어온다.
void CUDPInputView::UpdateConfig(bool IsSaveAndValidate) //IsSaveAndValidate=true
{
	if (IsSaveAndValidate)
	{
const CString command =
L"# udp input\n\
\n\
yaw_proportion = 1\n\
pitch_proportion = 1\n\
roll_proportion = 1\n\
heave_proportion = 1\n\
\n\
spline_enable = 0\n\
\n\
yaw_c1 = 0\n\
yaw_c2 = 0\n\
yaw_c3 = 0\n\
\n\
pitch_c1 = 0\n\
pitch_c2 = 255\n\
pitch_c3 = 255\n\
\n\
roll_c1 = 0\n\
roll_c2 = 255\n\
roll_c3 = 255\n\
\n\
heave_c1 = 0\n\
heave_c2 = 0\n\
heave_c3 = 0\n\
\n\
spline_plot_sampling_rate = 10\n\
spline_interpolation_rate = 10\n\
\n\
";

		CString cmdStr;
		if (!cMotionController::Get()->m_config.m_udpModCommand.empty())
		{
			cmdStr = str2wstr(cMotionController::Get()->m_config.m_udpModCommand).c_str();
		}
		else
		{
			cmdStr = command;
		}
		m_EditCommand.SetWindowTextW(cmdStr);


		CString strPlotComandEditor;
		if (!cMotionController::Get()->m_config.m_udpPlotCommand.empty())
		{
			strPlotComandEditor = str2wstr(cMotionController::Get()->m_config.m_udpPlotCommand).c_str();
		}
		else
		{
strPlotComandEditor = L"plot1 = 0, 0, 0, 0, 0\r\n\
string1 = %f;\r\n\
name1 = Yaw\r\n\
plot2 = 0, 0, 0, 0, 0\r\n\
string2 = %*f; %f;\r\n\
name2 = Pitch\r\n\
plot3 = 0, 0, 0, 0, 0\r\n\
string3 = %*f; %*f; %f;\r\n\
name3 = Roll\r\n\
plot4 = 0, 0, 0, 0, 0\r\n\
string4 = %*f; %*f; %*f; %f;\r\n\
name4 = Heave\r\n";
		}

		m_PlotCommandEditor.SetWindowTextW(strPlotComandEditor);

		m_ServerPort = cMotionController::Get()->m_config.m_udpPort;

		UpdateData(FALSE);
	}
	else
	{
		UpdateData();

		// 환경파일 저장
		CString modCommand;
		m_EditCommand.GetWindowTextW(modCommand);
		CString plotCommand;
		m_PlotCommandEditor.GetWindowText(plotCommand);
		cMotionController::Get()->m_config.m_udpModCommand = wstr2str((LPCTSTR)modCommand);
		cMotionController::Get()->m_config.m_udpPlotCommand = wstr2str((LPCTSTR)plotCommand);
		cMotionController::Get()->m_config.m_udpPort = m_ServerPort;
	}	
}


void CUDPInputView::OnSize(UINT nType, int cx, int cy)
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


void CUDPInputView::OnBnClickedButtonUpdate()
{
	CString str;
	m_PlotCommandEditor.GetWindowText(str);
	m_multiPlotWindows->ProcessPlotCommand(str, 3);

	CString command;
	m_EditCommand.GetWindowTextW(command);
	cMotionController::Get()->m_udpMod.ParseStr(common::wstr2str((LPCTSTR)command).c_str());
}


// Update
void CUDPInputView::Update(const float deltaSeconds)
{
	RET(STOP==m_state);

	if (!m_multiPlotWindows)
		return;

	m_incTime += deltaSeconds;
	m_updateIncTime += deltaSeconds;

	if (m_updateIncTime > 0.020f)
	{
		float origRoll, origPitch, origYaw, origHeave;
		cMotionController::Get()->m_udpMod.GetOriginal(origYaw, origPitch, origRoll, origHeave);
		m_multiPlotWindows->SetString(common::format("%f;%f;%f;%f", origYaw, origPitch, origRoll, origHeave).c_str(), 1);

		float roll, pitch, yaw, heave;
		cMotionController::Get()->m_udpMod.GetFinal(yaw, pitch, roll, heave);
		m_multiPlotWindows->SetString(common::format("%f;%f;%f;%f;", yaw, pitch, roll, heave).c_str(), 2);

		if (!m_isPause)
			m_multiPlotWindows->DrawGraph(m_updateIncTime);

		m_updateIncTime = 0.f;
	}
}


void CUDPInputView::UpdateUDP(const char *buffer, const int bufferLen)
{
	RET(STOP == m_state);

	m_PacketString = common::str2wstr(buffer).c_str();

	if (bufferLen < 10)
	{
		++m_PacketRecvCount;
		UpdateData(FALSE);
		return;
	}

	std::stringstream ss;
	vector<float> values(4);

	if (GAME_TYPE::DIRT3 == g_gameType) // Dirt3 Motion
	{
		// 패킷의 특정 값만 float으로 변환해서 가져온다.
		// 프로토콜에 대한 문서는 https://github.com/GDIKOREA/MotionSimulator/wiki/Dirt3-Motion-Data-Capture 를 참조하자.
		// packet[16~19] : yaw, radian, float
		// packet[20~23] : pitch, radian, float
		// packet[24~27] : roll, radian, float
		// packet[28~31] : heave, float(확실치 않음)

		const int indices[] = { 16, 20, 24, 28 };
		const int size = sizeof(indices) / sizeof(int);
		for (int i = 0; i < size; ++i)
		{
			float v = *(float*)&buffer[indices[i]];
			values[i] = v;
			ss << v << ";";
		}

		values[2] -= 1.55f;

		const float roll = values[2];
		const float pitch = values[1];
		const float yaw = -values[0];
		const float heave = values[3];

		cController::Get()->GetCubeFlight().SetEulerAngle(roll, yaw, pitch);

		m_PlotInputString = common::str2wstr(ss.str()).c_str();

		// 그래프 출력
		if (m_multiPlotWindows)
			m_multiPlotWindows->SetString(ss.str().c_str());

		cMotionController::Get()->m_udpMod.Update(m_incTime, yaw, pitch, roll, heave);
		m_incTime = 0;
	}
	else
	{ // MachineGun
		if (bufferLen < sizeof(sMotionPacket))
			return;

		sMotionPacket *packet = (sMotionPacket*)buffer;
		values[0] = packet->yaw;
		values[1] = packet->pitch;
		values[2] = packet->roll;
		values[3] = 0;

		// 3D Model에 적용.
		float roll = -values[2];
		float pitch = -values[1];
		const float yaw = 0;
		cController::Get()->GetCubeFlight().SetEulerAngle(roll, yaw, pitch);
		Vector3 euler = cController::Get()->GetCubeFlight().GetRotation().Euler();
		roll = euler.x;
		pitch = euler.z;

		ss << 0 << ";";
		ss << pitch << ";";
		ss << roll << ";";
		ss << 0 << ";";
		ss << packet->gamestate << ";";
		ss << packet->mission << ";";

		m_PlotInputString = common::str2wstr(ss.str()).c_str();

		// 그래프 출력
		if (m_multiPlotWindows)
			m_multiPlotWindows->SetString(ss.str().c_str());

		cMotionController::Get()->m_udpMod.Update(m_incTime, 0, pitch, roll, 0);
		m_incTime = 0;
	}


	++m_PacketRecvCount;
	UpdateData(FALSE);
}


void CUDPInputView::OnBnClickedButtonUdpServerBind()
{
	UpdateData();

	if (cController::Get()->GetUDPComm().IsConnect())
	{
// 		cController::Get()->GetUDPComm().Close();
// 		m_ServerBindButton.SetWindowTextW(L"Server Start");
// 
// 		SetBackgroundColor(g_grayColor);
		Stop();
	}
	else
	{
		Start();

// 		if (cController::Get()->GetUDPComm().InitServer(m_ServerPort))
// 		{
// 			m_ServerBindButton.SetWindowTextW(L"Server Stop");
// 
// 			CString str;
// 			m_PlotCommandEditor.GetWindowText(str);
// 			m_multiPlotWindows->ProcessPlotCommand(str, 3);
// 
// 			SetBackgroundColor(g_blueColor);
// 		}
// 		else
// 		{
// 			//m_LogList.InsertString(m_LogList.GetCount(), L"접속 실패");
// 		}
	}	
}


void CUDPInputView::OnBnClickedButtonPlotupdate()
{
	CString str;
	m_PlotCommandEditor.GetWindowText(str);
	m_multiPlotWindows->ProcessPlotCommand(str, 3);
}


void CUDPInputView::OnBnClickedButtonHelp()
{
 	AfxMessageBox(
 L"\n\
plot = x-range, y-range, x-visible-range, y-visible-range, option \n\
	- x-range: 0 - auto \n\
	- y-range : 0 - auto \n\
	- x-visible - range : 0 - auto \n\
	- y-visible - range : 0 - auto \n\
	- option : \n\
	 	- x, y scroll \n\
	 	- line width\n\
\n\
string = sscanf 의 인자로 들어갈 파싱 포맷\n\
\n\
예제)\n\
	- Serial Data\n\
		- Start, 1234, 1.11, 2.34, 3.33, 4.44\n\
\n\
	- Plot Command Script\n\
		plot1 = 0, 0, 0, 0, 0\n\
		string1 = %*s %f, %f, %*f, %*f, %*f\n\
\n\
		plot2 = 0, 0, 0, 0, 0\n\
		string2 = %*s %f, %*f, %f, %*f, %*f\n\
");
}


void CUDPInputView::OnBnClickedButtonPlotpause()
{
	m_isPause = !m_isPause;
}


void CUDPInputView::OnBnClickedCheckFixedmode()
{
	if (m_multiPlotWindows)
		m_multiPlotWindows->SetFixedWidthMode(m_FixedModeButton.GetCheck() > 0);
}


void CUDPInputView::OnDestroy()
{
	UpdateConfig(false);

	CDockablePaneChildView::OnDestroy();
}


// UDP 접속을 시작하고, 정보를 받는다.
void CUDPInputView::Start()
{
	UpdateData();

	m_state = START;

	cController::Get()->GetUDPComm().Close();

	if (cController::Get()->GetUDPComm().InitServer(m_ServerPort))
	{
		m_ServerBindButton.SetWindowTextW(L"Server Stop");

		CString str;
		m_PlotCommandEditor.GetWindowText(str);
		m_multiPlotWindows->ProcessPlotCommand(str, 3);
	}
	else
	{
		//m_LogList.InsertString(m_LogList.GetCount(), L"접속 실패");
	}

	CString str;
	m_PlotCommandEditor.GetWindowText(str);
	m_multiPlotWindows->ProcessPlotCommand(str, 3);

	CString command;
	m_EditCommand.GetWindowTextW(command);
	cMotionController::Get()->m_udpMod.ParseStr(common::wstr2str((LPCTSTR)command).c_str());

	SetBackgroundColor(g_blueColor);
}


// UDP 접속을 끊고, 화면 업데이트를 종료한다.
void CUDPInputView::Stop()
{
	m_state = STOP;

	cController::Get()->GetUDPComm().Close();
	m_ServerBindButton.SetWindowTextW(L"Server Start");

	SetBackgroundColor(g_grayColor);
}
