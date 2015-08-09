
// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "UDPAnalyzer.h"

#include "UDPView.h"
#include "PlotView.h"
#include "MixingView.h"
#include "OutputView.h"
#include "DXView.h"
#include "MainFrm.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWndEx)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND(ID_VIEW_CUSTOMIZE, &CMainFrame::OnViewCustomize)
	ON_REGISTERED_MESSAGE(AFX_WM_CREATETOOLBAR, &CMainFrame::OnToolbarCreateNew)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnUpdateApplicationLook)
	ON_WM_SETTINGCHANGE()
	ON_WM_DESTROY()
	ON_COMMAND(ID_FILE_OPEN, &CMainFrame::OnFileOpen)
	ON_COMMAND(ID_FILE_SAVEAS, &CMainFrame::OnFileSaveAs)
	ON_COMMAND(ID_FILE_NEW, &CMainFrame::OnFileNew)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVEAS, &CMainFrame::OnUpdateFileSaveas)
	ON_COMMAND(ID_VIEW_INITDOCKINGWINDOWS, &CMainFrame::OnViewInitdockingwindows)
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

	// 기본 설정파일을 연다.
	g_option.Read("udpanalyzer_default.cfg", false);
	g_option.m_fileName = "udpanalyzer_default.cfg";
}

CMainFrame::~CMainFrame()
{
	if (!g_option.m_fileName.empty())
	{
		g_option.Write(g_option.m_fileName);
	}
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (g_option.m_initWindows)
	{
		GetDockingManager()->DisableRestoreDockState();
		CDockingManager::SetDockingMode(DT_IMMEDIATE);
		g_option.m_initWindows = false;
	}

	BOOL bNameValid;

	if (!m_wndMenuBar.Create(this))
	{
		TRACE0("Failed to create menubar\n");
		return -1;      // fail to create
	}

	m_wndMenuBar.SetPaneStyle(m_wndMenuBar.GetPaneStyle() | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY);

	CMFCPopupMenu::SetForceMenuFocus(FALSE);

	CString strToolBarName;
	bNameValid = strToolBarName.LoadString(IDS_TOOLBAR_STANDARD);
	ASSERT(bNameValid);

	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	InitUserToolbars(NULL, uiFirstUserToolBarId, uiLastUserToolBarId);

	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndMenuBar);

	CDockingManager::SetDockingMode(DT_SMART);
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	CMFCToolBar::AddToolBarForImageCollection(IDR_MENU_IMAGES, theApp.m_bHiColorIcons ? IDB_MENU_IMAGES_24 : 0);

	if (!CreateDockingWindows())
	{
		TRACE0("Failed to create docking windows\n");
		return -1;
	}

	for each (auto &view in m_viewList)
		view->EnableDocking(CBRS_ALIGN_ANY);

	DockPane(m_udpView);
	CDockablePane* pTabbedBar = NULL;
	for each (auto &view in m_viewList)
	{
		if (view != m_udpView)
			view->AttachToTabWnd(m_udpView, DM_SHOW, TRUE, &pTabbedBar);
	}

	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2008));
	CDockingManager::SetDockingMode(DT_SMART);
	RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);
	theApp.WriteInt(_T("ApplicationLook"), theApp.m_nAppLook);

	EnablePaneMenu(TRUE, ID_VIEW_CUSTOMIZE, strCustomize, ID_VIEW_TOOLBAR);
	CMFCToolBar::EnableQuickCustomization();

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
#define CREATE_DOCKVIEW2(Class, VAR, PANE_NAME, PANE_ID, RESOURCE_ID) \
	{\
		VAR = new CDockablePaneBase();\
		if (!VAR->Create(PANE_NAME, this, CRect(0, 0, 200, 200), TRUE, PANE_ID, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))\
								{\
			TRACE0("Failed to create View window\n");\
			return FALSE;\
								}\
		Class *view = new Class(VAR);\
		BOOL reval = view->Create(RESOURCE_ID, VAR);\
		view->ShowWindow(SW_SHOW);\
		VAR->SetChildView(view);\
		m_viewList.push_back(VAR);\
		\
		HICON hClassViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(theApp.m_bHiColorIcons? IDI_CLASS_VIEW_HC : IDI_CLASS_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);\
 		VAR->SetIcon(hClassViewIcon, FALSE);\
	}
