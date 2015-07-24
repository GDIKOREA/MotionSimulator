// MotionInputView.cpp : implementation file
//

#include "stdafx.h"
#include "MotionMonitor.h"
#include "MotionInputView.h"
#include "afxdialogex.h"
#include "MotionController.h"


// CMotionInputView dialog
CMotionInputView::CMotionInputView(CWnd* pParent /*=NULL*/)
	: CDockablePaneChildView(CMotionInputView::IDD, pParent)
	, m_multiPlotWindows(NULL)
	, m_incTime(0)
{

}

CMotionInputView::~CMotionInputView()
{
}

void CMotionInputView::DoDataExchange(CDataExchange* pDX)
{
	CDockablePaneChildView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_UPDATE, m_UpdateButton);
	DDX_Control(pDX, IDC_RICHEDIT2_COMMAND, m_EditCommand);
}


BEGIN_ANCHOR_MAP(CMotionInputView)
	ANCHOR_MAP_ENTRY(IDC_STATIC_GRAPH, ANF_LEFT | ANF_TOP | ANF_RIGHT | ANF_BOTTOM)
	ANCHOR_MAP_ENTRY(IDC_RICHEDIT2_COMMAND, ANF_LEFT | ANF_TOP | ANF_RIGHT)
END_ANCHOR_MAP()


BEGIN_MESSAGE_MAP(CMotionInputView, CDockablePaneChildView)
	ON_BN_CLICKED(IDOK, &CMotionInputView::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CMotionInputView::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_UPDATE, &CMotionInputView::OnBnClickedButtonUpdate)
	ON_WM_SIZE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CMotionInputView message handlers


void CMotionInputView::OnBnClickedOk()
{
	//CDockablePaneChildView::OnOK();
}


void CMotionInputView::OnBnClickedCancel()
{
	//CDockablePaneChildView::OnCancel();
}


BOOL CMotionInputView::OnInitDialog()
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
	std::ifstream cfgfile("udpmod_config.cfg");
	if (cfgfile.is_open())
	{
		std::string str((std::istreambuf_iterator<char>(cfgfile)), std::istreambuf_iterator<char>());
		cmdStr = str2wstr(str).c_str();
	}
	else
	{
		cmdStr = command;
	}
	m_EditCommand.SetWindowTextW(cmdStr);

	return TRUE;	
}


void CMotionInputView::OnSize(UINT nType, int cx, int cy)
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


void CMotionInputView::OnBnClickedButtonUpdate()
{
	UpdateData();

	CString plotCommand = L"plot1 = 0, 0, 0, 0, 0\r\n\
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

	m_multiPlotWindows->ProcessPlotCommand(plotCommand, 2);
	m_multiPlotWindows->SetFixedWidthMode(true);


	CString command;
	m_EditCommand.GetWindowTextW(command);
	cMotionController::Get()->m_udpMod.ParseStr(common::wstr2str((LPCTSTR)command).c_str());
}


// Update
void CMotionInputView::Update(const float deltaSeconds)
{
	if (!m_multiPlotWindows)
		return;

	m_incTime += deltaSeconds;

	if (m_incTime > 0.033f)
	{
		float origRoll, origPitch, origYaw, origHeave;
		cMotionController::Get()->m_udpMod.GetOriginal(origYaw, origPitch, origRoll, origHeave);
		m_multiPlotWindows->SetString(common::format("%f;%f;%f;%f", origYaw, origPitch, origRoll, origHeave).c_str());


		float roll, pitch, yaw, heave;
		cMotionController::Get()->m_udpMod.GetFinal(yaw, pitch, roll, heave);
		m_multiPlotWindows->SetString(common::format("%f;%f;%f;%f;", yaw, pitch, roll, heave).c_str(), 1);

		m_multiPlotWindows->DrawGraph(m_incTime);

		m_incTime = 0.f;
	}
}


void CMotionInputView::OnDestroy()
{
	UpdateData();

	// 환경파일 저장
	std::ofstream cfgfile("udpmod_config.cfg");
	if (cfgfile.is_open())
	{
		CString command;
		m_EditCommand.GetWindowTextW(command);
		string str = wstr2str((LPCTSTR)command);
		cfgfile << str;
	}

	CDockablePaneChildView::OnDestroy();
}
