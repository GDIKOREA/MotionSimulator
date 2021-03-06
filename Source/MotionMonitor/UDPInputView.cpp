// MotionInputView.cpp : implementation file
//

#include "stdafx.h"
#include "MotionMonitor.h"
#include "UDPInputView.h"
#include "afxdialogex.h"
#include "EditDlg.h"
#include "PlotInputDlg.h"



// UDPInputView dialog
CUDPInputView::CUDPInputView(CWnd* pParent /*=NULL*/)
	: CDockablePaneChildView(CUDPInputView::IDD, pParent)
	, m_multiPlotWindows(NULL)
	, m_updateIncTime(0)
	, m_isPause(false)
	, m_rcvPacketCount(0)
	, m_state(STOP)
	, m_modEditDlg(NULL)
	, m_plotCmdEditDlg(NULL)
	, m_plotInputDlg(NULL)
{
}

CUDPInputView::~CUDPInputView()
{
	DELETE_WINDOW(m_modEditDlg);
	DELETE_WINDOW(m_plotCmdEditDlg);
	DELETE_WINDOW(m_plotInputDlg);
}

void CUDPInputView::DoDataExchange(CDataExchange* pDX)
{
	CDockablePaneChildView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_UPDATE, m_UpdateButton);
	DDX_Control(pDX, IDC_RICHEDIT2_COMMAND, m_EditCommand);
	DDX_Control(pDX, IDC_STATIC_PACKET, m_PacketString);
	DDX_Control(pDX, IDC_EDIT_COMMAND, m_PlotCommandEditor);
	DDX_Control(pDX, IDC_CHECK_FIXEDMODE, m_FixedModeButton);
	DDX_Control(pDX, IDC_STATIC_PACKET_COUNT, m_PacketRecvCount);
//	DDX_Control(pDX, IDC_STATIC_PLOTINPUT, m_PlotInputString);
	DDX_Control(pDX, IDC_EDIT_PLOTINPUT, m_PlotInputStrEditor);
}


BEGIN_ANCHOR_MAP(CUDPInputView)
	ANCHOR_MAP_ENTRY(IDC_STATIC_GRAPH, ANF_LEFT | ANF_TOP | ANF_RIGHT | ANF_BOTTOM)
	ANCHOR_MAP_ENTRY(IDC_RICHEDIT2_COMMAND, ANF_LEFT | ANF_TOP | ANF_RIGHT)
	ANCHOR_MAP_ENTRY(IDC_EDIT_PLOTINPUT, ANF_LEFT | ANF_TOP | ANF_RIGHT)
	ANCHOR_MAP_ENTRY(IDC_EDIT_COMMAND, ANF_LEFT | ANF_RIGHT | ANF_BOTTOM)
	ANCHOR_MAP_ENTRY(IDC_STATIC1, ANF_LEFT | ANF_BOTTOM)
	ANCHOR_MAP_ENTRY(IDC_BUTTON_PLOTUPDATE, ANF_RIGHT | ANF_BOTTOM)
	ANCHOR_MAP_ENTRY(IDC_BUTTON_HELP, ANF_RIGHT | ANF_BOTTOM)
	ANCHOR_MAP_ENTRY(IDC_BUTTON_PLOTPAUSE, ANF_RIGHT | ANF_BOTTOM)
	ANCHOR_MAP_ENTRY(IDC_CHECK_FIXEDMODE, ANF_RIGHT | ANF_BOTTOM)
//	ANCHOR_MAP_ENTRY(IDC_STATIC2, ANF_LEFT | ANF_BOTTOM)
	ANCHOR_MAP_ENTRY(IDC_BUTTON_PLOTCMD, ANF_LEFT | ANF_BOTTOM)
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
	ON_BN_CLICKED(IDC_BUTTON_MOD_EDITDLG, &CUDPInputView::OnBnClickedButtonModEditdlg)
	ON_BN_CLICKED(IDC_BUTTON_PLOTINPUT_DLG, &CUDPInputView::OnBnClickedButtonPlotinputDlg)
	ON_BN_CLICKED(IDC_BUTTON_PLOTCMD, &CUDPInputView::OnBnClickedButtonPlotcmd)
	ON_BN_CLICKED(IDC_BUTTON_PLOTINPUT_UPDATE, &CUDPInputView::OnBnClickedButtonPlotinputUpdate)
END_MESSAGE_MAP()


// UDPInputView message handlers


