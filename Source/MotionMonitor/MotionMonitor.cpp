
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



	// �������ڰ����� �Ѿ�� �������ϸ��� �ε��Ѵ�.
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


	// ������ ũ�� ����
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
		//PeekMessage �� �޽��� ť�� �޽����� ��� ���α׷��� ���߱� �ʰ� ������ �ȴ�.
		//�̶� �޽���ť�� �޽����� ������ false �� ���ϵǰ� �޽����� ������ true �� �����̵ȴ�.
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg); //���� Ű���� �� ���ڸ� �����Ͽ� WM_CHAR �޽����� �߻���Ų��.
			DispatchMessage(&msg);  //�޾ƿ� �޽��� ������ ������ ���ν��� �Լ��� �����Ų��.
		}

		if (!g_isLoop)
			break;

		const int curT = timeGetTime();
		const float deltaSeconds = (float)(curT - oldT) * 0.001f; // 1�ʰ� ����Ǹ� 1�̵ȴ�.
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
	const float deltaSeconds = (float)(curT - oldT) * 0.001f; // 1�ʰ� ����Ǹ� 1�̵ȴ�.

	cController::Get()->Update(deltaSeconds);

	oldT = curT;

	//return CWinAppEx::OnIdle(lCount); //ȣ�� ���ص� ��.
	return TRUE; // TRUE�� ���Ͻ��Ѿ� ��� OnIdle()�� ȣ��ȴ�.
}


void CMotionMonitorApp::OnFileOpen()
{
	// ���� ���� �޺��ڽ��� ����� ���͸� �����Ѵ�. (*.*, *.cpp, *.txt)
	//TCHAR name_filter[] = L"All Files (*.*)|*.*|C++ Files (*.cpp)|*.cpp|Text Files (*.txt)|*.txt||";
	TCHAR name_filter[] = L"All Files (*.*)|*.*|Config Files (*.json)|*.json|";

	// TRUE -> �����ȭ����, "cpp" -> ����ڰ� Ȯ���� ���� ���ϸ� �Է������� �ڵ����� �߰��� Ȯ���ڸ��̴�.
	// ��, stdafx ������ �Է��ϸ� stdafx.cpp��� �Է��ѰͰ� �����ϰ� �ϰ� ������ ����Ѵ�.
	// "*.cpp" �����̸� ����Ʈ�� ��µ� �⺻ ���ڿ��̴�. 
	// OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT -> ���� ��ȭ���ڿ� �߰������� ����� �Ӽ��̴�.
	// name_filter -> ���� ���� �޺��ڽ��� ����� ���������� ����ִ� �޸��� �ּ��̴�.
	CFileDialog ins_dlg(TRUE, L"json", L"*.json",
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, name_filter, NULL);

	// ���� ���� �޺��ڽ��� ������ ���͵� �߿��� 2��° �׸�(*.json)�� �����Ѵ�.
	ins_dlg.m_ofn.nFilterIndex = 1;

	if (ins_dlg.DoModal() == IDOK){
		// ���õ� ������ ��θ��� �̿��Ͽ� ��ť��Ʈ ������ �籸���Ѵ�.
		OpenDocumentFile(ins_dlg.GetPathName());
	}
	else {
		// ���� "���� ����" ��ɿ����� ���� ���⸦ ������� ��, Ư���� �޽����� ������ ������
		// �������� ȿ���� ��Ÿ���� ���ؼ� ���� ���⸦ ���������, ����ߴٴ� �޽����� ��������
		// �籸�� �߽��ϴ�.
		//::MessageBox(NULL, L"���� ���⸦ ����Ͽ����ϴ�.", L"�˸�", MB_ICONINFORMATION);
	}
}
