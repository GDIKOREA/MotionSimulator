// MotionControlView.cpp : implementation file
//

#include "stdafx.h"
#include "MotionMonitor.h"
#include "MotionControlView.h"
#include "afxdialogex.h"
#include "MotionController.h"


// CMotionControlView dialog
CMotionControlView::CMotionControlView(CWnd* pParent /*=NULL*/)
	: CDockablePaneChildView(CMotionControlView::IDD, pParent)
	, m_multiPlotWindows(NULL)
	, m_incTime(0)
{

}

CMotionControlView::~CMotionControlView()
{
}

void CMotionControlView::DoDataExchange(CDataExchange* pDX)
{
	CDockablePaneChildView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_UPDATE, m_UpdateButton);
	DDX_Control(pDX, IDC_RICHEDIT2_COMMAND, m_EditCommand);
}


BEGIN_ANCHOR_MAP(CMotionControlView)
	ANCHOR_MAP_ENTRY(IDC_STATIC_GRAPH, ANF_LEFT | ANF_TOP | ANF_RIGHT | ANF_BOTTOM)
	ANCHOR_MAP_ENTRY(IDC_RICHEDIT2_COMMAND, ANF_LEFT | ANF_TOP | ANF_RIGHT )
END_ANCHOR_MAP()

BEGIN_MESSAGE_MAP(CMotionControlView, CDockablePaneChildView)
	ON_BN_CLICKED(IDOK, &CMotionControlView::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CMotionControlView::OnBnClickedCancel)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_UPDATE, &CMotionControlView::OnBnClickedButtonUpdate)
END_MESSAGE_MAP()


// CMotionControlView message handlers


void CMotionControlView::OnBnClickedOk()
{
}


void CMotionControlView::OnBnClickedCancel()
{
}


BOOL CMotionControlView::OnInitDialog()
{
	CDockablePaneChildView::OnInitDialog();

	InitAnchors();

	// PlotÃ¢ »ý¼º.
	CRect rect;
	GetClientRect(rect);

	m_multiPlotWindows = new CMultiPlotWindow();
	BOOL result = m_multiPlotWindows->Create(NULL, NULL, WS_VISIBLE | WS_CHILD,
		CRect(0, 0, rect.Width(), 400), this, AFX_IDW_PANE_FIRST);

	m_multiPlotWindows->SetScrollSizes(MM_TEXT, CSize(rect.Width() - 30, 100));
	m_multiPlotWindows->ShowWindow(SW_SHOW);

	const CString command =
L"\n\
yaw_proportion = 0\n\
pitch_proportion = 0.3\n\
roll_proportion = 0.3\n\
heave_proportion = 0\n\
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
";
	m_EditCommand.SetWindowTextW(command);

	return TRUE;
}


void CMotionControlView::OnSize(UINT nType, int cx, int cy)
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


void CMotionControlView::Update(const float deltaSeconds)
{
	if (!m_multiPlotWindows)
		return;

	m_incTime += deltaSeconds;

	if (m_incTime > 0.033f)
	{
		float roll, pitch, yaw, heave;
		cMotionController::Get()->m_udpMod.GetFinal(yaw, pitch, roll, heave);
		m_multiPlotWindows->SetString( common::format("%f;%f;%f;", yaw, pitch, roll).c_str(), 1 );

		float originalRoll, originalPitch, originalYaw, originalHeave;
		cMotionController::Get()->m_udpMod.GetOriginal(originalYaw, originalPitch, originalRoll, originalHeave);
		m_multiPlotWindows->SetString(common::format("%f;%f;%f;", originalYaw, originalPitch, originalRoll).c_str(), 0);


		m_multiPlotWindows->DrawGraph(m_incTime);

		m_incTime = 0.f;
	}
}


void CMotionControlView::OnBnClickedButtonUpdate()
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
name3 = Roll\r\n";
	m_multiPlotWindows->ProcessPlotCommand(plotCommand, 2);
	m_multiPlotWindows->SetFixedWidthMode(true);


	CString command;
	m_EditCommand.GetWindowTextW(command);
	cMotionController::Get()->m_udpMod.ParseStr(common::wstr2str((LPCTSTR)command).c_str());
}

