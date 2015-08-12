
// UDPAnalyzer.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "UDPAnalyzer.h"
#include "MainFrm.h"

#include "UDPAnalyzerDoc.h"
#include "UDPAnalyzerView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CUDPAnalyzerApp

BEGIN_MESSAGE_MAP(CUDPAnalyzerApp, CWinAppEx)
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
END_MESSAGE_MAP()


// CUDPAnalyzerApp construction

CUDPAnalyzerApp::CUDPAnalyzerApp()
{
	m_bHiColorIcons = TRUE;

	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// If the application is built using Common Language Runtime support (/clr):
	//     1) This additional setting is needed for Restart Manager support to work properly.
	//     2) In your project, you must add a reference to System.Windows.Forms in order to build.
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif
	SetAppID(_T("UDPAnalyzer.AppID.NoVersion"));
}

// The one and only CUDPAnalyzerApp object

CUDPAnalyzerApp theApp;


// CUDPAnalyzerApp initialization

BOOL CUDPAnalyzerApp::InitInstance()
{
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	HRESULT hr = CoInitialize(NULL);
	if (FAILED(hr))
	{
		_tprintf(_T("Failed to initialize COM, Error:%x\n"), hr);
		return false;
	}

	//Initialize COM security (Required by CEnumerateSerial::UsingWMI)
	hr = CoInitializeSecurity(NULL, -1, NULL, NULL, RPC_C_AUTHN_LEVEL_DEFAULT, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE, NULL);
	if (FAILED(hr))
	{
		_tprintf(_T("Failed to initialize COM security, Error:%x\n"), hr);
		CoUninitialize();
		return false;
	}

	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	AfxInitRichEdit2();

	SetRegistryKey(_T("Local AppWizard-Generated Applications"));
	LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)


	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CUDPAnalyzerDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CUDPAnalyzerView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	m_pMainWnd->SetWindowTextW(L"UDP Analyzer");

	//Close down COM
	CoUninitialize();

	return TRUE;
}

int CUDPAnalyzerApp::ExitInstance()
{
	AfxOleTerm(FALSE);
	return CWinAppEx::ExitInstance();
}

// CUDPAnalyzerApp customization load/save methods

void CUDPAnalyzerApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
	bNameValid = strName.LoadString(IDS_EXPLORER);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EXPLORER);
}

void CUDPAnalyzerApp::LoadCustomState()
{
}

void CUDPAnalyzerApp::SaveCustomState()
{
}

// CUDPAnalyzerApp message handlers





BOOL CUDPAnalyzerApp::OnIdle(LONG lCount)
{
	static int oldT = timeGetTime();
	const int curT = timeGetTime();
	const float deltaSeconds = (float)(curT - oldT) * 0.001f; // 1초가 경과되면 1이된다.

	//if (deltaSeconds > 0.01f) // max 100 frame
	//{
	//	if (g_3DView)
	//	{
	//		//g_3DView->Update(deltaSeconds);
	//		//g_3DView->Render();
	//	}

	//	oldT = curT;
	//}

	cController::Get()->Update(deltaSeconds);

	oldT = curT;

	//CWinAppEx::OnIdle(lCount); 호출 안해도 됨.
	return TRUE; // TRUE를 리턴시켜야 계속 OnIdle()이 호출된다.
}
