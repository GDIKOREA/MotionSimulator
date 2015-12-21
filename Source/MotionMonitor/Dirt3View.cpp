Dirt3View.cpp : implementation file
//

#include "stdafx.h"
#include "MotionMonitor.h"
#include "Dirt3View.h"
#include "Dirt3Controller.h"
#include "afxdialogex.h"
#include "MixingView.h"
#include "MotionOutputView.h"
#include "MainFrm.h"


// CDirt3View dialog

CDirt3View::CDirt3View(CWnd* pParent /*=NULL*/)
: CDockablePaneChildView(CDirt3View::IDD, pParent)
, m_state(STOP)
, m_IsMotionSim(TRUE)
, m_IsAutoStart(FALSE)
, m_titleImage(common::str2wstr("../media/dirt3/title.jpg").c_str())
, m_controllerMainState(-1)
, m_controllerState(-1)
, m_controllerSubState(-1)
, m_enChange1(false)
, m_enChange2(false)
, m_enChange3(false)
, m_displayPlayTime(0)
, m_startTimer(0)
{
}

CDirt3View::~CDirt3View()
{
}

void CDirt3View::DoDataExchange(CDataExchange* pDX)
{
	CDockablePaneChildView::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_4AXIS, m_radioAxisType);
	DDX_Control(pDX, IDC_BUTTON_START, m_StartButton);
	DDX_Check(pDX, IDC_CHECK_MOTION_SIM, m_IsMotionSim);
	DDX_Check(pDX, IDC_CHECK_AUTOSTART, m_IsAutoStart);	
	DDX_Control(pDX, IDC_STATIC_STATE0, m_State0Text);
	DDX_Control(pDX, IDC_STATIC_STATE1, m_State1Text);
	DDX_Control(pDX, IDC_STATIC_STATE2, m_State2Text);
	DDX_Control(pDX, IDC_SLIDER_ACTION_RANGE, m_sliderActuatorRange);
	DDX_Control(pDX, IDC_EDIT_ACTUATOR, m_editActuatorRange);
	DDX_Control(pDX, IDC_SLIDER_ACTION_SPEED, m_sliderActuatorSpeed);
	DDX_Control(pDX, IDC_EDIT_ACTUATOR_SPEED, m_editActuatorSpeed);
	DDX_Control(pDX, IDC_EDIT_PLAYTIME, m_editPlayTime);
	DDX_Control(pDX, IDC_STATIC_PLAYTIME, m_PlayTimeText);
	DDX_Control(pDX, IDC_SLIDER_YAW_RANGE, m_sliderActuatorYawRange);
	DDX_Control(pDX, IDC_EDIT_ACTUATOR_YAW, m_editActuatorYawRange);
	DDX_Control(pDX, IDC_STATIC_FRAME, m_FrameStr);
}


BEGIN_MESSAGE_MAP(CDirt3View, CDockablePaneChildView)
	ON_BN_CLICKED(IDOK, &CDirt3View::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDirt3View::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_START, &CDirt3View::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_CHECK_MOTION_SIM, &CDirt3View::OnBnClickedCheckMotionSim)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_EMERGENCY, &CDirt3View::OnBnClickedButtonEmergency)
	ON_BN_CLICKED(IDC_RADIO_4AXIS, &CDirt3View::OnBnClickedRadio4axis)
	ON_BN_CLICKED(IDC_RADIO_3AXIS, &CDirt3View::OnBnClickedRadio3axis)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_ACTION_RANGE, &CDirt3View::OnNMCustomdrawSliderActionRange)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_ACTION_SPEED, &CDirt3View::OnNMCustomdrawSliderActionSpeed)
	ON_EN_CHANGE(IDC_EDIT_ACTUATOR, &CDirt3View::OnEnChangeEditActuator)
	ON_EN_CHANGE(IDC_EDIT_ACTUATOR_SPEED, &CDirt3View::OnEnChangeEditActuatorSpeed)
	ON_EN_CHANGE(IDC_EDIT_PLAYTIME, &CDirt3View::OnEnChangeEditPlaytime)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_YAW_RANGE, &CDirt3View::OnNMCustomdrawSliderYawRange)
	ON_EN_CHANGE(IDC_EDIT_ACTUATOR_YAW, &CDirt3View::OnEnChangeEditActuatorYaw)
	ON_BN_CLICKED(IDC_CHECK_AUTOSTART, &CDirt3View::OnBnClickedCheckAutostart)
