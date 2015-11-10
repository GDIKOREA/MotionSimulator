
// UDP2UDPDlg.cpp : implementation file
//

#include "stdafx.h"
#include "UDP2UDP.h"
#include "UDP2UDPDlg.h"
#include "afxdialogex.h"
#include <mmsystem.h>
#include "ProxyWindow.h"
#include "settingconfigparser.h"
#include "multiproxy.h"


#pragma comment(lib, "winmm.lib")

using namespace common;

//cUDP2UDPConfig g_config;


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CUDP2UDPDlg dialog
CUDP2UDPDlg::CUDP2UDPDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CUDP2UDPDlg::IDD, pParent)
	, m_loop(true)
	, m_state(STOP)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CUDP2UDPDlg::~CUDP2UDPDlg()
{
	//----------------------------------------------------
	// 환경설정 파일을 저장하고 종료한다.
//	g_config.m_sessions.clear();
// 	for each (auto &wnd in m_proxyWindows)
// 	{
// 		sSessionData data;
// 		data.receivePort = wnd->m_RcvPort;
// 		data.sendIP = wnd->GetSendIP();
// 		data.sendPort = wnd->m_SndPort;
// 		g_config.m_sessions.push_back(data);
// 	}
	//----------------------------------------------------

	using namespace std;
	ofstream ofs("udp2udpconfig.cfg");
	if (ofs.is_open())
	{
// 		stringstream ss;
// 		ss << ofs.rdbuf();
// 		m_editScript.SetWindowTextW(str2wstr(ss.str()).c_str());
		CString wstr;
		m_editScript.GetWindowTextW(wstr);
		string str = wstr2str((LPCTSTR)wstr);
		ofs << str;
	}

	CloseAll();

// 	for (u_int i = 0; i < m_proxyWindows.size(); ++i)
// 	{
// 		CProxyWindow *wnd = m_proxyWindows[i];
// 		if (wnd)
// 		{
// 			wnd->DestroyWindow();
// 			delete wnd;
// 			wnd = NULL;
// 		}
// 	}
}

void CUDP2UDPDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_LIST_LOG, m_LogList);
	DDX_Control(pDX, IDC_EDIT_SCRIPT, m_editScript);
	DDX_Control(pDX, IDC_LIST_STATE, m_listState);
	DDX_Control(pDX, IDC_BUTTON_START, m_StartButton);
}

BEGIN_ANCHOR_MAP(CUDP2UDPDlg)
	ANCHOR_MAP_ENTRY(IDC_EDIT_SCRIPT, ANF_LEFT | ANF_RIGHT | ANF_TOP | ANF_BOTTOM)
	ANCHOR_MAP_ENTRY(IDC_BUTTON_START, ANF_RIGHT | ANF_BOTTOM)
	ANCHOR_MAP_ENTRY(IDC_BUTTON_HELP, ANF_RIGHT | ANF_BOTTOM)
	ANCHOR_MAP_ENTRY(IDC_BUTTON_PASTE_SAMPLE, ANF_RIGHT | ANF_BOTTOM)
	ANCHOR_MAP_ENTRY(IDC_LIST_STATE, ANF_LEFT | ANF_RIGHT | ANF_BOTTOM)
//	ANCHOR_MAP_ENTRY(IDC_LIST_LOG, ANF_LEFT | ANF_RIGHT | ANF_BOTTOM)
END_ANCHOR_MAP()

BEGIN_MESSAGE_MAP(CUDP2UDPDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CUDP2UDPDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CUDP2UDPDlg::OnBnClickedCancel)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_START, &CUDP2UDPDlg::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_HELP, &CUDP2UDPDlg::OnBnClickedButtonHelp)
	ON_BN_CLICKED(IDC_BUTTON_PASTE_SAMPLE, &CUDP2UDPDlg::OnBnClickedButtonPasteSample)
END_MESSAGE_MAP()


// CUDP2UDPDlg message handlers

