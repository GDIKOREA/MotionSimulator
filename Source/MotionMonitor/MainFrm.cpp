// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "MotionMonitor.h"

#include "MotionOutputView.h"
#include "UDPInputView.h"
#include "JoystickView.h"
#include "MotionWaveView.h"
#include "MixingView.h"
#include "UDPParseView.h"
#include "ControlBoard.h"
#include "PlotView.h"
#include "LauncherView.h"
#include "MainFrm.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWndEx)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

CMotionWaveView *g_mwaveView = NULL;
CUDPInputView *g_udpInputView = NULL;
CControlBoard *g_controlView = NULL;


#define CREATE_DOCKPANE(CLASS, DOCKNAME, PANE_ID, VAR)\
{\
	VAR = new CDockablePaneBase();\
	if (!VAR->Create(DOCKNAME, this, CRect(0, 0, 460, 500), TRUE, PANE_ID, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))\
	{\
		TRACE0("Failed to create pane window\n");\
		return FALSE;\
	}\
	CLASS *view = new CLASS(VAR);\
	view->Create(CLASS::IDD, VAR);\
	view->ShowWindow(SW_SHOW);\
	VAR->SetChildView(view);\
	m_viewList.push_back(VAR);\
	HICON hClassViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(theApp.m_bHiColorIcons ? IDI_CLASS_VIEW_HC : IDI_CLASS_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);\
	VAR->SetIcon(hClassViewIcon, FALSE);\
}


// 일반 뷰 생성
#define CREATE_PANE(CLASS, VAR, SHOWCMD) \
	CLASS *VAR = new CLASS(this);\
	VAR->Create(CLASS::IDD, this);\
	VAR->ShowWindow(SHOWCMD);



BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
	ON_WM_CREATE()
	ON_REGISTERED_MESSAGE(AFX_WM_CREATETOOLBAR, &CMainFrame::OnToolbarCreateNew)
	ON_COMMAND(ID_VIEW_VIEWINITIALIZE, &CMainFrame::OnViewViewinitialize)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame construction/destruction

CMainFrame::CMainFrame() 
	:m_wndCube3DView(NULL)
, m_udpInputView(NULL)
, m_udpParseView(NULL)
, m_motionOutputView(NULL)
, m_joystickView(NULL)
, m_motionWaveView(NULL)
, m_mixingView(NULL)
, m_controlBoardView(NULL)
, m_plotView(NULL)
, m_launcherView(NULL)
{
	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_VS_2008);
}

CMainFrame::~CMainFrame()
{
	for each (auto &pane in m_viewList)
		delete pane;
	m_viewList.clear();
}

