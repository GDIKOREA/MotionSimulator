// Dirt3View.cpp : implementation file
//

#include "stdafx.h"
#include "MotionMonitor.h"
#include "Dirt3View.h"
#include "Dirt3Controller.h"
#include "afxdialogex.h"


// CDirt3View dialog

CDirt3View::CDirt3View(CWnd* pParent /*=NULL*/)
: CDockablePaneChildView(CDirt3View::IDD, pParent)
, m_state(STOP)
, m_IsMotionSim(FALSE)
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
}


BEGIN_MESSAGE_MAP(CDirt3View, CDockablePaneChildView)
	ON_BN_CLICKED(IDOK, &CDirt3View::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDirt3View::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_START, &CDirt3View::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_CHECK_MOTION_SIM, &CDirt3View::OnBnClickedCheckMotionSim)
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
}


void CDirt3View::UpdateUDP(const char *buffer, const int bufferLen)
{
	cDirt3Controller::Get()->UpdateUDP(buffer, bufferLen);
}


void CDirt3View::OnBnClickedCheckMotionSim()
{
	UpdateData();
}