BOOL CUDP2UDPDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	InitAnchors();

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	m_listState.InsertColumn(0, L"Server Port");
	m_listState.InsertColumn(1, L"Client");
	m_listState.InsertColumn(2, L"State");
	m_listState.InsertColumn(3, L"Recv");

	m_listState.SetColumnWidth(0, 70);
	m_listState.SetColumnWidth(1, 150);
	m_listState.SetColumnWidth(2, 80);
	m_listState.SetColumnWidth(3, 70);

	using namespace std;
	ifstream ifs("udp2udpconfig.cfg");
	if (ifs.is_open())
	{
		stringstream ss;
		ss << ifs.rdbuf();
		m_editScript.SetWindowTextW(str2wstr(ss.str()).c_str());
	}

	// Topmost 해제
	::SetWindowPos(GetSafeHwnd(), HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOREDRAW | SWP_NOSIZE );
	return TRUE;
}


void CUDP2UDPDlg::InitListControl()
{
	m_listState.DeleteAllItems();

	const int size = sizeof(m_RecvCountBuffer) / sizeof(int);
	for (int i = 0; i < size; ++i)
		m_RecvCountBuffer[i] = -1;

	int index = 0;
	for each (auto proxy in m_multiProxy)
	{
		int clientCount;
		network::cUDPClient *clients[10];
		network::cUDPServer *svr = proxy->GetState(clients, sizeof(clients) / sizeof(network::cUDPClient*), clientCount);

		m_listState.InsertItem(index, str2wstr(format("%d", svr->m_port)).c_str());
		m_listState.SetItemText(index, 2, svr->IsConnect() ? L"Connect" : L"Disconnect");
		m_listState.SetItemData(index, svr->IsConnect());
		++index;

		for (int i = 0; i < clientCount; ++i)
		{
			m_listState.InsertItem(index, L"");
			m_listState.SetItemText(index, 1, 
				str2wstr(format("%s (%d)", clients[i]->m_ip.c_str(), clients[i]->m_port)).c_str());
			m_listState.SetItemText(index, 2, clients[i]->IsConnect() ? L"Connect" : L"Disconnect");
			m_listState.SetItemData(index, clients[i]->IsConnect());
			++index;
		}
	}
}


void CUDP2UDPDlg::UpdateListControl()
{
	int index = 0;
	for each (auto proxy in m_multiProxy)
	{
		int clientCount;
		network::cUDPClient *clients[10];
		network::cUDPServer *svr = proxy->GetState(clients, sizeof(clients) / sizeof(network::cUDPClient*), clientCount);

		if (m_listState.GetItemData(index) != (DWORD)svr->IsConnect())
		{
			m_listState.SetItemText(index, 2, svr->IsConnect() ? L"Connect" : L"Disconnect");
			m_listState.SetItemData(index, svr->IsConnect());
		}
		if (m_RecvCountBuffer[index] != proxy->m_ReceiveCount)
		{
			m_listState.SetItemText(index, 3, str2wstr(format("%d", proxy->m_ReceiveCount)).c_str());
			m_RecvCountBuffer[index] = proxy->m_ReceiveCount;
		}

		++index;

		for (int i = 0; i < clientCount; ++i)
		{
			if (m_listState.GetItemData(index) != (DWORD)clients[i]->IsConnect())
			{
				m_listState.SetItemText(index, 2, clients[i]->IsConnect() ? L"Connect" : L"Disconnect");
				m_listState.SetItemData(index, clients[i]->IsConnect());
			}
			++index;
		}
	}
}


// 환경파일을 읽어서, ProxyWindow를 생성한다.
// void CUDP2UDPDlg::InitProxyWindows()
// {
// 	if (g_config.m_sessions.empty())
// 	{
// 		AddProxyWindow();
// 		return;
// 	}
// 
// 	for each (auto &session in g_config.m_sessions)
// 	{
// 		if (CProxyWindow *wnd = AddProxyWindow())
// 		{
// 			wnd->Init(session.receivePort, session.sendIP, session.sendPort);
// 		}
// 	}
// }