#define CREATE_DOCKVIEW(Class, VAR, PANE_NAME, PANE_ID) \
	CREATE_DOCKVIEW2(Class, VAR, PANE_NAME, PANE_ID, Class::IDD);


	// Create serial editor view
	CREATE_DOCKVIEW(CUDPView, m_udpView, L"UDP View", ID_VIEW_UDP);
	CREATE_DOCKVIEW(CPlotView, m_plotView, L"Plot View", ID_VIEW_PLOT);
	CREATE_DOCKVIEW(CMixingView, m_mixingView, L"Mixing View", ID_VIEW_MIXING);
	CREATE_DOCKVIEW(COutputView, m_outputView, L"Output View", ID_VIEW_OUTPUT);
	CREATE_DOCKVIEW2(C3DView, m_dxView, L"3D View", ID_VIEW_DX, IDD_DIALOG_3D);




	graphic::GetMainCamera()->SetCamera(Vector3(-8.5f, 2.4f, -3.2f), Vector3(0, 0, 0), Vector3(0, 1, 0));
	return TRUE;
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

void CMainFrame::OnViewCustomize()
{
	CMFCToolBarsCustomizeDialog* pDlgCust = new CMFCToolBarsCustomizeDialog(this, TRUE /* scan menus */);
	pDlgCust->EnableUserDefinedToolbars();
	pDlgCust->Create();
}

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


void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
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



void CMainFrame::OnDestroy()
{
	CFrameWndEx::OnDestroy();

	for each (auto &view in m_viewList)
	{
		view->DestroyWindow();
		delete view;
	}
	m_viewList.clear();
}


// 환경변수에 설정된 값을 UI 에 적용한다.
void CMainFrame::UpdateViewConfig()
{
	((COutputView*)m_outputView->GetChildView())->UpdateConfig();
	((CPlotView*)m_plotView->GetChildView())->UpdateConfig();
	((CUDPView*)m_udpView->GetChildView())->UpdateConfig();
	((CMixingView*)m_mixingView->GetChildView())->UpdateConfig();
}


// UI에 설정된 값을 환경변수에 저장한다.
void CMainFrame::SaveViewConfig(const string fileName)
{
	RET(fileName.empty());

	((COutputView*)m_outputView->GetChildView())->SaveConfig();
	((CPlotView*)m_plotView->GetChildView())->SaveConfig();
	((CUDPView*)m_udpView->GetChildView())->SaveConfig();
	((CMixingView*)m_mixingView->GetChildView())->SaveConfig();

	// 환경변수 파일 저장
	g_option.m_fileName = fileName;
	g_option.Write(fileName);
}


void CMainFrame::OnFileOpen()
{
	TCHAR szFilter[] = L"Option(*.cfg)|*.cfg| All Files(*.*)|*.*||";
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, szFilter);
	if (IDOK == dlg.DoModal()) 
	{
		// 설정해놓은 값을 먼저 저장한다.
		SaveViewConfig(g_option.m_fileName);

		g_option.Read(wstr2str((LPCTSTR)dlg.GetPathName()));

		UpdateViewConfig();
	}
}


void CMainFrame::OnFileSaveAs()
{
	TCHAR szFilter[] = L"Option(*.cfg)|*.cfg| All Files(*.*)|*.*||";
	CFileDialog dlg(FALSE , NULL, NULL, OFN_HIDEREADONLY, szFilter);
	if (IDOK == dlg.DoModal())
	{
		// UI에 설정된 값을 환경변수에 저장한다.
		SaveViewConfig(wstr2str((LPCTSTR)dlg.GetPathName()));
	}
}


// 기본 설정 값을 로드한다.
void CMainFrame::OnFileNew()
{
	// UI에 설정된 값을 환경변수에 저장한다.
	SaveViewConfig(g_option.m_fileName);

	g_option.Read("udpanalyzer_default.cfg", false);

	UpdateViewConfig();
}


void CMainFrame::OnUpdateFileSaveas(CCmdUI *pCmdUI)
{
	pCmdUI->Enable();
}


void CMainFrame::OnViewInitdockingwindows()
{
	g_option.m_initWindows = true;
}
