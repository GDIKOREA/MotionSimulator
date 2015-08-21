// MixingView.cpp : implementation file
//

#include "stdafx.h"
#include "MotionMonitor.h"
#include "MixingView.h"
#include "afxdialogex.h"
#include "mixingconfig.h"


const static CString g_mixingviewPlotCommand = L"\r\n\
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
string4 = %*f; %*f; %f; \r\n\
name4 = Heave\r\n\
linewidth4 = 2\r\n\
";


// CMixingView dialog
CMixingView::CMixingView(CWnd* pParent /*=NULL*/)
	: CDockablePaneChildView(CMixingView::IDD, pParent)
	, m_multiPlotWindows(NULL)
	, m_incTime(0)
	, m_totalIncTime(0)
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
	ON_WM_DESTROY()
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

	UpdateConfig();

	// Plot창 생성.
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


void CMixingView::UpdateConfig(bool IsSaveAndValidate) //IsSaveAndValidate=true
{
	if (IsSaveAndValidate)
	{
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
mixing_rate3_heave = 0\n\
\n\
mixing_bias_yaw = 0\n\
mixing_bias_pitch = 0\n\
mixing_bias_roll = 0\n\
mixing_bias_heave = 0\n\
\n\
";

		CString cmdStr;
		if (!cMotionController::Get()->m_config.m_mixingModCommand.empty())
		{
			cmdStr = str2wstr(cMotionController::Get()->m_config.m_mixingModCommand).c_str();
		}
		else
		{
			cmdStr = command;
		}
		m_EditCommand.SetWindowTextW(cmdStr);

		m_config.ParseStr(wstr2str((LPCTSTR)cmdStr));

		UpdateData(FALSE);
	}
	else
	{
		UpdateData();

		// 환경파일 저장
		CString command;
		m_EditCommand.GetWindowTextW(command);
		cMotionController::Get()->m_config.m_mixingModCommand = wstr2str((LPCTSTR)command);
	}
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

	SetBackgroundColor(g_blueColor);

	m_isStart = true;

	m_incTime = 0;
}


void CMixingView::Update(const float deltaSeconds)
{
	RET(!m_isStart);

	const float elapseT = 0.033f;
	m_incTime += deltaSeconds;
	m_totalIncTime += deltaSeconds;

	if (m_incTime > elapseT)
	{
		float yaw, pitch, roll, heave;
		Mixing(deltaSeconds, yaw, pitch, roll, heave);

		cMotionController::Get()->m_yaw = yaw;
		cMotionController::Get()->m_pitch = pitch;
		cMotionController::Get()->m_roll = roll;
		cMotionController::Get()->m_heave = heave;

		m_multiPlotWindows->SetXY(0, m_totalIncTime, yaw, 0);
		m_multiPlotWindows->SetXY(1, m_totalIncTime, pitch, 0);
		m_multiPlotWindows->SetXY(2, m_totalIncTime, roll, 0);
		m_multiPlotWindows->SetXY(3, m_totalIncTime, heave, 0);
		m_multiPlotWindows->DrawGraph(m_incTime, false);


		// render 3D Model
// 		const float yawRadian = (yaw - 256.f) / 512.f;
// 		const float pitchRadian = -(pitch - 256.f) / 512.f;
// 		const float rollRadian = -(roll - 256.f) / 512.f;
// 		cController::Get()->GetCubeFlight().SetEulerAngle(rollRadian, pitchRadian, yawRadian);


		m_incTime -= elapseT;
		if (m_incTime > elapseT)
			m_incTime = 0;
	}
}

float calcRate(const float src, const float center, const float rate)
{
	return (src - center) * rate + center;
}


