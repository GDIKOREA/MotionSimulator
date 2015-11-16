// LauncherView.cpp : implementation file
//

#include "stdafx.h"
#include "MotionMonitor.h"
#include "LauncherView.h"
#include "afxdialogex.h"
#include "LauncherConfig.h"
#include "MainFrm.h"
#include "MachineGunController.h"
#include "ControlBoard.h"


void GameExeTerminate(int id, void*arg);

// CLauncherView dialog
CLauncherView::CLauncherView(CWnd* pParent /*=NULL*/)
	: CDockablePaneChildView(CLauncherView::IDD, pParent)
	, m_CamSens1Edit(0)
	, m_CamSens2Edit(0)
	, m_camSensitiveMid(2.3f)
	, m_isGameExeTerminate(false)
	, m_Credit(0)
	, m_Coin(0)
	, m_CoinPerGame(2)
	, m_GameCount(0)
	, m_CreditCount(0)
	, m_titleImage(common::str2wstr("../media/machinegun/MGX_White.png").c_str())
{
}

CLauncherView::~CLauncherView()
{
	m_gameExeTracker.Termaniate();
	m_cameraExeTracker.Termaniate();
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
	DDX_Text(pDX, IDC_EDIT_CREDIT, m_Credit);
	DDX_Text(pDX, IDC_EDIT_COIN, m_Coin);
	DDX_Text(pDX, IDC_EDIT_COINPERGAME, m_CoinPerGame);
	DDX_Control(pDX, IDC_BUTTON_GAMESTART, m_StartButton);
	DDX_Text(pDX, IDC_STATIC_GAME_COUNT, m_GameCount);
	DDX_Text(pDX, IDC_STATIC_CREDIT_COUNT, m_CreditCount);
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
	ON_EN_CHANGE(IDC_EDIT_CREDIT, &CLauncherView::OnEnChangeEditCredit)
	ON_EN_CHANGE(IDC_EDIT_COIN, &CLauncherView::OnEnChangeEditCoin)
	ON_EN_CHANGE(IDC_EDIT_COINPERGAME, &CLauncherView::OnEnChangeEditCoinpergame)
	ON_BN_CLICKED(IDC_BUTTON_BOARDCHECK, &CLauncherView::OnBnClickedButtonBoardcheck)
	ON_WM_PAINT()
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

	m_DifficultCombo.AddString(L"Easy");
	m_DifficultCombo.AddString(L"Normal");
	m_DifficultCombo.AddString(L"Hard");
	m_DifficultCombo.SetCurSel(1);

	m_DifficultCombo2.AddString(L"Easy");
	m_DifficultCombo2.AddString(L"Normal");
	m_DifficultCombo2.AddString(L"Hard");
	m_DifficultCombo2.SetCurSel(1);

	m_CamSensSlider.SetRange(0, 2000);
	m_CamSens2Slider.SetRange(0, 2000);

	// ī�޶� ���� ���� �о�´�.
	// �߰� �� = 2.3
	ReadCameraSensitive("camera contour1.cfg", m_camSens1);
	ReadCameraSensitive("camera contour2.cfg", m_camSens2);

	const int sliderPos1 = (int)((m_camSens1 - 2.3f) * 1000.f);
	const int sliderPos2 = (int)((m_camSens2 - 2.3f) * 1000.f);
	m_CamSensSlider.SetPos(sliderPos1 + 1000);
	m_CamSens2Slider.SetPos(sliderPos2 + 1000);

	//SetBackgroundColor(RGB(8, 8, 8));


// 	const wstring wfileName = common::str2wstr("../media/machinegun/MGX_White.png");
// 	m_titleImage.FromFile(wfileName.c_str());
// 	Gdiplus::Bitmap bmp(wfileName.c_str());
// 	if (Gdiplus::Ok != bmp.GetLastStatus())
// 		return false;


	return TRUE;
}


// ��ü�� ��� ������ ��, ȣ��ȴ�.
// �ӽŰ� ��Ʈ�ѷ� �ʱ�ȭ
bool CLauncherView::Init()
{

	const cMotionMonitorConfig &config = cMotionController::Get()->m_config;
	if ((config.m_mode == "machinegun_stand") ||
		(config.m_mode == "machinegun_stand_debug"))
	{
		// UDP ��� �ʱ�ȭ
		cMachineGunController::Get()->Init();
		// ���� Ŭ���̾�Ʈ�� UDP ��� ����
		g_controlView->Start();
		//cMachineGunController::Get()->StartMotionSim(config.m_fileName, false);
	}

	return true;
}


void CLauncherView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePaneChildView::OnSize(nType, cx, cy);
}