void CUDP2UDPDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}


HCURSOR CUDP2UDPDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CUDP2UDPDlg::OnBnClickedOk()
{
	//CDialogEx::OnOK();
}


void CUDP2UDPDlg::OnBnClickedCancel()
{
	CDialogEx::OnCancel();
	m_loop = false;
}


void CUDP2UDPDlg::MainLoop()
{
	//메시지 구조체
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	int oldT = timeGetTime();
	int incT = 0;
	while (m_loop)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		const int curT = timeGetTime();
		const int deltaT = curT - oldT;
		incT += deltaT;

		for each (auto &proxy in m_multiProxy)
			proxy->Update();

		if (incT > 100)
		{
			incT = 0;
			UpdateListControl();
		}
	}
}


// 로그 출력.
void CUDP2UDPDlg::Log(const string &str)
{
// 	m_LogList.InsertString(m_LogList.GetCount(), common::str2wstr(str).c_str());
// 	m_LogList.SetTopIndex(m_LogList.GetCount() - 1);
}


// ProxyWindow 버튼 메세지를 받는다.
BOOL CUDP2UDPDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	const int buttonID = LOWORD(wParam);
	const HWND hWnd = (HWND)lParam;

	switch (buttonID)
	{
	case IDC_BUTTON_ADD:
	{
		//AddProxyWindow(hWnd);
	}
	break;

	case IDC_BUTTON_REMOVE:
	{
		//RemoveProxyWindow(hWnd);
		//CalculateWindowSize();
	}
	break;

	default:
		return CDialogEx::OnCommand(wParam, lParam);
	}	

	return TRUE;
}


// ProxyWindow를 생성하고, 추가한다.
// CProxyWindow* CUDP2UDPDlg::AddProxyWindow(const HWND instHwnd)
// {
// 	const int W = 400;
// 	const int H = 135;
// 
// 	const int x = 10;
// 	const int y = 10 + (m_proxyWindows.size() * (H+10));
// 
// 	CProxyWindow *wnd = new CProxyWindow(this);
// 	wnd->Create(CProxyWindow::IDD, this);
// 	wnd->MoveWindow(CRect(x, y, x+W, y+H));
// 	wnd->ShowWindow(SW_SHOW);
// 	wnd->Init(8888, "192.168.0.1", 8889);
// 	m_proxyWindows.push_back(wnd);
// 
// 	CRect wr;
// 	GetWindowRect(wr);
// 
// 	const int dlgHeight = y + H + 160;
// 	MoveWindow(CRect(wr.left, wr.top, wr.right, wr.top+dlgHeight));
// 
// 	return wnd;
// }
// 
// 
// // ProxyWindow 를 제거한다.
// void CUDP2UDPDlg::RemoveProxyWindow(const HWND removeHwnd)
// {
// 	for each(auto &wnd in m_proxyWindows)
// 	{
// 		if (wnd->GetSafeHwnd() == removeHwnd)
// 		{
// 			wnd->DestroyWindow();
// 			delete wnd;
// 			common::removevector(m_proxyWindows, wnd);
// 			break;
// 		}
// 	}
// }


void CUDP2UDPDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	CRect rcWnd;
	GetWindowRect(&rcWnd);
	HandleAnchors(&rcWnd);
}


// 자식으로 생성된 ProxyWindow 위치를 재 설정한다.
// void CUDP2UDPDlg::CalculateWindowSize()
// {
// 	const int W = 400;
// 	const int H = 135;
// 	const int x = 10;
// 	int y = 10;
// 
// 	for each(auto &wnd in m_proxyWindows)
// 	{
// 		wnd->MoveWindow(CRect(x, y, x + W, y + H));
// 		y += H + 10;
// 	}
// 
// 	CRect wr;
// 	GetWindowRect(wr);
// 	const int dlgHeight = y + 150;
// 	MoveWindow(CRect(wr.left, wr.top, wr.right, wr.top + dlgHeight));
// }