END_MESSAGE_MAP()


// CDirt3View message handlers
void CDirt3View::OnBnClickedOk()
{
}
void CDirt3View::OnBnClickedCancel()
{
}


BOOL CDirt3View::OnInitDialog()
{
	CDockablePaneChildView::OnInitDialog();

	UpdateConfig();

	float actuatorPowerRate = 1;
	float actuatorYawRangeRate = 1;
	float actuatorSpeedRate = 1;
	m_radioAxisType = AXIS_TYPE::AXIS4;

	const int maxRange = MAX_SLIDER_LIMIT;
	m_sliderActuatorRange.SetRange(0, maxRange);
	m_sliderActuatorRange.SetPos((int)((float)maxRange * actuatorPowerRate));

	m_sliderActuatorYawRange.SetRange(0, maxRange);
	m_sliderActuatorYawRange.SetPos((int)((float)maxRange * actuatorYawRangeRate));

	m_sliderActuatorSpeed.SetRange(0, maxRange);
	m_sliderActuatorSpeed.SetPos((int)((float)maxRange * actuatorSpeedRate));

	CString playTimeStr;
	playTimeStr.Format(L"%f", cMotionController::Get()->m_config.m_dirt3ViewPlayTime);
	m_editPlayTime.SetWindowTextW(playTimeStr);

	UpdateData(FALSE);
	return TRUE;
}


void CDirt3View::UpdateConfig(bool IsSaveAndValidate)
{
	if (IsSaveAndValidate)
	{
		m_radioAxisType = cMotionController::Get()->m_config.m_dirt3ViewAxisType;
// 		if (CMainFrame *frm = dynamic_cast<CMainFrame*>(AfxGetMainWnd()))
// 			frm->m_mixingView->SetMixingAxisMode((AXIS_TYPE::TYPE)cMotionController::Get()->m_config.m_dirt3ViewAxisType);

		CString strPlayTime;
		strPlayTime.Format(L"%f", cMotionController::Get()->m_config.m_dirt3ViewPlayTime);
		m_editPlayTime.SetWindowTextW(strPlayTime);

		const int rangePos = (int)(cMotionController::Get()->m_config.m_dirt3ViewActuatorPower * MAX_SLIDER_LIMIT);
		const int yawPos = (int)(cMotionController::Get()->m_config.m_dirt3ViewActuatorYawPower * MAX_SLIDER_LIMIT);
		const int speedPos = (int)(cMotionController::Get()->m_config.m_dirt3ViewActuatorSpeed * MAX_SLIDER_LIMIT);
		m_sliderActuatorRange.SetPos(rangePos);
		m_sliderActuatorYawRange.SetPos(yawPos);
		m_sliderActuatorSpeed.SetPos(speedPos);

		const bool autoStart = cMotionController::Get()->m_config.m_dirt3ViewAutoStart;
		m_IsAutoStart = autoStart? TRUE : FALSE;

		static bool onlyOne = true; // 프로그램 시작시 한번만 실행된다.
		if (onlyOne)
		{
			onlyOne = false;
			if (autoStart)
			{
				// 일정 시간이 지난 후 Start를 시작한다.
				m_startTimer = timeGetTime();
			}
		}

// 		if (CMainFrame *frm = dynamic_cast<CMainFrame*>(AfxGetMainWnd()))
// 			frm->m_mixingView->SetActuatorPowerRate(cMotionController::Get()->m_config.m_dirt3ViewActuatorPower);
		//cController::Get()->SetActuatorSpeed(cMotionController::Get()->m_config.m_dirt3ViewActuatorSpeed);

		UpdateData(FALSE);
	}
	else
	{
		UpdateData();

		cMotionController::Get()->m_config.m_dirt3ViewAutoStart = m_IsAutoStart? true : false;

// 		if (CMainFrame *frm = dynamic_cast<CMainFrame*>(AfxGetMainWnd()))
// 			cMotionController::Get()->m_config.m_dirt3ViewAxisType = frm->m_mixingView->GetMixingAxisMode();

		//cMotionController::Get()->m_config.m_dirt3ViewPlayTime = cController::Get()->GetPlayTime();

// 		if (CMainFrame *frm = dynamic_cast<CMainFrame*>(AfxGetMainWnd()))
// 			cMotionController::Get()->m_config.m_dirt3ViewActuatorPower = frm->m_mixingView->GetActuatorPowerRate();

		//cMotionController::Get()->m_config.m_dirt3ViewActuatorYawPower = cController::Get()->GetActuatorYawPower();
		//cMotionController::Get()->m_config.m_dirt3ViewActuatorSpeed = cController::Get()->GetActuatorSpeed();
	}
}


