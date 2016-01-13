// RealShotView.cpp : implementation file
//

#include "stdafx.h"
#include "MotionMonitor.h"
#include "RealShotView.h"
#include "afxdialogex.h"
#include "RealshotController.h"


void CameraExeTerminate(int id, void*arg);


// CRealShotView dialog

CRealShotView::CRealShotView(CWnd* pParent /*=NULL*/)
: CDockablePaneChildView(CRealShotView::IDD, pParent)
, m_isStart(false)
, m_isCameraExeTerminate(false)
{
}

CRealShotView::~CRealShotView()
{
}

void CRealShotView::DoDataExchange(CDataExchange* pDX)
{
	CDockablePaneChildView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_START, m_StartButton);
	DDX_Control(pDX, IDC_STATIC1, m_PrintStr);
	DDX_Control(pDX, IDC_BUTTON_CALIBRATION, m_CalibrationButton);
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


BOOL CRealShotView::OnInitDialog()
{
	CDockablePaneChildView::OnInitDialog();

	m_StartButton.MoveWindow(CRect(25, 25, 300, 125));
	m_CalibrationButton.MoveWindow(CRect(25, 150, 300, 250));

	return TRUE;
}


void CRealShotView::Update(const float deltaSeconds)
{
	// F12Ű�� ������ ����ȴ�.
	if (GetAsyncKeyState(VK_F12))
	{
		if (m_isStart)
			OnBnClickedButtonStart();
	}

	if (m_isCameraExeTerminate)
	{
		if (m_isStart)
			OnBnClickedButtonStart();

		m_isCameraExeTerminate = false;
		m_cameraExeTracker.Termaniate(); // ī�޶� ���α׷� �����Ѵ�.
		::ShowWindow(::AfxGetMainWnd()->m_hWnd, SW_RESTORE);
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
		m_cameraExeTracker.Termaniate(); // ī�޶� ���α׷� �����Ѵ�.

		m_isStart = false;
		SetBackgroundColor(g_grayColor);
	}
	else
	{
		cLauncherConfig launcherConfig("../media/realshot/launcher_config.cfg");

		// �ӽŰ� ī�޶� ���α׷� ����
		m_cameraExeTracker.Execute(launcherConfig.m_cameraExePath, "", 1, CameraExeTerminate, this);

		cRealShotController::Get()->StartMotionSim("../media/realshot/motionmonitor_realshot.json", false);
		m_StartButton.SetWindowTextW(L"Stop (F12)");

		m_isStart = true;
		SetBackgroundColor(g_blueColor);
	}
}


// ���� Ŭ���̾�Ʈ�� ����Ǹ�, ȣ��ȴ�.
void CameraExeTerminate(int id, void*arg)
{
	CRealShotView *realshot = (CRealShotView*)arg;
	realshot->m_isCameraExeTerminate = true;
}

