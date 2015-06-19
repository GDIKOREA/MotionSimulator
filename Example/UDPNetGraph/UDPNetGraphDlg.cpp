
// UDPNetGraphDlg.cpp : implementation file
//

#include "stdafx.h"
#include "UDPNetGraph.h"
#include "UDPNetGraphDlg.h"
#include "afxdialogex.h"
#include <winsock2.h>
#include "GraphWindow.h"

#pragma comment(lib,"ws2_32.lib") //Winsock Library


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}



//------------------------------------------------------------------------
// 멀티바이트 문자를 유니코드로 변환
//------------------------------------------------------------------------
std::wstring str2wstr(const std::string &str)
{
	int len;
	int slength = (int)str.length() + 1;
	len = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	::MultiByteToWideChar(CP_ACP, 0, str.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}

//------------------------------------------------------------------------
// 유니코드를 멀티바이트 문자로 변환
//------------------------------------------------------------------------
std::string wstr2str(const std::wstring &wstr)
{
	const int slength = (int)wstr.length() + 1;
	const int len = ::WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), slength, 0, 0, NULL, FALSE);
	char* buf = new char[len];
	::WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), slength, buf, len, NULL, FALSE);
	std::string r(buf);
	delete[] buf;
	return r;
}



BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CUDPNetGraphDlg dialog



CUDPNetGraphDlg::CUDPNetGraphDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CUDPNetGraphDlg::IDD, pParent)
	, m_Port(10000)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_loop = true;
	m_isConnect = false;
	m_graphDlg = NULL;
}

void CUDPNetGraphDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_PORT, m_Port);
	DDX_Control(pDX, IDC_LIST_LOG, m_LogList);
}

BEGIN_MESSAGE_MAP(CUDPNetGraphDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CUDPNetGraphDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CUDPNetGraphDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_CONNECT, &CUDPNetGraphDlg::OnBnClickedButtonConnect)
	ON_BN_CLICKED(IDC_BUTTON_SHOWGRAPH, &CUDPNetGraphDlg::OnBnClickedButtonShowgraph)
END_MESSAGE_MAP()


// CUDPNetGraphDlg message handlers

BOOL CUDPNetGraphDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CUDPNetGraphDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CUDPNetGraphDlg::OnPaint()
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

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CUDPNetGraphDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CUDPNetGraphDlg::OnBnClickedOk()
{
	m_loop = false;
}


void CUDPNetGraphDlg::OnBnClickedCancel()
{
	m_loop = false;
}


void CUDPNetGraphDlg::MainLoop()
{
	//메시지 구조체
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	int oldT = 0;
	while (m_loop)
	{
		//PeekMessage 는 메시지 큐에 메시지가 없어도 프로그램이 멈추기 않고 진행이 된다.
		//이때 메시지큐에 메시지가 없으면 false 가 리턴되고 메시지가 있으면 true 가 리턴이된다.
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg); //눌린 키보드 의 문자를 번역하여 WM_CHAR 메시지를 발생시킨다.
			DispatchMessage(&msg);  //받아온 메시지 정보로 윈도우 프로시져 함수를 실행시킨다.
		}


		{
			PacketProcess();

			const int curT = timeGetTime();
			const int deltaT = curT - oldT;
			if (deltaT > 10)
			{
				oldT = curT;

				if (m_graphDlg)
					m_graphDlg->Update(deltaT/1000.f);
			}
		}
	}
}



void CUDPNetGraphDlg::PacketProcess()
{
	if (!m_isConnect)
		return;

	const timeval t = { 0, 1 }; // 10 millisecond
	fd_set readSockets;
	FD_ZERO(&readSockets);
	FD_SET(m_socket, &readSockets);

	const int ret = select(readSockets.fd_count, &readSockets, NULL, NULL, &t);
	if (ret != 0 && ret != SOCKET_ERROR)
	{
		char buff[128];
		const int result = recv(readSockets.fd_array[0], buff, sizeof(buff), 0);
		if (result == SOCKET_ERROR || result == 0) // 받은 패킷사이즈가 0이면 서버와 접속이 끊겼다는 의미다.
		{
			//m_ChatList.InsertString(m_ChatList.GetCount(), L"서버와 연결이 끊김");
			closesocket(m_socket);
		}
		else
		{
			ParsePacket(buff);
		}
	}
}



void CUDPNetGraphDlg::ParsePacket(char buff[128])
{
	float v = *(float*)&buff[32];
	if (m_graphDlg)
	{
		m_graphDlg->SetGraphValue2(v);
//		m_graphDlg->SetGraphValue(buff);
	}

// 	using namespace network;
// 	sPacketHeader *header = (sPacketHeader*)buff;
// 
// 	switch (header->protocol) 
// 	{
// 	case PROTOCOL::LOGIN:
// 		break;
// 
// 	case PROTOCOL::CHATTING:
// 	{
// 		const sChatProtocol *protocol = (sChatProtocol*)buff;
// 		const wstring wstr = str2wstr(protocol->msg);
// 		//m_ChatList.InsertString(m_ChatList.GetCount(), wstr.c_str());
// 	}
// 	break;
// 	}
}



void CUDPNetGraphDlg::OnBnClickedButtonConnect()
{
	UpdateData();
	if (network::LaunchUDPServer(m_Port, m_socket))
	{
		m_LogList.InsertString(m_LogList.GetCount(), L"접속 성공");
		m_isConnect = true;
	}
	else
	{
		m_LogList.InsertString(m_LogList.GetCount(), L"접속 실패");
		m_isConnect = false;
	}
}


void CUDPNetGraphDlg::OnBnClickedButtonShowgraph()
{
	if (!m_graphDlg)
	{
		m_graphDlg = new CGraphWindow();
		m_graphDlg->Create(CGraphWindow::IDD);
	}
	m_graphDlg->ShowWindow(SW_SHOW);
}
