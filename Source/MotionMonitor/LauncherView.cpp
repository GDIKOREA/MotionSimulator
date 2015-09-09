// LauncherView.cpp : implementation file
//

#include "stdafx.h"
#include "MotionMonitor.h"
#include "LauncherView.h"
#include "afxdialogex.h"
#include "LauncherConfig.h"


void GameExeTerminate(int id, void*arg);

// CLauncherView dialog
CLauncherView::CLauncherView(CWnd* pParent /*=NULL*/)
	: CDockablePaneChildView(CLauncherView::IDD, pParent)
	, m_CamSens1Edit(0)
	, m_CamSens2Edit(0)
	, m_camSensitiveMid(2.3f)
{
}

CLauncherView::~CLauncherView()
{
}

void CLauncherView::DoDataExchange(CDataExchange* pDX)
{
	CDockablePaneChildView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_DIFFICULT, m_DifficultCombo);
	DDX_Control(pDX, IDC_SLIDER_CAM_SENS, m_CamSensSlider);
	DDX_Text(pDX, IDC_EDIT_CAM_SENS, m_CamSens1Edit);
	DDX_Text(pDX, IDC_EDIT_CAM_SENS2, m_CamSens2Edit);
	DDX_Control(pDX, IDC_SLIDER_CAM_SENS2, m_CamSens2Slider);
	DDX_Control(pDX, IDC_COMBO_DIFFICULT2, m_DifficultCombo2);
}


BEGIN_MESSAGE_MAP(CLauncherView, CDockablePaneChildView)
	ON_BN_CLICKED(IDOK, &CLauncherView::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CLauncherView::OnBnClickedCancel)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_GAMESTART, &CLauncherView::OnBnClickedButtonGamestart)
	ON_BN_CLICKED(IDC_BUTTON_PLAYER1, &CLauncherView::OnBnClickedButtonPlayer1)
	ON_BN_CLICKED(IDC_BUTTON_PLAYER2, &CLauncherView::OnBnClickedButtonPlayer2)
	ON_BN_CLICKED(IDC_BUTTON_CAM_ADJUSTMENT_P1, &CLauncherView::OnBnClickedButtonCamAdjustmentP1)
	ON_BN_CLICKED(IDC_BUTTON_CAM_ADJUSTMENT_P2, &CLauncherView::OnBnClickedButtonCamAdjustmentP2)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_CAM_SENS, &CLauncherView::OnNMCustomdrawSliderCamSens)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_CAM_SENS2, &CLauncherView::OnNMCustomdrawSliderCamSens2)
END_MESSAGE_MAP()


// CLauncherView message handlers

void CLauncherView::OnBnClickedOk()
{
}


void CLauncherView::OnBnClickedCancel()
{
}


BOOL CLauncherView::OnInitDialog()
{
	CDockablePaneChildView::OnInitDialog();

	m_DifficultCombo.AddString(L"1");
	m_DifficultCombo.AddString(L"2");
	m_DifficultCombo.AddString(L"3");
	m_DifficultCombo.SetCurSel(0);

	m_DifficultCombo2.AddString(L"1");
	m_DifficultCombo2.AddString(L"2");
	m_DifficultCombo2.AddString(L"3");
	m_DifficultCombo2.SetCurSel(0);

	m_CamSensSlider.SetRange(0, 2000);
	m_CamSens2Slider.SetRange(0, 2000);

	// 카메라 감도 값을 읽어온다.
	// 중간 값 = 2.3
	ReadCameraSensitive("camera contour1.cfg", m_camSens1);
	ReadCameraSensitive("camera contour2.cfg", m_camSens2);

	const int sliderPos1 = (int)((m_camSens1 - 2.3f) * 1000.f);
	const int sliderPos2 = (int)((m_camSens2 - 2.3f) * 1000.f);
	m_CamSensSlider.SetPos(sliderPos1 + 1000);
	m_CamSens2Slider.SetPos(sliderPos2 + 1000);

	return TRUE;
}


void CLauncherView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePaneChildView::OnSize(nType, cx, cy);
}


