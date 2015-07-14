
// UDP2UDPDlg.cpp : implementation file
//

#include "stdafx.h"
#include "UDP2UDP.h"
#include "UDP2UDPDlg.h"
#include "afxdialogex.h"
#include <mmsystem.h>
#include "ProxyWindow.h"

#pragma comment(lib, "winmm.lib")

using namespace common;

cUDP2UDPConfig g_config;


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CUDP2UDPDlg dialog
CUDP2UDPDlg::CUDP2UDPDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CUDP2UDPDlg::IDD, pParent)
	, m_loop(true)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CUDP2UDPDlg::~CUDP2UDPDlg()
{
	//----------------------------------------------------
	// 환경설정 파일을 저장하고 종료한다.
	g_config.m_sessions.clear();
	for each (auto &wnd in m_proxyWindows)
	{
		sSessionData data;
		data.receivePort = wnd->m_RcvPort;
		data.sendIP = wnd->GetSendIP();
		data.sendPort = wnd->m_SndPort;
		g_config.m_sessions.push_back(data);
	}
	//----------------------------------------------------


	for (u_int i = 0; i < m_proxyWindows.size(); ++i)
	{
		CProxyWindow *wnd = m_proxyWindows[i];
		if (wnd)
		{
			wnd->DestroyWindow();
			delete wnd;
			wnd = NULL;
		}
	}
}

void CUDP2UDPDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_LOG, m_LogList);
}

BEGIN_ANCHOR_MAP(CUDP2UDPDlg)
	ANCHOR_MAP_ENTRY(IDC_LIST_LOG, ANF_LEFT | ANF_RIGHT | ANF_BOTTOM)
END_ANCHOR_MAP()

BEGIN_MESSAGE_MAP(CUDP2UDPDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CUDP2UDPDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CUDP2UDPDlg::OnBnClickedCancel)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CUDP2UDPDlg message handlers

BOOL CUDP2UDPDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	InitAnchors();

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon


	InitProxyWindows();
	m_proxyWindows[0]->HideRemoveButton();

	return TRUE;
}


// 환경파일을 읽어서, ProxyWindow를 생성한다.
void CUDP2UDPDlg::InitProxyWindows()
{
	if (g_config.m_sessions.empty())
	{
		AddProxyWindow();
		return;
	}

	for each (auto &session in g_config.m_sessions)
	{
		if (CProxyWindow *wnd = AddProxyWindow())
		{
			wnd->Init(session.receivePort, session.sendIP, session.sendPort);
		}
	}
}


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

		if (incT > 10)
		{
			incT = 0;
			for each (auto &wnd in m_proxyWindows)
			{
				wnd->Update();
			}
		}
	}
}


// 로그 출력.
void CUDP2UDPDlg::Log(const string &str)
{
	m_LogList.InsertString(m_LogList.GetCount(), common::str2wstr(str).c_str());
	m_LogList.SetTopIndex(m_LogList.GetCount() - 1);
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
		AddProxyWindow(hWnd);
	}
	break;

	case IDC_BUTTON_REMOVE:
	{
		RemoveProxyWindow(hWnd);
		CalculateWindowSize();
	}
	break;

	default:
		return CDialogEx::OnCommand(wParam, lParam);
	}	

	return TRUE;
}


// ProxyWindow를 생성하고, 추가한다.
CProxyWindow* CUDP2UDPDlg::AddProxyWindow(const HWND instHwnd)
{
	const int W = 400;
	const int H = 135;

	const int x = 10;
	const int y = 10 + (m_proxyWindows.size() * (H+10));

	CProxyWindow *wnd = new CProxyWindow(this);
	wnd->Create(CProxyWindow::IDD, this);
	wnd->MoveWindow(CRect(x, y, x+W, y+H));
	wnd->ShowWindow(SW_SHOW);
	wnd->Init(8888, "192.168.0.1", 8889);
	m_proxyWindows.push_back(wnd);

	CRect wr;
	GetWindowRect(wr);

	const int dlgHeight = y + H + 160;
	MoveWindow(CRect(wr.left, wr.top, wr.right, wr.top+dlgHeight));

	return wnd;
}


// ProxyWindow 를 제거한다.
void CUDP2UDPDlg::RemoveProxyWindow(const HWND removeHwnd)
{
	for each(auto &wnd in m_proxyWindows)
	{
		if (wnd->GetSafeHwnd() == removeHwnd)
		{
			wnd->DestroyWindow();
			delete wnd;
			common::removevector(m_proxyWindows, wnd);
			break;
		}
	}
}


void CUDP2UDPDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	CRect rcWnd;
	GetWindowRect(&rcWnd);
	HandleAnchors(&rcWnd);
}


// 자식으로 생성된 ProxyWindow 위치를 재 설정한다.
void CUDP2UDPDlg::CalculateWindowSize()
{
	const int W = 400;
	const int H = 135;
	const int x = 10;
	int y = 10;

	for each(auto &wnd in m_proxyWindows)
	{
		wnd->MoveWindow(CRect(x, y, x + W, y + H));
		y += H + 10;
	}

	CRect wr;
	GetWindowRect(wr);
	const int dlgHeight = y + 150;
	MoveWindow(CRect(wr.left, wr.top, wr.right, wr.top + dlgHeight));
}


// 윈도우 메세지 핸들러
BOOL CUDP2UDPDlg::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	switch (message)
	{
	case WM_UDP2UDP_START_SUCCESS:
	{
		HWND hWnd = (HWND)lParam;
		if (CProxyWindow *wnd = FindProxyWindow(hWnd))
		{
			Log(common::format("Port = %d, 접속 성공", wnd->m_RcvPort));
		}
	}
	break;

	case WM_UDP2UDP_START_FAIL:
	{
		HWND hWnd = (HWND)lParam;
		if (CProxyWindow *wnd = FindProxyWindow(hWnd))
		{
			Log(common::format("Port = %d, 접속 실패!!!", wnd->m_RcvPort));
		}
	}
	break;

	case WM_UDP2UDP_STOP:
	{
		HWND hWnd = (HWND)lParam;
		if (CProxyWindow *wnd = FindProxyWindow(hWnd))
		{
			Log(common::format("Port = %d, 접속 종료", wnd->m_RcvPort));
		}
	}
	break;


	default:
		return CDialogEx::OnWndMsg(message, wParam, lParam, pResult);
		break;
	}

	return TRUE;
}


// proxyWindow중 hWnd 핸들인 것을 찾아 리턴한다.
CProxyWindow* CUDP2UDPDlg::FindProxyWindow(const HWND hWnd)
{
	for each(auto &wnd in m_proxyWindows)
	{
		if (wnd->GetSafeHwnd() == hWnd)
			return wnd;
	}
	return NULL;
}