void CUDPInputView::OnBnClickedOk()
{
}
void CUDPInputView::OnBnClickedCancel()
{
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


		CString strPlotInputComandEditor;
		if (!cMotionController::Get()->m_config.m_udpPlotInputCmd.empty())
		{
			strPlotInputComandEditor = str2wstr(cMotionController::Get()->m_config.m_udpPlotInputCmd).c_str();
		}
		else
		{
			strPlotInputComandEditor = L"$1;$2;$3;\r\n"
										L"$4;$5;$6;\r\n";
		}

		ParsePlotInputStringFormat(wstr2str((LPCTSTR)strPlotInputComandEditor), m_plotInputParser);
		m_PlotInputStrEditor.SetWindowTextW(strPlotInputComandEditor);

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
		CString plotInputCommand;
		m_PlotInputStrEditor.GetWindowText(plotInputCommand);
		cMotionController::Get()->m_config.m_udpModCommand = wstr2str((LPCTSTR)modCommand);
		cMotionController::Get()->m_config.m_udpPlotCommand = wstr2str((LPCTSTR)plotCommand);
		cMotionController::Get()->m_config.m_udpPlotInputCmd = wstr2str((LPCTSTR)plotInputCommand);
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
	m_multiPlotWindows->ProcessPlotCommand(str, 2);

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

	m_updateIncTime += deltaSeconds;

	if (!m_isPause)
		m_multiPlotWindows->DrawGraph(deltaSeconds, false);
}


void CUDPInputView::UpdateUDP(const char *buffer, const int bufferLen)
{
	RET(STOP == m_state);

	m_PacketString.SetWindowTextW(common::str2wstr(buffer).c_str());

	if (bufferLen < 10)
	{
		++m_rcvPacketCount;
		CString str;
		str.Format(L"%d", m_rcvPacketCount);
		m_PacketRecvCount.SetWindowTextW(str);
		return;
	}

	const float t = timeGetTime()*0.001f;
	float yaw, pitch, roll, heave;
	cMotionController::Get()->m_udpMod.GetOriginal(yaw, pitch, roll, heave);

	float origRoll, origPitch, origYaw, origHeave;
	cMotionController::Get()->m_udpMod.GetOriginal(origYaw, origPitch, origRoll, origHeave);

	float froll, fpitch, fyaw, fheave;
	cMotionController::Get()->m_udpMod.GetFinal(fyaw, fpitch, froll, fheave);

	if (!g_isReleaseMode)
	{
		for (u_int i = 0; i < m_plotInputParser.size(); ++i)
		{
			const string str = m_plotInputParser[i].Execute();
			m_multiPlotWindows->SetString(t, str.c_str(), i);
		}
	}

	++m_rcvPacketCount;
	CString str;
	str.Format(L"%d", m_rcvPacketCount);
	m_PacketRecvCount.SetWindowTextW(str);
}


void CUDPInputView::OnBnClickedButtonUdpServerBind()
{
	UpdateData();

	if (cController::Get()->GetUDPComm().IsConnect())
	{
		Stop();
	}
	else
	{
		Start();
	}	
}


void CUDPInputView::OnBnClickedButtonPlotupdate()
{
	CString str;
	m_PlotCommandEditor.GetWindowText(str);
	m_multiPlotWindows->ProcessPlotCommand(str, 2);
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

	CString str;
	m_PlotCommandEditor.GetWindowText(str);
	m_multiPlotWindows->ProcessPlotCommand(str, 2);

	CString command;
	m_EditCommand.GetWindowTextW(command);
	cMotionController::Get()->m_udpMod.ParseStr(common::wstr2str((LPCTSTR)command));

	SetBackgroundColor(g_blueColor);
}


// UDP 접속을 끊고, 화면 업데이트를 종료한다.
void CUDPInputView::Stop()
{
	m_state = STOP;

	SetBackgroundColor(g_grayColor);
}


void CUDPInputView::OnBnClickedButtonModEditdlg()
{
	if (!m_modEditDlg)
	{
		m_modEditDlg = new CEditDlg();
		m_modEditDlg->Create(CEditDlg::IDD);
	}

	m_modEditDlg->Init(L"UDP Input View -  Modulator Edit", &m_EditCommand, 
		this, (UpdateFunc)&CUDPInputView::OnBnClickedButtonUpdate);
	m_modEditDlg->ShowWindow(SW_SHOW);
}


void CUDPInputView::OnBnClickedButtonPlotinputDlg()
{
	if (!m_plotInputDlg)
	{
		m_plotInputDlg = new CPlotInputDlg();
		m_plotInputDlg->Create(CPlotInputDlg::IDD);
	}

	m_plotInputDlg->Init(L"UDP Input View - Plot Input String Format Edit", &m_PlotInputStrEditor,
		this, (UpdateFunc)&CUDPInputView::OnBnClickedButtonPlotinputUpdate);
	m_plotInputDlg->ShowWindow(SW_SHOW);
}


void CUDPInputView::OnBnClickedButtonPlotcmd()
{
	if (!m_plotCmdEditDlg)
	{
		m_plotCmdEditDlg = new CEditDlg();
		m_plotCmdEditDlg->Create(CEditDlg::IDD);
	}

	m_plotCmdEditDlg->Init(L"UDP Input View - Plot Command Edit", &m_PlotCommandEditor, 
		this, (UpdateFunc)&CUDPInputView::OnBnClickedButtonPlotupdate);
	m_plotCmdEditDlg->ShowWindow(SW_SHOW);
}



void CUDPInputView::OnBnClickedButtonPlotinputUpdate()
{
	CString tmp;
	m_PlotInputStrEditor.GetWindowTextW(tmp);

	const string str = wstr2str((LPCTSTR)tmp);
	if (str.empty())
		return;

	ParsePlotInputStringFormat(str, m_plotInputParser);
}

