
// MotionMonitor.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "MotionMonitor.h"
#include "MainFrm.h"

#include "MotionMonitorDoc.h"
#include "MotionMonitorView.h"
#include "LoginDialog.h"
#include <gdiplus.h> 
#pragma comment( lib, "gdiplus.lib" ) 
using namespace Gdiplus;



#ifdef _DEBUG
#define new DEBUG_NEW
#endif


bool g_isLoop = true;

// CMotionMonitorApp
BEGIN_MESSAGE_MAP(CMotionMonitorApp, CWinAppEx)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	//ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
	ON_COMMAND(ID_FILE_OPEN, &CMotionMonitorApp::OnFileOpen)
END_MESSAGE_MAP()


// CMotionMonitorApp construction

CMotionMonitorApp::CMotionMonitorApp()
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

	// TODO: replace application ID string below with unique ID string; recommended
	// format for string is CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("MotionMonitor.AppID.NoVersion"));

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

// The one and only CMotionMonitorApp object

CMotionMonitorApp theApp;


// CMotionMonitorApp initialization

BOOL CMotionMonitorApp::InitInstance()
{
	AfxInitRichEdit2();

	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
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

	
	GdiplusStartupInput GdiplusStartupInput;
	if (::GdiplusStartup(&m_gdiplusToken, &GdiplusStartupInput, NULL) != Ok)
	{
		AfxMessageBox(_T("Error : Failed to initialize GDI+ library!"));
		return FALSE;
	}



	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// AfxInitRichEdit2() is required to use RichEdit control	
	// AfxInitRichEdit2();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));
	LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)


	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);



	// 실행인자값으로 넘어온 설정파일명을 로드한다.
	cMotionMonitorConfig &config = cMotionController::Get()->m_config;
	if (__argc > 1)
	{
		string commandLine = wstr2str(__targv[1]);
		if (!commandLine.empty())
		{
			config.ReadConfigFile(commandLine);
		}
	}

//  	CLoginDialog loginDlg;
//  	if (IDCANCEL == loginDlg.DoModal())
//  		return FALSE;

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CMotionMonitorDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CMotionMonitorView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);


	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	
	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	int width=0, height=0;
	if (__argc > 3)
	{
		width = _ttoi(__targv[2]);
		height = _ttoi(__targv[3]);
	}


	// 윈도우 크기 설정
	if (config.m_mode == "machinegun_stand")
	{
		if (width == 0 || height == 0)
		{
			width = 445;
			height = 600;
		}

		CRect wr;
		m_pMainWnd->GetWindowRect(wr);
		wr.right = wr.left + width;
		wr.bottom = wr.top + height;
		m_pMainWnd->MoveWindow(wr);
	}
	else if (config.m_mode == "dirt3_release")
	{
		CRect wr;
		m_pMainWnd->GetWindowRect(wr);
		wr.right = wr.left + 475;
		wr.bottom = wr.top + 680;
		m_pMainWnd->MoveWindow(wr);
	}
	else if (config.m_mode == "realshot")
	{
		CRect wr;
		m_pMainWnd->GetWindowRect(wr);
// 		wr.right = wr.left + 650;
// 		wr.bottom = wr.top + 400;
		wr.right = wr.left + 350;
		wr.bottom = wr.top + 300;
		m_pMainWnd->MoveWindow(wr);
	}

	// The one and only window has been initialized, so show and update it
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	//m_pMainWnd->SetWindowTextW(L"Motion Monitor");

	const string title = common::GetFileName(cMotionController::Get()->m_config.m_fileName);

	if (config.m_mode == "machinegun_stand")
	{
		m_pMainWnd->SetWindowTextW(L"MachineGun-X Admin Manager");
	}
	else if (config.m_mode == "dirt3_release")
	{
		m_pMainWnd->SetWindowTextW(L"Dirt3 Manager");
	}
	else if (config.m_mode == "realshot")
	{
		m_pMainWnd->SetWindowTextW(L"RealShot Manager");
	}
	else
	{
		m_pMainWnd->SetWindowTextW(str2wstr(title).c_str());
	}
	((CMainFrame*)m_pMainWnd)->Init();
		
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	int oldT = timeGetTime();
	while (1)
	{
		//PeekMessage 는 메시지 큐에 메시지가 없어도 프로그램이 멈추기 않고 진행이 된다.
		//이때 메시지큐에 메시지가 없으면 false 가 리턴되고 메시지가 있으면 true 가 리턴이된다.
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg); //눌린 키보드 의 문자를 번역하여 WM_CHAR 메시지를 발생시킨다.
			DispatchMessage(&msg);  //받아온 메시지 정보로 윈도우 프로시져 함수를 실행시킨다.
		}

		if (!g_isLoop)
			break;

		const int curT = timeGetTime();
		const float deltaSeconds = (float)(curT - oldT) * 0.001f; // 1초가 경과되면 1이된다.
		if (deltaSeconds > 0.01f)
		{
			cController::Get()->Update(deltaSeconds);
			oldT = curT;
		}
	}
	//Close down COM
	CoUninitialize();

	return TRUE;
}