void CMainFrame::OnClose()
{
	UpdateConfig(false);
	cMotionController::Get()->m_config.WriteConfigFile(cMotionController::Get()->m_config.m_fileName);
	CFrameWndEx::OnClose();
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// View 가 화면에 나오지 않을 때, 리셋시키기 위한 환경파일을 검사한다. 
	using namespace std;
	ifstream ifs("view.cfg");
	if (ifs.is_open())
	{
		int n;
		ifs >> n;
		if (n > 0)
		{
			GetDockingManager()->DisableRestoreDockState();
			CDockingManager::SetDockingMode(DT_IMMEDIATE);

			ifs.close();

			// 복구
			ofstream ofs("view.cfg");
			if (ofs.is_open())
				ofs << 0 << endl;
		}
	}


	const cMotionMonitorConfig &config = cMotionController::Get()->m_config;

	// 머신건 스탠드 버젼은 심플하게 간다.
	if (config.m_mode == "machinegun_stand")
	{
		SetMenu(NULL);
	}
	else
	{
		if (!m_wndMenuBar.Create(this))
		{
			TRACE0("Failed to create menubar\n");
			return -1;      // fail to create
		}
		m_wndMenuBar.SetPaneStyle(m_wndMenuBar.GetPaneStyle() | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY);
		// prevent the menu bar from taking the focus on activation
		CMFCPopupMenu::SetForceMenuFocus(FALSE);
		m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
		EnableDocking(CBRS_ALIGN_ANY);
		DockPane(&m_wndMenuBar);
	}


	CString strCustomize;
	BOOL bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);

	// enable Visual Studio 2005 style docking window behavior
	CDockingManager::SetDockingMode(DT_SMART);
	// enable Visual Studio 2005 style docking window auto-hide behavior
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// Load menu item image (not placed on any standard toolbars):
	CMFCToolBar::AddToolBarForImageCollection(IDR_MENU_IMAGES, theApp.m_bHiColorIcons ? IDB_MENU_IMAGES_24 : 0);

	// create docking windows
	if (!CreateDockingWindows())
	{
		TRACE0("Failed to create docking windows\n");
		return -1;
	}


	if (!m_viewList.empty())
	{
 		for each (auto &view in m_viewList)
 			view->EnableDocking(CBRS_ALIGN_ANY);

		CDockablePaneBase *parentPane = m_viewList.front();

		DockPane(parentPane);

		CDockablePane* pTabbedBar = NULL;
		for each (auto &view in m_viewList)
		{
			if (view != parentPane)
				view->AttachToTabWnd(parentPane, DM_SHOW, TRUE, &pTabbedBar);
		}

		if (config.m_mode == "machinegun_stand")
		{
			for each (auto &view in m_viewList)
			{
				//view->SetDockingMode()
				//view->ShowWindow(SW_HIDE);
			}
		}
	}

	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2008));
	CDockingManager::SetDockingMode(DT_SMART);
	RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);
	theApp.WriteInt(_T("ApplicationLook"), theApp.m_nAppLook);


	// Enable toolbar and docking window menu replacement
	EnablePaneMenu(TRUE, ID_VIEW_CUSTOMIZE, strCustomize, ID_VIEW_TOOLBAR);

	// enable quick (Alt+drag) toolbar customization
	CMFCToolBar::EnableQuickCustomization();

	if (CMFCToolBar::GetUserImages() == NULL)
	{
		// load user-defined toolbar images
		if (m_UserImages.Load(_T(".\\UserImages.bmp")))
		{
			CMFCToolBar::SetUserImages(&m_UserImages);
		}
	}

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs


	const cMotionMonitorConfig &config = cMotionController::Get()->m_config;
	if (config.m_mode == "machinegun_stand")
	{
		// 사이즈를 줄이거나 늘리지 못하게 한다.
		cs.style = WS_OVERLAPPED | WS_SYSMENU | WS_BORDER;
	}

	return TRUE;
}