// 입력값을 믹싱해서 리턴한다.
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
			float joyYaw, joyPitch, joyRoll, joyHeave;
			cMotionController::Get()->m_joystickMod.GetFinal(joyYaw, joyPitch, joyRoll, joyHeave);

			if (m_config.m_rate1_all == 0)
			{
				yaw += calcRate(joyYaw, m_config.m_rate1_center_yaw, m_config.m_rate1_yaw);
				pitch += calcRate(joyPitch, m_config.m_rate1_center_pitch, m_config.m_rate1_pitch);
				roll += calcRate(joyRoll, m_config.m_rate1_center_roll, m_config.m_rate1_roll);
				heave += calcRate(joyHeave, m_config.m_rate1_center_heave, m_config.m_rate1_heave);
			}
			else
			{
				yaw += calcRate(joyYaw, m_config.m_rate1_center_yaw, m_config.m_rate1_all);
				pitch += calcRate(joyPitch, m_config.m_rate1_center_pitch, m_config.m_rate1_all);
				roll += calcRate(joyRoll, m_config.m_rate1_center_roll, m_config.m_rate1_all);
				heave += calcRate(joyHeave, m_config.m_rate1_center_heave, m_config.m_rate1_all);
			}
		}
	}

	if (m_config.m_inputType | INPUT_UDP)
	{
		if (m_config.m_input2_enable)
		{
			float udpYaw, udpPitch, udpRoll, udpHeave;
			cMotionController::Get()->m_udpMod.GetFinal(udpYaw, udpPitch, udpRoll, udpHeave);

			if (m_config.m_rate2_all == 0)
			{
				yaw += calcRate(udpYaw, m_config.m_rate2_center_yaw, m_config.m_rate2_yaw);
				pitch += calcRate(udpPitch, m_config.m_rate2_center_pitch, m_config.m_rate2_pitch);
				roll += calcRate(udpRoll, m_config.m_rate2_center_roll, m_config.m_rate2_roll);
				heave += calcRate(udpHeave, m_config.m_rate2_center_heave, m_config.m_rate2_heave);
			}
			else
			{
				yaw += calcRate(udpYaw, m_config.m_rate2_center_yaw, m_config.m_rate2_all);
				pitch += calcRate(udpPitch, m_config.m_rate2_center_pitch, m_config.m_rate2_all);
				roll += calcRate(udpRoll, m_config.m_rate2_center_roll, m_config.m_rate2_all);
				heave += calcRate(udpHeave, m_config.m_rate2_center_heave, m_config.m_rate2_all);
			}
		}
	}

	if (m_config.m_inputType | INPUT_MOTIONWAVE)
	{
		if (m_config.m_input3_enable)
		{
			float mwavYaw, mwavPitch, mwavRoll, mwavHeave;
			cMotionController::Get()->m_mwavMod.GetFinal(mwavYaw, mwavPitch, mwavRoll, mwavHeave);

			if (m_config.m_rate3_all == 0)
			{
				yaw += calcRate(mwavYaw, m_config.m_rate3_center_yaw, m_config.m_rate3_yaw);
				pitch += calcRate(mwavPitch, m_config.m_rate3_center_pitch, m_config.m_rate3_pitch);
				roll += calcRate(mwavRoll, m_config.m_rate3_center_roll, m_config.m_rate3_roll);
				heave += calcRate(mwavHeave, m_config.m_rate3_center_heave, m_config.m_rate3_heave);
			}
			else
			{
				yaw += calcRate(mwavYaw, m_config.m_rate3_center_yaw, m_config.m_rate3_all);
				pitch += calcRate(mwavPitch, m_config.m_rate3_center_pitch, m_config.m_rate3_all);
				roll += calcRate(mwavRoll, m_config.m_rate3_center_roll, m_config.m_rate3_all);
				heave += calcRate(mwavHeave, m_config.m_rate3_center_heave, m_config.m_rate3_all);
			}
		}
	}

	yaw += m_config.m_bias_yaw;
	pitch += m_config.m_bias_pitch;
	roll += m_config.m_bias_roll;
	heave += m_config.m_bias_heave;


	if (pitch > 512)
		pitch = 512;
	if (pitch < 0)
		pitch = 0;

	if (roll > 512)
		roll = 512;
	if (roll < 0)
		roll = 0;

	if (yaw > 512)
		yaw = 512;
	if (yaw < 0)
		yaw = 0;

	if (heave > 512)
		heave = 512;
	if (heave < 0)
		heave = 0;

}


void CMixingView::OnDestroy()
{
	UpdateConfig(false);

	CDockablePaneChildView::OnDestroy();
}
