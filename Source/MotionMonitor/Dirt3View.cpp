// Dirt3View.cpp : implementation file
//

#include "stdafx.h"
#include "MotionMonitor.h"
#include "Dirt3View.h"
#include "Dirt3Controller.h"
#include "afxdialogex.h"
#include "MotionOutputView.h"
#include "MainFrm.h"


// CDirt3View dialog

CDirt3View::CDirt3View(CWnd* pParent /*=NULL*/)
: CDockablePaneChildView(CDirt3View::IDD, pParent)
, m_state(STOP)
, m_IsMotionSim(FALSE)
, m_titleImage(common::str2wstr("../media/dirt3/title.jpg").c_str())
, m_controllerState(-1)
, m_controllerSubState(-1)
{
}

CDirt3View::~CDirt3View()
{
}

void CDirt3View::DoDataExchange(CDataExchange* pDX)
{
	CDockablePaneChildView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_START, m_StartButton);
	DDX_Check(pDX, IDC_CHECK_MOTION_SIM, m_IsMotionSim);
	DDX_Control(pDX, IDC_STATIC_STATE1, m_State1Text);
	DDX_Control(pDX, IDC_STATIC_STATE2, m_State2Text);
}


BEGIN_MESSAGE_MAP(CDirt3View, CDockablePaneChildView)
	ON_BN_CLICKED(IDOK, &CDirt3View::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDirt3View::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_START, &CDirt3View::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_CHECK_MOTION_SIM, &CDirt3View::OnBnClickedCheckMotionSim)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_EMERGENCY, &CDirt3View::OnBnClickedButtonEmergency)
END_MESSAGE_MAP()


// CDirt3View message handlers
void CDirt3View::OnBnClickedOk()
{
}
void CDirt3View::OnBnClickedCancel()
{
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
	cDirt3Controller::Get()->Update(deltaSeconds);


	// 정보가 바뀔 때, UI 출력을 변경한다.
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
