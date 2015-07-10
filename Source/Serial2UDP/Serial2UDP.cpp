
#include "stdafx.h"
#include "Serial2UDP.h"
#include "Serial2UDPDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


BEGIN_MESSAGE_MAP(CSerial2UDPApp, CWinApp)
END_MESSAGE_MAP()


CSerial2UDPApp::CSerial2UDPApp()
{
}

CSerial2UDPApp theApp;

BOOL CSerial2UDPApp::InitInstance()
{
	CWinApp::InitInstance();

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


	//CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	CSerial2UDPDlg *dlg;
	dlg = new CSerial2UDPDlg();
	dlg->Create(CSerial2UDPDlg::IDD, NULL);
	m_pMainWnd = dlg;
	dlg->ShowWindow(SW_SHOW);
	dlg->MainLoop();

	dlg->DestroyWindow();
	delete dlg;

	//Close down COM
	CoUninitialize();

	return FALSE;
}