// ���α׷��� �����Ѵ�.
void CLauncherView::OnBnClickedButtonGamestart()
{
	UpdateData();

	// ������ �ɼ��� ������ ��, ������ �����Ѵ�.
	WriteCameraSensitive("camera contour1.cfg", m_camSens1);
	WriteCameraSensitive("camera contour2.cfg", m_camSens2);

	cLauncherConfig launcherConfig("../media/machinegun/launcher_config.cfg");

	// �ӽŰ� ī�޶� ���α׷� ����
	m_cameraExeTracker.Execute(launcherConfig.m_cameraExePath, "", 1, GameExeTerminate, this);


	// �ӽŰ� ���� ����
	stringstream ss;
	ss << m_DifficultCombo.GetCurSel() << " ";
	ss << m_DifficultCombo2.GetCurSel() << " ";
	ss << 1920 << " " << 1080; // �ػ�
	const string commandLine = ss.str();	
	m_gameExeTracker.Execute(launcherConfig.m_gameExePath, commandLine, 0, GameExeTerminate, this);
	

	// �Ŵ��� ���α׷��� Ÿ��Ʋ�ٷ� �̵��Ѵ�.
	::ShowWindow(::AfxGetMainWnd()->m_hWnd, SW_MINIMIZE);
}


void CLauncherView::OnBnClickedButtonPlayer1()
{
	cLauncherConfig config("../media/machinegun/launcher_config.cfg");
	const string exeDir = common::GetFilePathExceptFileName(config.m_cameraCheckExePath);
	const string exeName = common::GetFileName(config.m_cameraCheckExePath);

	ShellExecuteA(NULL, "open", exeName.c_str(), "0", exeDir.c_str(), SW_SHOW);
}


void CLauncherView::OnBnClickedButtonPlayer2()
{
	cLauncherConfig config("../media/machinegun/launcher_config.cfg");
	const string exeDir = common::GetFilePathExceptFileName(config.m_cameraCheckExePath);
	const string exeName = common::GetFileName(config.m_cameraCheckExePath);

	ShellExecuteA(NULL, "open", exeName.c_str(), "1", exeDir.c_str(), SW_SHOW);
}


void CLauncherView::OnBnClickedButtonCamAdjustmentP1()
{
	cLauncherConfig config("../media/machinegun/launcher_config.cfg");
	const string calibrationExeDir = common::GetFilePathExceptFileName(config.m_cameraCalibrationExePath);
	const string calibrationExeName = common::GetFileName(config.m_cameraCalibrationExePath);

	ShellExecuteA(NULL, "open", calibrationExeName.c_str(), "0", calibrationExeDir.c_str(), SW_SHOW);
}


void CLauncherView::OnBnClickedButtonCamAdjustmentP2()
{
	cLauncherConfig config("../media/machinegun/launcher_config.cfg");
	const string calibrationExeDir = common::GetFilePathExceptFileName(config.m_cameraCalibrationExePath);
	const string calibrationExeName = common::GetFileName(config.m_cameraCalibrationExePath);

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


// �������� ���� ȣ��ȴ�.
// ������ ����Ǹ�, �����츦 Ȱ��ȭ ��Ų��.
void CLauncherView::Update(const float deltaSeconds)
{
	//  ������ ����� ��, �̺�Ʈ ó��
	if (m_isGameExeTerminate)
	{
		m_isGameExeTerminate = false;
		::ShowWindow(::AfxGetMainWnd()->m_hWnd, SW_RESTORE);
	}
	
}


// ���� Ŭ���̾�Ʈ�� ����Ǹ�, ȣ��ȴ�.
void GameExeTerminate(int id, void*arg)
{
	CLauncherView *launcherView = (CLauncherView*)arg;
	launcherView->m_isGameExeTerminate = true;
}


void CLauncherView::OnEnChangeEditCredit()
{
	UpdateData();

	cMachineGunController::Get()->m_credit = m_Credit;
}


void CLauncherView::OnEnChangeEditCoin()
{
	UpdateData();

	cMachineGunController::Get()->m_coin = m_Coin;
}


void CLauncherView::OnEnChangeEditCoinpergame()
{
	UpdateData();
	if (m_CoinPerGame <= 0)
	{
		m_CoinPerGame = 10;
		UpdateData(FALSE);
	}

	cMachineGunController::Get()->m_coinPerGame = m_CoinPerGame;
}


void CLauncherView::UpdateCoin(const int credit, const int coin, const int coinPerGame)
{
	m_Credit = credit;
	m_Coin = coin;
	m_CoinPerGame = coinPerGame;
	UpdateData(FALSE);
}


void CLauncherView::UpdateGameInfo(const int creditCount, const int gameCount)
{
	m_CreditCount = creditCount;
	m_GameCount = gameCount;
	UpdateData(FALSE);
}


void CLauncherView::OnBnClickedButtonBoardcheck()
{
	cLauncherConfig config("../media/machinegun/launcher_config.cfg");
	const string exeDir = common::GetFilePathExceptFileName(config.m_boardCheckExePath);
	const string exeName = common::GetFileName(config.m_boardCheckExePath);

	// ���κ���� ���Ϳ� UDP�� ���Ͻ÷� ������ ��Ʈ�� ���α׷��� ����.
	const string fileName = "../media/machinegun/mg_controller.ini";
	const int proxyPort = uiutil::GetProfileInt("MainBoard UDP", "ProxyPort", 10000, fileName);

	ShellExecuteA(NULL, "open", exeName.c_str(), format("%d", proxyPort).c_str(), exeDir.c_str(), SW_SHOW);
}


void CLauncherView::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	// Ÿ��Ʋ �̹��� ���
	Gdiplus::Graphics graphic(dc);
	graphic.DrawImage(&m_titleImage, Gdiplus::Rect(80, 0, m_titleImage.GetWidth(), m_titleImage.GetHeight()));
}