// MixingView.cpp : implementation file
//

#include "stdafx.h"
#include "MotionMonitor.h"
#include "MixingView.h"
#include "afxdialogex.h"
#include "mixingconfig.h"
#include "MotionController.h"


const static CString g_mixingviewPlotCommand = L"plot1 = 0, 0, 0, 0, 0\r\n\
string1 = %f;\r\n\
name1 = Yaw\r\n\
plot2 = 0, 0, 0, 0, 0\r\n\
string2 = %*f; %f;\r\n\
name2 = Pitch\r\n\
plot3 = 0, 0, 0, 0, 0\r\n\
string3 = %*f; %*f; %f;\r\n\
name3 = Roll\r\n\
plot4 = 0, 0, 0, 0, 0\r\n\
string4 = %*f; %*f; %f; \r\n\
name4 = Heave\r\n";


// CMixingView dialog
CMixingView::CMixingView(CWnd* pParent /*=NULL*/)
	: CDockablePaneChildView(CMixingView::IDD, pParent)
	, m_multiPlotWindows(NULL)
	, m_incTime(0)
	, m_isStart(false)
{
}

CMixingView::~CMixingView()
{
}

void CMixingView::DoDataExchange(CDataExchange* pDX)
{
	CDockablePaneChildView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RICHEDIT2_COMMAND, m_EditCommand);
}


BEGIN_MESSAGE_MAP(CMixingView, CDockablePaneChildView)
	ON_BN_CLICKED(IDCANCEL, &CMixingView::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CMixingView::OnBnClickedOk)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_UPDATE, &CMixingView::OnBnClickedButtonUpdate)
END_MESSAGE_MAP()


BEGIN_ANCHOR_MAP(CMixingView)
	ANCHOR_MAP_ENTRY(IDC_RICHEDIT2_COMMAND, ANF_LEFT | ANF_TOP | ANF_RIGHT)
	ANCHOR_MAP_ENTRY(IDC_STATIC_GRAPH, ANF_LEFT | ANF_TOP | ANF_RIGHT | ANF_BOTTOM)
END_ANCHOR_MAP()


// CMixingView message handlers
BOOL CMixingView::OnInitDialog()
{
	CDockablePaneChildView::OnInitDialog();

	InitAnchors();

	const CString command =
L"#input = udp\n\
#input = motionwave\n\
input = joystick+udp+motionwave\n\
\n\
mixing_input1_enable = 1\n\
mixing_rate1_all = 1\n\
mixing_rate1_yaw = 0\n\
mixing_rate1_pitch = 0\n\
mixing_rate1_roll = 0\n\
mixing_rate1_heave = 0\n\
\n\
mixing_input2_enable = 0\n\
mixing_rate2_all = 0\n\
mixing_rate2_yaw = 0\n\
mixing_rate2_pitch = 0\n\
mixing_rate2_roll = 0\n\
mixing_rate2_heave = 0\n\
\n\
mixing_input3_enable = 0\n\
mixing_rate3_all = 0\n\
mixing_rate3_yaw = 0\n\
mixing_rate3_pitch = 0\n\
mixing_rate3_roll = 0\n\
mixing_rate3_heave = 0\n";
	m_EditCommand.SetWindowTextW(command);


	// PlotÃ¢ »ý¼º.
	CRect rect;
	GetClientRect(rect);

	m_multiPlotWindows = new CMultiPlotWindow();
	BOOL result = m_multiPlotWindows->Create(NULL, NULL, WS_VISIBLE | WS_CHILD,
		CRect(0, 0, rect.Width(), 400), this, AFX_IDW_PANE_FIRST);

	m_multiPlotWindows->SetScrollSizes(MM_TEXT, CSize(rect.Width() - 30, 100));
	m_multiPlotWindows->ShowWindow(SW_SHOW);
	m_multiPlotWindows->SetFixedWidthMode(true);


	return TRUE;
}


void CMixingView::OnBnClickedCancel()
{
	//CDockablePaneChildView::OnCancel();
}


void CMixingView::OnBnClickedOk()
{
	//CDockablePaneChildView::OnOK();
}


void CMixingView::OnSize(UINT nType, int cx, int cy)
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


void CMixingView::OnBnClickedButtonUpdate()
{
	UpdateData();

	CString command;
	m_EditCommand.GetWindowTextW(command);
	m_config.ParseStr(common::wstr2str((LPCTSTR)command).c_str());

	m_multiPlotWindows->ProcessPlotCommand(g_mixingviewPlotCommand, 2);
	m_multiPlotWindows->SetFixedWidthMode(true);

	m_isStart = true;
}


void CMixingView::Update(const float deltaSeconds)
{
	RET(!m_isStart);

	m_incTime += deltaSeconds;

	if (m_incTime > 0.01f)
	{
		float yaw, pitch, roll, heave;
		Mixing(deltaSeconds, yaw, pitch, roll, heave);

		m_multiPlotWindows->SetXY(0, yaw, 0);
		m_multiPlotWindows->SetXY(1, pitch, 0);
		m_multiPlotWindows->SetXY(2, roll, 0);
		m_multiPlotWindows->SetXY(3, heave, 0);
		m_multiPlotWindows->DrawGraph(m_incTime);

		m_incTime = 0;
	}
}


void CMixingView::Mixing(const float deltaSeconds, 
	OUT float &yaw, OUT float &pitch, OUT float &roll, OUT float &heave)
{
	yaw = 0;
	pitch = 0;
	roll = 0;
	heave = 0;

	if (m_config.m_inputType | INPUT_JOYSTICK)
	{
		if (m_config.m_input1_enable)
		{
			if (m_config.m_rate1_all == 0)
			{
				yaw += cMotionController::Get()->m_joystickYaw * m_config.m_rate1_yaw;
				pitch += cMotionController::Get()->m_joystickPitch * m_config.m_rate1_pitch;
				roll += cMotionController::Get()->m_joystickRoll * m_config.m_rate1_roll;
				heave += cMotionController::Get()->m_joystickHeave * m_config.m_rate1_heave;
			}
			else
			{
				yaw += cMotionController::Get()->m_joystickYaw * m_config.m_rate1_all;
				pitch += cMotionController::Get()->m_joystickPitch * m_config.m_rate1_all;
				roll += cMotionController::Get()->m_joystickRoll * m_config.m_rate1_all;
				heave += cMotionController::Get()->m_joystickHeave * m_config.m_rate1_all;
			}
		}
	}

}
