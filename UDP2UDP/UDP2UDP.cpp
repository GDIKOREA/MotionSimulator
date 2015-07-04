
// UDP2UDP.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "UDP2UDP.h"
#include "UDP2UDPDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


BEGIN_MESSAGE_MAP(CUDP2UDPApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


CUDP2UDPApp::CUDP2UDPApp()
{
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;
}

CUDP2UDPApp theApp;

BOOL CUDP2UDPApp::InitInstance()
{
// 	INITCOMMONCONTROLSEX InitCtrls;
// 	InitCtrls.dwSize = sizeof(InitCtrls);
// 	InitCtrls.dwICC = ICC_WIN95_CLASSES;
// 	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

//	AfxEnableControlContainer();

//	CShellManager *pShellManager = new CShellManager;
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	CUDP2UDPDlg *dlg;
	dlg = new CUDP2UDPDlg();
	dlg->Create(CUDP2UDPDlg::IDD);
	m_pMainWnd = dlg;
	dlg->ShowWindow(SW_SHOW);

	dlg->MainLoop();

	delete dlg;
	return FALSE;
}
