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
#include "MachineGunController.h"

#pragma comment(lib, "winmm.lib")


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
CLauncherView *g_launcherView = NULL;


#define CREATE_DOCKPANE(CLASS, DOCKNAME, PANE_ID, RESOURCE_ID, VAR)\
{\
	CDockablePaneBase *pane = new CDockablePaneBase();\
	if (!pane->Create(DOCKNAME, this, CRect(0, 0, 460, 500), TRUE, PANE_ID, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))\
		{\
		TRACE0("Failed to create pane window\n");\
		return FALSE;\
		}\
	VAR = new CLASS(pane);\
	VAR->Create(RESOURCE_ID, pane);\
	VAR->ShowWindow(SW_SHOW);\
	pane->SetChildView(VAR);\
	m_viewList.push_back(pane);\
	HICON hClassViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(theApp.m_bHiColorIcons ? IDI_CLASS_VIEW_HC : IDI_CLASS_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);\
	pane->SetIcon(hClassViewIcon, FALSE);\
}


#define CREATE_DOCKPANE2(CLASS, DOCKNAME, PANE_ID, VAR) \
	CREATE_DOCKPANE(CLASS, DOCKNAME, PANE_ID, CLASS::IDD, VAR)



// �Ϲ� �� ����
#define CREATE_PANE(CLASS, VAR, SHOWCMD) \
	VAR = new CLASS(this);\
	VAR->Create(CLASS::IDD, this);\
	m_childViewList.push_back(VAR);\
	VAR->ShowWindow(SHOWCMD);\



BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
	ON_WM_CREATE()
	ON_REGISTERED_MESSAGE(AFX_WM_CREATETOOLBAR, &CMainFrame::OnToolbarCreateNew)
	ON_COMMAND(ID_VIEW_VIEWINITIALIZE, &CMainFrame::OnViewViewinitialize)
	ON_WM_CLOSE()
	ON_WM_SIZE()
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
: m_wndCube3DView(NULL)
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
	if (m_viewList.empty())
	{
		for each (auto &view in m_childViewList)
			delete view;
		m_childViewList.clear();
	}
	else
	{
		for each (auto &pane in m_viewList)
			delete pane;
		m_viewList.clear();
	}
}

void CMainFrame::OnClose()
{
	UpdateConfig(false);
	cMotionController::Get()->m_config.WriteConfigFile(cMotionController::Get()->m_config.m_fileName);
	CFrameWndEx::OnClose();
}


// ������������ ��� �����ǰ� �� ��, App���� ȣ�� �ȴ�.
void CMainFrame::Init()
{
	if (m_launcherView)
	{
		m_launcherView->Init();
	}
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// View �� ȭ�鿡 ������ ���� ��, ���½�Ű�� ���� ȯ�������� �˻��Ѵ�. 
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

			// ����
			ofstream ofs("view.cfg");
			if (ofs.is_open())
				ofs << 0 << endl;
		}
	}


	const cMotionMonitorConfig &config = cMotionController::Get()->m_config;

	// �ӽŰ� ���ĵ� ������ �����ϰ� ����.
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
		// ����� ���̰ų� �ø��� ���ϰ� �Ѵ�.
		cs.style = WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX | WS_BORDER;
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
		CREATE_PANE(CLauncherView, m_launcherView, SW_SHOW);
		CREATE_PANE(CUDPInputView, m_udpInputView, SW_HIDE);
		CREATE_PANE(CMixingView, m_mixingView, SW_HIDE);
		CREATE_PANE(CUDPParseView, m_udpParseView, SW_HIDE);
		CREATE_PANE(CPlotView, m_plotView, SW_HIDE);
		CREATE_PANE(CControlBoard, m_controlBoardView, SW_HIDE);

		g_udpInputView = m_udpInputView;
		g_controlView = m_controlBoardView;
		g_launcherView = m_launcherView;
	}
	else
	{
		CREATE_DOCKPANE(C3DView, L"Motion Output View", ID_VIEW_CUBE3D, IDD_DIALOG_3D, m_wndCube3DView);
		CREATE_DOCKPANE2(CMotionOutputView, L"Motion Output View", ID_VIEW_MOTION_OUTPUT, m_motionOutputView);
		CREATE_DOCKPANE2(CUDPInputView, L"UDP Input View", ID_VIEW_MOTION_INPUT, m_udpInputView);
		CREATE_DOCKPANE2(CJoystickView, L"Joystick View", ID_VIEW_JOYSTICK, m_joystickView);
		CREATE_DOCKPANE2(CMotionWaveView, L"MotionWave View", ID_VIEW_MOTIONWAVE, m_motionWaveView);
		CREATE_DOCKPANE2(CMixingView, L"Mixing View", ID_VIEW_MIXING, m_mixingView);
		CREATE_DOCKPANE2(CControlBoard, L"Control Board", ID_VIEW_CONTROLBOARD, m_controlBoardView);
		CREATE_DOCKPANE2(CUDPParseView, L"UDP Parse View", ID_VIEW_UDPPARSE, m_udpParseView);
		CREATE_DOCKPANE2(CPlotView, L"Plot View", ID_VIEW_PLOT, m_plotView);

		g_mwaveView = m_motionWaveView;
		g_udpInputView = m_udpInputView;
		g_controlView = m_controlBoardView;
		g_launcherView = m_launcherView;

		cController::Get()->Init(m_wndCube3DView->GetRenderer());
		m_wndCube3DView->SetRenderCube(true);
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


// �� ��ü�� UpdateConfig() �Լ��� ȣ���Ѵ�.
void CMainFrame::UpdateConfig(bool IsSaveAndValidate) //IsSaveAndValidate=true
{
	for each (auto &view in m_viewList)
		view->GetChildView()->UpdateConfig(IsSaveAndValidate);
}


// ȯ�漳�� ������ �а�, �並 ������Ʈ �Ѵ�.
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
	CPlotView *plotView;

	CString viewName;
	viewName.Format(L"Plot View%d", plotViewIncId++);
	CREATE_DOCKPANE2(CPlotView, viewName, plotViewId++, plotView);

	CDockablePane *pane = (CDockablePane*)plotView->GetParent();
	pane->EnableDocking(CBRS_ALIGN_ANY);
	CDockablePane* pTabbedBar = NULL;
	pane->AttachToTabWnd((CDockablePane*)m_plotView->GetParent(), DM_SHOW, TRUE, &pTabbedBar);

	plotView->SetAddPlotView(true);

	return TRUE;
}