void CDirt3View::OnBnClickedButtonStart()
{
	if (STOP == m_state)
	{
		m_state = START;

		cDirt3Controller::Get()->StartMotionSim("../media/dirt3/motionmonitor_dirt3.json", m_IsMotionSim? true : false);
		m_StartButton.SetWindowTextW(L"Stop");
		SetBackgroundColor(g_blueColor);
	}
	else if (START == m_state)
	{
		m_state = STOP;

		cDirt3Controller::Get()->StopMotionSim();
		m_StartButton.SetWindowTextW(L"Start");
		SetBackgroundColor(g_grayColor);
	}
}


void CDirt3View::Update(const float deltaSeconds)
{
	if (m_startTimer)
	{
		// 프로그램이 시작된 후, 5초후에 Start 상태로 시작한다.
		if (timeGetTime() - m_startTimer > 5000)
		{
			m_startTimer = 0;
			OnBnClickedButtonStart();
		}
	}

	cDirt3Controller::Get()->Update(deltaSeconds);

	// 정보가 바뀔 때, UI 출력을 변경한다.
	if (cDirt3Controller::Get()->GetState() != m_controllerMainState)
	{
		m_controllerMainState = cDirt3Controller::Get()->GetState();
		m_State0Text.SetWindowTextW(str2wstr(cDirt3Controller::Get()->GetStateStr()).c_str());
	}
	if (cDirt3Controller::Get()->GetMotionSim().GetState() != m_controllerState)
	{
		m_controllerState = cDirt3Controller::Get()->GetMotionSim().GetState();
		m_State1Text.SetWindowTextW(str2wstr(cDirt3Controller::Get()->GetMotionSim().GetStateStr()).c_str());
	}
	if (cDirt3Controller::Get()->GetMotionSim().GetSubState() != m_controllerSubState)
	{
		m_controllerSubState = cDirt3Controller::Get()->GetMotionSim().GetSubState();
		m_State2Text.SetWindowTextW(str2wstr(cDirt3Controller::Get()->GetMotionSim().GetSubStateStr()).c_str());
	}
	if ((int)cDirt3Controller::Get()->GetMotionSim().GetPlayTime() != m_displayPlayTime)
	{
		CString str;
		str.Format(L"%d", (int)cDirt3Controller::Get()->GetMotionSim().GetPlayTime());
		m_displayPlayTime = (int)cDirt3Controller::Get()->GetMotionSim().GetPlayTime();
		m_PlayTimeText.SetWindowTextW(str);
	}
}


void CDirt3View::UpdateUDP(const char *buffer, const int bufferLen)
{
	cDirt3Controller::Get()->UpdateUDP(buffer, bufferLen);
}


void CDirt3View::OnBnClickedCheckMotionSim()
{
	UpdateData();
}


void CDirt3View::OnPaint()
{
	CPaintDC dc(this);

	// 타이틀 이미지 출력
	Gdiplus::Graphics graphic(dc);
	graphic.DrawImage(&m_titleImage, Gdiplus::Rect(0, 0, m_titleImage.GetWidth(), m_titleImage.GetHeight()));
}


// 비상 정지 버튼 클릭
void CDirt3View::OnBnClickedButtonEmergency()
{
	if (CMainFrame *pFrm = dynamic_cast<CMainFrame*>(AfxGetMainWnd()))
	{
		if (pFrm->m_motionOutputView)
			pFrm->m_motionOutputView->EmergencyStop();
	}
}


