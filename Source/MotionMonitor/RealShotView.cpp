// RealShotView.cpp : implementation file
//

#include "stdafx.h"
#include "MotionMonitor.h"
#include "RealShotView.h"
#include "afxdialogex.h"
#include "RealshotController.h"


// CRealShotView dialog

CRealShotView::CRealShotView(CWnd* pParent /*=NULL*/)
: CDockablePaneChildView(CRealShotView::IDD, pParent)
, m_isStart(false)
{
}

CRealShotView::~CRealShotView()
{
}

void CRealShotView::DoDataExchange(CDataExchange* pDX)
{
	CDockablePaneChildView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_START, m_StartButton);
}


BEGIN_MESSAGE_MAP(CRealShotView, CDockablePaneChildView)
	ON_BN_CLICKED(IDOK, &CRealShotView::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CRealShotView::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_CALIBRATION, &CRealShotView::OnBnClickedButtonCalibration)
	ON_BN_CLICKED(IDC_BUTTON_START, &CRealShotView::OnBnClickedButtonStart)
END_MESSAGE_MAP()


// CRealShotView message handlers


void CRealShotView::OnBnClickedOk()
{
	//CDockablePaneChildView::OnOK();
}


void CRealShotView::OnBnClickedCancel()
{
	//CDockablePaneChildView::OnCancel();
}


void CRealShotView::Update(const float deltaSeconds)
{
	// F12키를 누르면 종료된다.
	if (GetAsyncKeyState(VK_F12))
	{
		if (m_isStart)
		{
			OnBnClickedButtonStart();
		}
	}
}


void CRealShotView::OnBnClickedButtonCalibration()
{
	cLauncherConfig config("../media/realshot/launcher_config.cfg");
	const string calibrationExeDir = common::GetFilePathExceptFileName(config.m_cameraCalibrationExePath);
	const string calibrationExeName = common::GetFileName(config.m_cameraCalibrationExePath);

	ShellExecuteA(NULL, "open", calibrationExeName.c_str(), "0", calibrationExeDir.c_str(), SW_SHOW);
}


void CRealShotView::OnBnClickedButtonStart()
{
	if (m_isStart)
	{
		cRealShotController::Get()->StopMotionSim();
		m_StartButton.SetWindowTextW(L"Start");

		m_isStart = false;
		SetBackgroundColor(g_grayColor);
	}
	else
	{
		cRealShotController::Get()->StartMotionSim("../media/realshot/motionmonitor_realshot.json", false);
		m_StartButton.SetWindowTextW(L"Stop");

		m_isStart = true;
		SetBackgroundColor(g_blueColor);
	}
}
