
// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "MotionMonitor.h"

#include "SerialEditorView.h"
#include "SerialGraphForm.h"
#include "UDPGraphView.h"
#include "UDPEditorView.h"
#include "MotionOutputView.h"
#include "MotionControlView.h"
#include "MotionInputView.h"
#include "JoystickView.h"
#include "MotionWaveView.h"
#include "MixingView.h"
#include "ControlBoard.h"
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


BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
	ON_WM_CREATE()
	ON_REGISTERED_MESSAGE(AFX_WM_CREATETOOLBAR, &CMainFrame::OnToolbarCreateNew)
	ON_COMMAND(ID_VIEW_VIEWINITIALIZE, &CMainFrame::OnViewViewinitialize)
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
{
	// TODO: add member initialization code here
	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_VS_2008);
}

CMainFrame::~CMainFrame()
{
	for each (auto &pane in m_viewList)
		delete pane;
	m_viewList.clear();
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

	BOOL bNameValid;

	if (!m_wndMenuBar.Create(this))
	{
		TRACE0("Failed to create menubar\n");
		return -1;      // fail to create
	}

	m_wndMenuBar.SetPaneStyle(m_wndMenuBar.GetPaneStyle() | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY);

	// prevent the menu bar from taking the focus on activation
	CMFCPopupMenu::SetForceMenuFocus(FALSE);

// 	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
// 		!m_wndToolBar.LoadToolBar(theApp.m_bHiColorIcons ? IDR_MAINFRAME_256 : IDR_MAINFRAME))
// 	{
// 		TRACE0("Failed to create toolbar\n");
// 		return -1;      // fail to create
// 	}

// 	CString strToolBarName;
// 	bNameValid = strToolBarName.LoadString(IDS_TOOLBAR_STANDARD);
// 	ASSERT(bNameValid);
// 	m_wndToolBar.SetWindowText(strToolBarName);

	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
// 	ASSERT(bNameValid);
// 	m_wndToolBar.EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);

	// Allow user-defined toolbars operations:
//	InitUserToolbars(NULL, uiFirstUserToolBarId, uiLastUserToolBarId);

// 	if (!m_wndStatusBar.Create(this))
// 	{
// 		TRACE0("Failed to create status bar\n");
// 		return -1;      // fail to create
// 	}
// 	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	// TODO: Delete these five lines if you don't want the toolbar and menubar to be dockable
	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
//	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndMenuBar);
//	DockPane(&m_wndToolBar);


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

	for each (auto &view in m_viewList)
		view->EnableDocking(CBRS_ALIGN_ANY);

	DockPane(m_wndCube3DView);
	CDockablePane* pTabbedBar = NULL;
	for each (auto &view in m_viewList)
	{
		if (view != m_wndCube3DView)
			view->AttachToTabWnd(m_wndCube3DView, DM_SHOW, TRUE, &pTabbedBar);
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

	return TRUE;
}

BOOL CMainFrame::CreateDockingWindows()
{
	BOOL bNameValid;

	// Create class view
	CString strClassView;
	bNameValid = strClassView.LoadString(IDS_CLASS_VIEW);
	ASSERT(bNameValid);


	// Create cube3d view
	m_wndCube3DView = new CCube3DPane();
	if (!m_wndCube3DView->Create(L"3DView", this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_CUBE3D, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create cube 3d View window\n");
		return FALSE; // failed to create
	}

	// Create udp graph view
	{
		m_udpGraphView = new CDockablePaneBase();
		if (!m_udpGraphView->Create(L"UDP Graph View", this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_UDP_GRAPH, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
		{
			TRACE0("Failed to create UDP Graph View window\n");
			return FALSE; // failed to create
		}

		CUDPGraphView *view = new CUDPGraphView(m_udpGraphView);
		view->Create(CUDPGraphView::IDD, m_udpGraphView);
		view->ShowWindow(SW_SHOW);
		m_udpGraphView->SetChildView(view);
	}


	// Create udp graph view
	{
		m_udpEditorView = new CDockablePaneBase();
		if (!m_udpEditorView->Create(L"UDP Editor View", this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_UDP_EDITOR, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
		{
			TRACE0("Failed to create UDP Editor View window\n");
			return FALSE; // failed to create
		}

		CUDPEditorView *view = new CUDPEditorView(m_udpEditorView);
		view->Create(CUDPEditorView::IDD, m_udpEditorView);
		view->ShowWindow(SW_SHOW);
		m_udpEditorView->SetChildView(view);
	}


	// Create motion output view
	{
		m_motionOutputView = new CDockablePaneBase();
		if (!m_motionOutputView->Create(L"Motion Output View", this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_MOTION_OUTPUT, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
		{
			TRACE0("Failed to create Motion Output View window\n");
			return FALSE; // failed to create
		}

		CMotionOutputView *view = new CMotionOutputView(m_motionOutputView);
		view->Create(CMotionOutputView::IDD, m_motionOutputView);
		view->ShowWindow(SW_SHOW);
		m_motionOutputView->SetChildView(view);
	}


	// Create motion control view
	{
		m_motionControlView = new CDockablePaneBase();
		if (!m_motionControlView->Create(L"Motion Control View", this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_MOTION_CONTROL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
		{
			TRACE0("Failed to create Motion Control View window\n");
			return FALSE; // failed to create
		}

		CMotionControlView *view = new CMotionControlView(m_motionControlView);
		view->Create(CMotionControlView::IDD, m_motionControlView);
		view->ShowWindow(SW_SHOW);
		m_motionControlView->SetChildView(view);
	}


	// Create motion input view
	{
		m_motionInputView = new CDockablePaneBase();
		if (!m_motionInputView->Create(L"UDP Input View", this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_MOTION_INPUT, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
		{
			TRACE0("Failed to create UDP Input View window\n");
			return FALSE; // failed to create
		}

		CMotionInputView *view = new CMotionInputView(m_motionInputView);
		view->Create(CMotionInputView::IDD, m_motionInputView);
		view->ShowWindow(SW_SHOW);
		m_motionInputView->SetChildView(view);
	}


	// Create Joystick view
	{
		m_joystickView = new CDockablePaneBase();
		if (!m_joystickView->Create(L"Joystick View", this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_JOYSTICK, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
		{
			TRACE0("Failed to create Joystick View window\n");
			return FALSE; // failed to create
		}

		CJoystickView *view = new CJoystickView(m_joystickView);
		view->Create(CJoystickView::IDD, m_joystickView);
		view->ShowWindow(SW_SHOW);
		m_joystickView->SetChildView(view);
	}


	// Create MotionWave view
	{
		m_motionWaveView = new CDockablePaneBase();
		if (!m_motionWaveView->Create(L"MotionWave View", this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_MOTIONWAVE, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
		{
			TRACE0("Failed to create MotionWave View window\n");
			return FALSE; // failed to create
		}

		CMotionWaveView *view = new CMotionWaveView(m_motionWaveView);
		view->Create(CMotionWaveView::IDD, m_motionWaveView);
		view->ShowWindow(SW_SHOW);
		m_motionWaveView->SetChildView(view);

		g_mwaveView = view;
	}


	// Create Mixing view
	{
		m_mixingView= new CDockablePaneBase();
		if (!m_mixingView->Create(L"Mixing View", this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_MIXING, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
		{
			TRACE0("Failed to create Mixing View window\n");
			return FALSE; // failed to create
		}

		CMixingView *view = new CMixingView(m_mixingView);
		view->Create(CMixingView::IDD, m_mixingView);
		view->ShowWindow(SW_SHOW);
		m_mixingView->SetChildView(view);
	}

	// Create Controlboard view
	{
		m_controlBoardView = new CDockablePaneBase();
		if (!m_controlBoardView->Create(L"Control Board", this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_CONTROLBOARD, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
		{
			TRACE0("Failed to create Controlboard window\n");
			return FALSE; // failed to create
		}

		CControlBoard *view = new CControlBoard(m_controlBoardView);
		view->Create(CControlBoard::IDD, m_controlBoardView);
		view->ShowWindow(SW_SHOW);
		m_controlBoardView->SetChildView(view);
	}



	// Create serial editor view
	{
		m_wndSerialEditorView = new CDockablePaneBase();
		if (!m_wndSerialEditorView->Create(L"SerialEditorView", this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_SERIAL_EDITOR, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
		{
			TRACE0("Failed to create SerialEditor View window\n");
			return FALSE; // failed to create
		}

		CSerialEditorView *view = new CSerialEditorView(m_wndSerialEditorView);
		BOOL reval = view->Create(CSerialEditorView::IDD, m_wndSerialEditorView);
		view->ShowWindow(SW_SHOW);
		m_wndSerialEditorView->SetChildView(view);
	}


	// Create serial graph view
	{
		m_serialGraphView = new CDockablePaneBase();
		if (!m_serialGraphView->Create(L"SerialGraphView", this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_SERIAL_GRAPH, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
		{
			TRACE0("Failed to create SerialGraph View window\n");
			return FALSE; // failed to create
		}

		CSerialGraphForm *view = new CSerialGraphForm(m_serialGraphView);
		view->Create(CSerialGraphForm::IDD, m_serialGraphView);
		view->ShowWindow(SW_SHOW);
		m_serialGraphView->SetChildView(view);
	}

	m_viewList.push_back(m_wndCube3DView);
	m_viewList.push_back(m_udpGraphView);
	m_viewList.push_back(m_udpEditorView);
	m_viewList.push_back(m_motionOutputView);
	m_viewList.push_back(m_motionInputView);
	m_viewList.push_back(m_motionControlView);
	m_viewList.push_back(m_joystickView);
	m_viewList.push_back(m_motionWaveView);
	m_viewList.push_back(m_mixingView);	
	m_viewList.push_back(m_controlBoardView);
	m_viewList.push_back(m_wndSerialEditorView);
	m_viewList.push_back(m_serialGraphView);


	SetDockingWindowIcons(theApp.m_bHiColorIcons);
	return TRUE;
}

void CMainFrame::SetDockingWindowIcons(BOOL bHiColorIcons)
{
// 	HICON hFileViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_FILE_VIEW_HC : IDI_FILE_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
// 	m_wndFileView.SetIcon(hFileViewIcon, FALSE);
// 
 	HICON hClassViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_CLASS_VIEW_HC : IDI_CLASS_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
// 	m_wndClassView.SetIcon(hClassViewIcon, FALSE);
// 
// 	HICON hOutputBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_OUTPUT_WND_HC : IDI_OUTPUT_WND), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
// 	m_wndOutput.SetIcon(hOutputBarIcon, FALSE);
// 
// 	HICON hPropertiesBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_PROPERTIES_WND_HC : IDI_PROPERTIES_WND), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
// 	m_wndProperties.SetIcon(hPropertiesBarIcon, FALSE);

	m_wndCube3DView->SetIcon(hClassViewIcon, FALSE);
	//m_wndSensorView->SetIcon(hCube3DViewIcon, FALSE);
	m_wndSerialEditorView->SetIcon(hClassViewIcon, FALSE);
	m_serialGraphView->SetIcon(hClassViewIcon, FALSE);
	m_udpGraphView->SetIcon(hClassViewIcon, FALSE);
	m_udpEditorView->SetIcon(hClassViewIcon, FALSE);
	m_motionOutputView->SetIcon(hClassViewIcon, FALSE);
	m_motionInputView->SetIcon(hClassViewIcon, FALSE);
	m_motionControlView->SetIcon(hClassViewIcon, FALSE);
	m_joystickView->SetIcon(hClassViewIcon, FALSE);
	m_motionWaveView->SetIcon(hClassViewIcon, FALSE);
	m_mixingView->SetIcon(hClassViewIcon, FALSE);
	m_controlBoardView->SetIcon(hClassViewIcon, FALSE);
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
