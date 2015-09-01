// LauncherView.cpp : implementation file
//

#include "stdafx.h"
#include "MotionMonitor.h"
#include "LauncherView.h"
#include "afxdialogex.h"


// CLauncherView dialog
CLauncherView::CLauncherView(CWnd* pParent /*=NULL*/)
	: CDockablePaneChildView(CLauncherView::IDD, pParent)
	, m_CamSensEdit(0)
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
	DDX_Text(pDX, IDC_EDIT_CAM_SENS, m_CamSensEdit);
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

	m_CamSensSlider.SetRange(0, 1000);

	return TRUE;
}


void CLauncherView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePaneChildView::OnSize(nType, cx, cy);
}


// 프로그램을 시작한다.
void CLauncherView::OnBnClickedButtonGamestart()
{
	const string cameraExeDir = "D:\\Project\\LaserRecognition2\\Source\\Bin\\";
	const string cameraExePath = "MachineGunCamera.exe";
	const string gameExePath = "aa.exe";

	// 프로그램이 이미 실행 중이라면, 종료하고, 다시 실행한다.

	ShellExecuteA(NULL, "open", cameraExePath.c_str(), NULL, cameraExeDir.c_str(), SW_SHOW);
}


void CLauncherView::OnBnClickedButtonPlayer1()
{
	// TODO: Add your control notification handler code here
}


void CLauncherView::OnBnClickedButtonPlayer2()
{
	// TODO: Add your control notification handler code here
}


void CLauncherView::OnBnClickedButtonCamAdjustmentP1()
{
	// TODO: Add your control notification handler code here
}


void CLauncherView::OnBnClickedButtonCamAdjustmentP2()
{
	// TODO: Add your control notification handler code here
}


void CLauncherView::OnNMCustomdrawSliderCamSens(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	*pResult = 0;

	m_CamSensEdit = (float)m_CamSensSlider.GetPos();
	UpdateData(FALSE);
}