void CDirt3View::OnBnClickedRadio4axis()
{
	UpdateData();

	cMotionController::Get()->m_config.m_dirt3ViewAxisType = m_radioAxisType;
// 	if (CMainFrame *frm = dynamic_cast<CMainFrame*>(AfxGetMainWnd()))
// 		frm->m_mixingView->SetMixingAxisMode((AXIS_TYPE::TYPE)m_radioAxisType);
}

void CDirt3View::OnBnClickedRadio3axis()
{
	UpdateData();

	cMotionController::Get()->m_config.m_dirt3ViewAxisType = m_radioAxisType;
// 	if (CMainFrame *frm = dynamic_cast<CMainFrame*>(AfxGetMainWnd()))
// 		frm->m_mixingView->SetMixingAxisMode((AXIS_TYPE::TYPE)m_radioAxisType);
}


void CDirt3View::OnNMCustomdrawSliderActionRange(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	*pResult = 0;

	const float rate = (float)m_sliderActuatorRange.GetPos() / (float)MAX_SLIDER_LIMIT;
	cMotionController::Get()->m_config.m_dirt3ViewActuatorPower = rate;

	if (!m_enChange1)
	{
		CString str;
		str.Format(L"%f", rate);
		m_editActuatorRange.SetWindowTextW(str);
	}

}


void CDirt3View::OnNMCustomdrawSliderActionSpeed(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	*pResult = 0;

	if (CMainFrame *frm = dynamic_cast<CMainFrame*>(AfxGetMainWnd()))
	{
		const float rate = (float)m_sliderActuatorSpeed.GetPos() / (float)MAX_SLIDER_LIMIT;
		cMotionController::Get()->m_config.m_dirt3ViewActuatorSpeed = rate;

		if (!m_enChange2)
		{
			CString str;
			str.Format(L"%f", rate);
			m_editActuatorSpeed.SetWindowTextW(str);
		}
	}
}


void CDirt3View::OnNMCustomdrawSliderYawRange(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	*pResult = 0;

	if (CMainFrame *frm = dynamic_cast<CMainFrame*>(AfxGetMainWnd()))
	{
		const float rate = (float)m_sliderActuatorYawRange.GetPos() / (float)MAX_SLIDER_LIMIT;
		cMotionController::Get()->m_config.m_dirt3ViewActuatorYawPower = rate;

		if (!m_enChange3)
		{
			CString str;
			str.Format(L"%f", rate);
			m_editActuatorYawRange.SetWindowTextW(str);
		}
	}
}


void CDirt3View::OnEnChangeEditActuatorYaw()
{
	CString str;
	m_editActuatorYawRange.GetWindowTextW(str);
	const float rate = (float)_wtof((LPCTSTR)str);

	m_enChange3 = true;
	m_sliderActuatorYawRange.SetPos((int)(rate * MAX_SLIDER_LIMIT));
	m_enChange3 = false;
}


void CDirt3View::OnEnChangeEditActuator()
{
	CString str;
	m_editActuatorRange.GetWindowTextW(str);
	const float rate = (float)_wtof((LPCTSTR)str);

	m_enChange1 = true;
	m_sliderActuatorRange.SetPos((int)(rate * MAX_SLIDER_LIMIT));
	m_enChange1 = false;
}


void CDirt3View::OnEnChangeEditActuatorSpeed()
{
	CString str;
	m_editActuatorSpeed.GetWindowTextW(str);
	const float rate = (float)_wtof((LPCTSTR)str);

	m_enChange2 = true;
	m_sliderActuatorSpeed.SetPos((int)(rate * MAX_SLIDER_LIMIT));
	m_enChange2 = false;
}


void CDirt3View::OnEnChangeEditPlaytime()
{
	CString str;
	m_editPlayTime.GetWindowTextW(str);
	const float t = (float)_wtof((LPCTSTR)str);

	cMotionController::Get()->m_config.m_dirt3ViewPlayTime = t;
}
  

void CDirt3View::OnBnClickedCheckAutostart()
{

}