int CMotionMonitorApp::ExitInstance()
{
	//TODO: handle additional resources you may have added
	AfxOleTerm(FALSE);

	::GdiplusShutdown(m_gdiplusToken);

	return CWinAppEx::ExitInstance();
}


// CMotionMonitorApp customization load/save methods

void CMotionMonitorApp::PreLoadState()
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

void CMotionMonitorApp::LoadCustomState()
{
}

void CMotionMonitorApp::SaveCustomState()
{
}

// CMotionMonitorApp message handlers

BOOL CMotionMonitorApp::OnIdle(LONG lCount)
{
	static int oldT = timeGetTime();
	const int curT = timeGetTime();
	const float deltaSeconds = (float)(curT - oldT) * 0.001f; // 1초가 경과되면 1이된다.

	cController::Get()->Update(deltaSeconds);

	oldT = curT;

	//return CWinAppEx::OnIdle(lCount); //호출 안해도 됨.
	return TRUE; // TRUE를 리턴시켜야 계속 OnIdle()이 호출된다.
}


void CMotionMonitorApp::OnFileOpen()
{
	// 파일 형식 콤보박스에 등록할 필터를 정의한다. (*.*, *.cpp, *.txt)
	//TCHAR name_filter[] = L"All Files (*.*)|*.*|C++ Files (*.cpp)|*.cpp|Text Files (*.txt)|*.txt||";
	TCHAR name_filter[] = L"All Files (*.*)|*.*|Config Files (*.json)|*.json|";

	// TRUE -> 열기대화상자, "cpp" -> 사용자가 확장자 없이 파일명만 입력했을때 자동으로 추가될 확장자명이다.
	// 즉, stdafx 까지만 입력하면 stdafx.cpp라고 입력한것과 동일하게 하고 싶을때 사용한다.
	// "*.cpp" 파일이름 에디트에 출력될 기본 문자열이다. 
	// OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT -> 파일 대화상자에 추가적으로 사용할 속성이다.
	// name_filter -> 파일 형식 콤보박스에 등록할 필터정보를 담고있는 메모리의 주소이다.
	CFileDialog ins_dlg(TRUE, L"json", L"*.json",
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, name_filter, NULL);

	// 파일 형식 콤보박스에 나열된 필터들 중에서 2번째 항목(*.json)을 선택한다.
	ins_dlg.m_ofn.nFilterIndex = 1;

	if (ins_dlg.DoModal() == IDOK){
		// 선택된 파일의 경로명을 이용하여 도큐먼트 정보를 재구성한다.
		OpenDocumentFile(ins_dlg.GetPathName());
	}
	else {
		// 원래 "파일 열기" 기능에서는 파일 열기를 취소했을 때, 특별한 메시지가 나오지 않지만
		// 재정의한 효과를 나타내기 위해서 파일 열기를 취소했을때, 취소했다는 메시지가 나오도록
		// 재구성 했습니다.
		//::MessageBox(NULL, L"파일 열기를 취소하였습니다.", L"알림", MB_ICONINFORMATION);
	}
}