// 윈도우 메세지 핸들러
BOOL CUDP2UDPDlg::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	switch (message)
	{
	case WM_UDP2UDP_START_SUCCESS:
	{
//		HWND hWnd = (HWND)lParam;
// 		if (CProxyWindow *wnd = FindProxyWindow(hWnd))
// 		{
// 			Log(common::format("Port = %d, 접속 성공", wnd->m_RcvPort));
// 		}
	}
	break;

	case WM_UDP2UDP_START_FAIL:
	{
// 		HWND hWnd = (HWND)lParam;
// 		if (CProxyWindow *wnd = FindProxyWindow(hWnd))
// 		{
// 			Log(common::format("Port = %d, 접속 실패!!!", wnd->m_RcvPort));
// 		}
	}
	break;

	case WM_UDP2UDP_STOP:
	{
// 		HWND hWnd = (HWND)lParam;
// 		if (CProxyWindow *wnd = FindProxyWindow(hWnd))
// 		{
// 			Log(common::format("Port = %d, 접속 종료", wnd->m_RcvPort));
// 		}
	}
	break;


	default:
		return CDialogEx::OnWndMsg(message, wParam, lParam, pResult);
		break;
	}

	return TRUE;
}


// proxyWindow중 hWnd 핸들인 것을 찾아 리턴한다.
// CProxyWindow* CUDP2UDPDlg::FindProxyWindow(const HWND hWnd)
// {
// 	for each(auto &wnd in m_proxyWindows)
// 	{
// 		if (wnd->GetSafeHwnd() == hWnd)
// 			return wnd;
// 	}
// 	return NULL;
// }


void CUDP2UDPDlg::OnBnClickedButtonStart()
{
	if (STOP == m_state)
	{
		CloseAll();

		CString str;
		m_editScript.GetWindowTextW(str);

		cSettingConfigParser parser;
		vector<cSettingConfigParser::sUDPConnect> connectList;
		parser.ParseStr(common::wstr2str((LPCTSTR)str), connectList);

		for each (auto con in connectList)
		{
			cMultiProxy *proxy = new cMultiProxy();
			proxy->Init(con);
			m_multiProxy.push_back(proxy);
		}

		m_state = START;
		m_StartButton.SetWindowTextW(L"Stop");
		InitListControl();
	}
	else
	{
		CloseAll();
		m_listState.DeleteAllItems();

		m_state = STOP;
		m_StartButton.SetWindowTextW(L"Start");
	}

}


void CUDP2UDPDlg::OnBnClickedButtonHelp()
{
	CString str =
		L"Sample\n"
		L"\n"
		L"recv_port = 1100\n"
		L"	send_ip = 127.0.0.1\n"
		L"	send_port = 10001\n"
		L"	send_ip = 127.0.0.1\n"
		L"	send_port = 10002\n"
		L"\n"
		L"recv_port = 1102\n"
		L"	send_ip = 127.0.0.1\n"
		L"	send_port = 11001\n"
		L"	send_ip = 127.0.0.1\n"
		L"	send_port = 11002\n";
	AfxMessageBox(str);
}


void CUDP2UDPDlg::CloseAll()
{
	for each(auto &proxy in m_multiProxy)
	{
		proxy->Close();
		delete proxy;
	}
	m_multiProxy.clear();
}


void CUDP2UDPDlg::OnBnClickedButtonPasteSample()
{
	if (IDYES == AfxMessageBox(L"Paste Sample Script to Editor", MB_YESNO))
	{
		CString str =
			L"recv_port = 1100\r\n"
			L"	send_ip = 127.0.0.1\r\n"
			L"	send_port = 10001\r\n"
			L"	send_ip = 127.0.0.1\r\n"
			L"	send_port = 10002\r\n"
			L"\r\n"
			L"recv_port = 1102\r\n"
			L"	send_ip = 127.0.0.1\r\n"
			L"	send_port = 11001\r\n"
			L"	send_ip = 127.0.0.1\r\n"
			L"	send_port = 11002\r\n";
		m_editScript.SetWindowTextW(str);
	}
}