BOOL CMainFrame::CreateDockingWindows()
{
	cMotionMonitorConfig &config = cMotionController::Get()->m_config;

	// Create class view
	CString strClassView;
	BOOL bNameValid = strClassView.LoadString(IDS_CLASS_VIEW);
	ASSERT(bNameValid);


	if (config.m_mode == "machinegun_stand")
	{
		CREATE_PANE(CLauncherView, view, SW_SHOW);
		CREATE_PANE(CUDPInputView, m_udpInputView, SW_HIDE);
		CREATE_PANE(CMixingView, m_mixingView, SW_HIDE);
		CREATE_PANE(CUDPParseView, m_udpParseView, SW_HIDE);
		CREATE_PANE(CPlotView, m_plotView, SW_HIDE);
		CREATE_PANE(CControlBoard, m_controlBoardView, SW_HIDE);

		g_udpInputView = (CUDPInputView *)m_udpInputView;
		g_controlView = (CControlBoard*)m_controlBoardView;
	}
	else
	{
		// Create cube3d view
		m_wndCube3DView = new CCube3DPane();
		if (!m_wndCube3DView->Create(L"3DView", this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_CUBE3D, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
		{
			TRACE0("Failed to create cube 3d View window\n");
			return FALSE; // failed to create
		}
		HICON hClassViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(theApp.m_bHiColorIcons ? IDI_CLASS_VIEW_HC : IDI_CLASS_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
		m_wndCube3DView->SetIcon(hClassViewIcon, FALSE);
		m_viewList.push_back(m_wndCube3DView);

		CREATE_DOCKPANE(CMotionOutputView, L"Motion Output View", ID_VIEW_MOTION_OUTPUT, m_motionOutputView);
		CREATE_DOCKPANE(CUDPInputView, L"UDP Input View", ID_VIEW_MOTION_INPUT, m_udpInputView);
		CREATE_DOCKPANE(CJoystickView, L"Joystick View", ID_VIEW_JOYSTICK, m_joystickView);
		CREATE_DOCKPANE(CMotionWaveView, L"MotionWave View", ID_VIEW_MOTIONWAVE, m_motionWaveView);
		CREATE_DOCKPANE(CMixingView, L"Mixing View", ID_VIEW_MIXING, m_mixingView);
		CREATE_DOCKPANE(CControlBoard, L"Control Board", ID_VIEW_CONTROLBOARD, m_controlBoardView);
		CREATE_DOCKPANE(CUDPParseView, L"UDP Parse View", ID_VIEW_UDPPARSE, m_udpParseView);
		CREATE_DOCKPANE(CPlotView, L"Plot View", ID_VIEW_PLOT, m_plotView);
		CREATE_DOCKPANE(CLauncherView, L"Launcher View", ID_VIEW_LAUNCHER, m_launcherView);

		g_mwaveView = (CMotionWaveView*)m_motionWaveView->GetChildView();
		g_udpInputView = (CUDPInputView *)m_udpInputView->GetChildView();
		g_controlView = (CControlBoard*)m_controlBoardView->GetChildView();
	}

	return TRUE;
}

void CMainFrame::SetDockingWindowIcons(BOOL bHiColorIcons)
{
//   	HICON hClassViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_CLASS_VIEW_HC : IDI_CLASS_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
//  	m_wndCube3DView->SetIcon(hClassViewIcon, FALSE);
}

// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// CMainFrame message handlers

// void CMainFrame::OnViewCustomize()
// {
// 	CMFCToolBarsCustomizeDialog* pDlgCust = new CMFCToolBarsCustomizeDialog(this, TRUE /* scan menus */);
// 	pDlgCust->EnableUserDefinedToolbars();
// 	pDlgCust->Create();
// }

LRESULT CMainFrame::OnToolbarCreateNew(WPARAM wp,LPARAM lp)
{
	LRESULT lres = CFrameWndEx::OnToolbarCreateNew(wp,lp);
	if (lres == 0)
	{
		return 0;
	}

	CMFCToolBar* pUserToolbar = (CMFCToolBar*)lres;
	ASSERT_VALID(pUserToolbar);

	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
	return lres;
}


BOOL CMainFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext) 
{
	// base class does the real work

	if (!CFrameWndEx::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext))
	{
		return FALSE;
	}


	// enable customization button for all user toolbars
	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	for (int i = 0; i < iMaxUserToolbars; i ++)
	{
		CMFCToolBar* pUserToolbar = GetUserToolBarByIndex(i);
		if (pUserToolbar != NULL)
		{
			pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
		}
	}

	return TRUE;
}


void CMainFrame::OnViewViewinitialize()
{
	using namespace std;
	ofstream ofs("view.cfg");
	if (!ofs.is_open())
		return;
	ofs << 1 << endl;
}


// 뷰 전체에 UpdateConfig() 함수를 호출한다.
void CMainFrame::UpdateConfig(bool IsSaveAndValidate) //IsSaveAndValidate=true
{
	for each (auto &view in m_viewList)
		view->GetChildView()->UpdateConfig(IsSaveAndValidate);
}


// 환경설정 파일을 읽고, 뷰를 업데이트 한다.
void CMainFrame::LoadConfigFile(const string &fileName)
{
	cMotionMonitorConfig &config = cMotionController::Get()->m_config;

	UpdateConfig(false);
	config.WriteConfigFile(config.m_fileName);

	if (config.ReadConfigFile(fileName))
		UpdateConfig();

	SetWindowTextW(str2wstr(GetFileName(fileName)).c_str());
}


BOOL CMainFrame::NewPlotWindow()
{
	static int plotViewId = 50000;
	static int plotViewIncId = 2;
	CDockablePaneBase *plotView;

	CString viewName;
	viewName.Format(L"Plot View%d", plotViewIncId++);
	CREATE_DOCKPANE(CPlotView, viewName, plotViewId++, plotView);

	plotView->EnableDocking(CBRS_ALIGN_ANY);
	CDockablePane* pTabbedBar = NULL;
	plotView->AttachToTabWnd(m_plotView, DM_SHOW, TRUE, &pTabbedBar);

	((CPlotView*)plotView->GetChildView())->SetAddPlotView(true);

	return TRUE;
}