void CMainFrame::OnSize(UINT nType, int cx, int cy)
{
	CFrameWndEx::OnSize(nType, cx, cy);

	RET(m_viewList.empty());

	// MainFrameũ�⿡ ���� Pane ũ�⸦ �����.
	CDockablePane *parentPane = NULL;
	for each(auto it in m_viewList)
	{
		CPaneDivider* pDefaultPaneDivider = it->GetDefaultPaneDivider();
		if (pDefaultPaneDivider)
		{
			parentPane = it;
			break;
		}
	}

	if (parentPane)
	{
		for each(auto it in m_viewList)
		{
			CBaseTabbedPane *tabPane = it->GetParentTabbedPane();
			if (tabPane)
			{
				// TabPane�� �Ϲ� Pane�� ���� MainFrame�ȿ� �����ִٸ�, ������ũ�⸦ �ڵ� �������� �ʴ´�.	
				if (tabPane->GetDefaultPaneDivider())
					return;
			}
		}
	}
	else
	{
		// ParentPane�� ���ٸ�, TabControl�� �ִ��� �˻��Ѵ�.
		for each(auto it in m_viewList)
		{
			CBaseTabbedPane *tabPane = it->GetParentTabbedPane();
			if (!tabPane)
				continue;

			CPaneDivider* pDefaultPaneDivider = tabPane->GetDefaultPaneDivider();
			if (pDefaultPaneDivider)
			{
				parentPane = tabPane;
				break;
			}
		}
	}

	RET(!parentPane);

	if (IsWindowVisible())
	{
		// �Ǽ�, ȭ�鿡 �������� ������ ����, ó�� ���۽ÿ��� -1�� �����Ǿ� �ִ�.
		// ó�� ���۽ÿ��� ������ ũ�⸦ �������� �ʴ´�.
		if (m_nWindow >= 0)
			SetContainerSize(parentPane, cx, cy);
	}
}


// Pane ũ�� ����
void CMainFrame::SetContainerSize(CDockablePane* targetPane, UINT cx, UINT cy)
{
	CPaneDivider* pDefaultPaneDivider = targetPane->GetDefaultPaneDivider();
	if (pDefaultPaneDivider == NULL)
	{
		OutputDebugString(_T("The DialogBar is not docked.\n"));
		return;
	}

	BOOL bLeftBar = FALSE;
	CPaneContainer* pContainer = pDefaultPaneDivider->FindPaneContainer(targetPane, bLeftBar);

	while (pContainer->GetParentPaneContainer() != NULL)
	{
		pContainer = pContainer->GetParentPaneContainer();
	}

	CRect rectContainer;
	pContainer->GetWindowRect(rectContainer, FALSE);
	DWORD dwDividerStyle = pDefaultPaneDivider->GetCurrentAlignment();

	CPoint ptOffset(0, 0);
	switch (dwDividerStyle)
	{
	case CBRS_ALIGN_TOP:
		ptOffset.y = cy - rectContainer.Height();
		break;
	case CBRS_ALIGN_BOTTOM:
		ptOffset.y = rectContainer.Height() - cy;
		break;
	case CBRS_ALIGN_LEFT:
		ptOffset.x = cx - rectContainer.Width();
		break;
	case CBRS_ALIGN_RIGHT:
		ptOffset.x = rectContainer.Width() - cx;
		break;
	}

	pDefaultPaneDivider->Move(ptOffset);
}