// 프로그램을 시작한다.
void CLauncherView::OnBnClickedButtonGamestart()
{
	// 설정한 옵션을 저장한 후, 게임을 실행한다.
	WriteCameraSensitive("camera contour1.cfg", m_camSens1);
	WriteCameraSensitive("camera contour2.cfg", m_camSens2);


	const string cameraExeDir = "D:\\Project\\LaserRecognition2\\Source\\Bin\\";
	const string cameraExePath = "MachineGunCamera.exe";
	const string gameExePath = "aa.exe";

	// 프로그램이 이미 실행 중이라면, 종료하고, 다시 실행한다.
//	ShellExecuteA(NULL, "open", cameraExePath.c_str(), NULL, cameraExeDir.c_str(), SW_SHOW);


	cLauncherConfig config("../media/machine gun/launcher_config.cfg");
// 	const string gameExeDir = common::GetFilePathExceptFileName(config.m_gameExePath) + "\\";
// 	const string gameExeName = common::GetFileName(config.m_gameExePath);
	// 프로그램이 이미 실행 중이라면, 종료하고, 다시 실행한다.
	//ShellExecuteA(NULL, "open", gameExeName.c_str(), "1 2 1920 1080", gameExeDir.c_str(), SW_SHOW);

	m_gameExeTracker.Execute(config.m_gameExePath, 0, GameExeTerminate, this);
}


void CLauncherView::OnBnClickedButtonPlayer1()
{
	cLauncherConfig config("../media/machine gun/launcher_config.cfg");
	const string exeDir = common::GetFilePathExceptFileName(config.m_cameraCheckExePath);
	const string exeName = common::GetFileName(config.m_cameraCheckExePath);

	// 프로그램이 이미 실행 중이라면, 종료하고, 다시 실행한다.
	ShellExecuteA(NULL, "open", exeName.c_str(), "0", exeDir.c_str(), SW_SHOW);
}


void CLauncherView::OnBnClickedButtonPlayer2()
{
	cLauncherConfig config("../media/machine gun/launcher_config.cfg");
	const string exeDir = common::GetFilePathExceptFileName(config.m_cameraCheckExePath);
	const string exeName = common::GetFileName(config.m_cameraCheckExePath);

	// 프로그램이 이미 실행 중이라면, 종료하고, 다시 실행한다.
	ShellExecuteA(NULL, "open", exeName.c_str(), "1", exeDir.c_str(), SW_SHOW);
}


void CLauncherView::OnBnClickedButtonCamAdjustmentP1()
{
	cLauncherConfig config("../media/machine gun/launcher_config.cfg");
	const string calibrationExeDir = common::GetFilePathExceptFileName(config.m_cameraCalibrationExePath);
	const string calibrationExeName = common::GetFileName(config.m_cameraCalibrationExePath);

	// 프로그램이 이미 실행 중이라면, 종료하고, 다시 실행한다.
	ShellExecuteA(NULL, "open", calibrationExeName.c_str(), "0", calibrationExeDir.c_str(), SW_SHOW);
}


void CLauncherView::OnBnClickedButtonCamAdjustmentP2()
{
	cLauncherConfig config("../media/machine gun/launcher_config.cfg");
	const string calibrationExeDir = common::GetFilePathExceptFileName(config.m_cameraCalibrationExePath);
	const string calibrationExeName = common::GetFileName(config.m_cameraCalibrationExePath);

	// 프로그램이 이미 실행 중이라면, 종료하고, 다시 실행한다.
	ShellExecuteA(NULL, "open", calibrationExeName.c_str(), "1", calibrationExeDir.c_str(), SW_SHOW);
}


void CLauncherView::OnNMCustomdrawSliderCamSens(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	*pResult = 0;

	const int pos = m_CamSensSlider.GetPos();
	m_camSens1 = m_camSensitiveMid + ((float)(pos-1000) * 0.001f);
	m_CamSens1Edit = pos * 0.001f;

	UpdateData(FALSE);
}


void CLauncherView::OnNMCustomdrawSliderCamSens2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	*pResult = 0;

	const int pos = m_CamSens2Slider.GetPos();
	m_camSens2 = m_camSensitiveMid + ((float)(pos - 1000) * 0.001f);
	m_CamSens2Edit = pos * 0.001f;

	UpdateData(FALSE);
}


// 게임 클라이언트가 종료되면, 호출된다.
void GameExeTerminate(int id, void*arg)
{
	CLauncherView *launcherView = (CLauncherView*)arg;

}

