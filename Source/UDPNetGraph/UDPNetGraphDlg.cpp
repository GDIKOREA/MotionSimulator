
// UDPNetGraphDlg.cpp : implementation file
//

#include "stdafx.h"
#include "UDPNetGraph.h"
#include "UDPNetGraphDlg.h"
#include "afxdialogex.h"
#include <winsock2.h>
#include "GraphWindow.h"
#include "MiniFrame.h"
#include "udpnetgraphconfig.h"

#pragma comment(lib,"ws2_32.lib") //Winsock Library


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


cUDPNetGraphConfig g_config;



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




// CUDPNetGraphDlg dialog



CUDPNetGraphDlg::CUDPNetGraphDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CUDPNetGraphDlg::IDD, pParent)
	, m_Port(10000)
	, m_IsPrintPacket(TRUE)
	, m_RcvCount(0)
	, m_IsPrintMemory(FALSE)
	, m_MaxLine(40)
	, m_IsPrintHexa(FALSE)
	, m_ConnectRadioType(1)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_loop = true;
	m_graphDlg = NULL;
}

void CUDPNetGraphDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_PORT, m_Port);
	DDX_Control(pDX, IDC_RICHEDIT2_LOG, m_LogEdit);
	DDX_Check(pDX, IDC_CHECK_PRINT_PACKET, m_IsPrintPacket);
	DDX_Text(pDX, IDC_STATIC_RCV_COUNT, m_RcvCount);
	DDX_Control(pDX, IDC_BUTTON_CONNECT, m_ServerStartButton);
	DDX_Check(pDX, IDC_CHECK_PRINT_MEMORY, m_IsPrintMemory);
	DDX_Text(pDX, IDC_EDIT_MAX_LINE, m_MaxLine);
	DDX_Check(pDX, IDC_CHECK_PRINT_HEXA, m_IsPrintHexa);
	DDX_Radio(pDX, IDC_RADIO_CLIENT, m_ConnectRadioType);
	DDX_Control(pDX, IDC_IPADDRESS_IP, m_IpAddr);
	DDX_Control(pDX, IDC_STATIC_IP, m_StaticIP);
}

BEGIN_MESSAGE_MAP(CUDPNetGraphDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CUDPNetGraphDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CUDPNetGraphDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_CONNECT, &CUDPNetGraphDlg::OnBnClickedButtonConnect)
	ON_BN_CLICKED(IDC_BUTTON_SHOWGRAPH, &CUDPNetGraphDlg::OnBnClickedButtonShowgraph)
	ON_BN_CLICKED(IDC_CHECK_PRINT_PACKET, &CUDPNetGraphDlg::OnBnClickedCheckPrintPacket)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_CHECK_PRINT_MEMORY, &CUDPNetGraphDlg::OnBnClickedCheckPrintMemory)
	ON_EN_CHANGE(IDC_EDIT_MAX_LINE, &CUDPNetGraphDlg::OnEnChangeEditMaxLine)
	ON_BN_CLICKED(IDC_CHECK_PRINT_HEXA, &CUDPNetGraphDlg::OnBnClickedCheckPrintHexa)
	ON_BN_CLICKED(IDC_RADIO_CLIENT, &CUDPNetGraphDlg::OnBnClickedRadioClient)
	ON_BN_CLICKED(IDC_RADIO_SERVER, &CUDPNetGraphDlg::OnBnClickedRadioServer)
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

		
	m_LogEdit.SetBackgroundColor(FALSE, RGB(0, 0, 0));

	m_font.CreateFont(14,                     // 글자높이
		10,                     // 글자너비
		0,                      // 출력각도
		0,                      // 기준 선에서의각도
		FW_HEAVY,              // 글자굵기
		FALSE,                  // Italic 적용여부
		FALSE,                  // 밑줄적용여부
		FALSE,                  // 취소선적용여부
		DEFAULT_CHARSET,       // 문자셋종류
		OUT_DEFAULT_PRECIS,    // 출력정밀도
		CLIP_DEFAULT_PRECIS,   // 클리핑정밀도
		DEFAULT_QUALITY,       // 출력문자품질
		DEFAULT_PITCH,         // 글꼴Pitch
		_T("굴림체")           // 글꼴
		);
	m_LogEdit.SetFont(&m_font);


	//g_config.m_ip;
	vector<string> token;
	common::tokenizer(g_config.m_ip, ".", "", token);
	if (token.size() >= 4)
	{
		m_IpAddr.SetAddress(
			atoi(token[0].c_str()),
			atoi(token[1].c_str()),
			atoi(token[2].c_str()),
			atoi(token[3].c_str()));
	}
	else
	{
		m_IpAddr.SetAddress(127, 0, 0, 1);
	}

	m_ConnectRadioType = g_config.m_agentType;
	m_Port = g_config.m_port;

// 	CMiniFrame *frm = new CMiniFrame();
// 	frm->Create(NULL, NULL, WS_VISIBLE | WS_SYSMENU | WS_SIZEBOX, CRect(0, 0, 100, 100));
// 	frm->ShowWindow(SW_SHOW);

	if (1 == m_ConnectRadioType)
	{
		m_IpAddr.ShowWindow(SW_HIDE);
		m_StaticIP.ShowWindow(SW_HIDE);
	}

	UpdateData(FALSE);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CUDPNetGraphDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
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



void CUDPNetGraphDlg::PacketProcess()
{
	if (0 == m_ConnectRadioType)
	{
 		// Client
		char buff[512];

		// 주기적으로 서버에게 패킷을 보내고, 받는다. 1초에 30번을 보낸다.
		if ((timeGetTime() - m_clientSndTime) < 33)
			return;

		m_clientSndTime = timeGetTime();

		m_udpClient.SendData(buff, 1);

		if (const int result = m_udpClient.GetReceiveData(buff, sizeof(buff)))
		{
			buff[result] = NULL;
			ParsePacket(buff, result);
		}
	}
	else
	{
 		// Server 
		char buff[512];
		if (const int result = m_udpServer.GetRecvData(buff, sizeof(buff)))
		{
			buff[result] = NULL;
			ParsePacket(buff, result);
		}

	}
}


void CUDPNetGraphDlg::ParsePacket(char buff[128], const int buffLen)
{
	if (m_IsPrintPacket)
	{
		if (m_IsPrintMemory)
		{
			int maxLen = m_MaxLine;
			if (m_MaxLine <= 0)
				maxLen = 10;

			CString str;
			for (int i = 0; i < buffLen; ++i)
			{
				CString s;
				s.Format(L"%c", (BYTE)buff[i]);
				s.MakeUpper();
				str += s;
				if (((i + 1) % maxLen) == 0)
					str += L"\n";
			}
			AppendToLogAndScroll(str + L"\n", RGB(200, 200, 200));
		}
		else if (m_IsPrintHexa)
		{
			int maxLen = m_MaxLine;
			if (m_MaxLine <= 0)
				maxLen = 10;

			CString str;
			for (int i = 0; i < buffLen; ++i)
			{
				CString s;
				s.Format(L"%2x", (BYTE)buff[i]);
				s.MakeUpper();
				str += s;
				if (((i + 1) % maxLen) == 0)
					str += L"\n";
			}
			AppendToLogAndScroll(str + L"\n", RGB(200, 200, 200));
		}
		else
		{
			CString str = str2wstr(buff).c_str();
			AppendToLogAndScroll(str, RGB(200, 200, 200));
		}
	}

	++m_RcvCount;

//	float v = *(float*)&buff[24];
	if (m_graphDlg)
	{
//		m_graphDlg->SetGraphValue2(v);
		m_graphDlg->SetGraphValue(buff);
	}

	UpdateData(FALSE);
}


void CUDPNetGraphDlg::OnBnClickedButtonConnect()
{
	UpdateData();

	if (0 == m_ConnectRadioType)
	{
		StartClient();
	}
	else
	{
		StartServer();
	}

	g_config.m_agentType = m_ConnectRadioType;
	g_config.m_port = m_Port;

	{
		DWORD address;
		m_IpAddr.GetAddress(address);
		std::stringstream ss;
		ss << ((address & 0xff000000) >> 24) << "."
			<< ((address & 0x00ff0000) >> 16) << "."
			<< ((address & 0x0000ff00) >> 8) << "."
			<< (address & 0x000000ff);
		g_config.m_ip = ss.str();
	}

}


// 클라이언트 시작
bool CUDPNetGraphDlg::StartClient()
{
	UpdateData();

	if (m_udpClient.IsConnect())
	{
		m_udpClient.Close(true);
		AppendToLogAndScroll(L"클라이언트 종료\n", RGB(255, 255, 255));
		m_ServerStartButton.SetWindowTextW(L"Start");
	}
	else
	{
		DWORD address;
		m_IpAddr.GetAddress(address);
		std::stringstream ss;
		ss << ((address & 0xff000000) >> 24) << "."
			<< ((address & 0x00ff0000) >> 16) << "."
			<< ((address & 0x0000ff00) >> 8) << "."
			<< (address & 0x000000ff);
		const string ip = ss.str();
 		if (m_udpClient.Init(ip, m_Port))		{			AppendToLogAndScroll(L"클라이언트 실행 성공\n", RGB(255, 255, 255));
			m_ServerStartButton.SetWindowTextW(L"Client Stop");
		}
		else
		{
			AppendToLogAndScroll(L"Error!! 클라이언트 실행 실패\n", RGB(255, 255, 255));
		}
	}

	return true;
}


// 서버 시작.
bool CUDPNetGraphDlg::StartServer()
{
	UpdateData();

	if (m_udpServer.IsConnect())
	{
		m_udpServer.Close(true);

		AppendToLogAndScroll(L"서버 종료\n", RGB(255, 255, 255));
		m_ServerStartButton.SetWindowTextW(L"Start");
	}
	else
	{
		if (m_udpServer.Init(0, m_Port))
		{
			AppendToLogAndScroll(L"서버 실행 성공\n", RGB(255, 255, 255));
			m_ServerStartButton.SetWindowTextW(L"Server Stop");
		}
		else
		{
			AppendToLogAndScroll(L"Error!! 서버 실행 실패\n", RGB(255, 255, 255));
		}
	}

	return true;
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


void CUDPNetGraphDlg::OnBnClickedCheckPrintPacket()
{
	UpdateData();
}



// http://www.codeproject.com/Articles/12093/Using-RichEditCtrl-to-Display-Formatted-Logs
int CUDPNetGraphDlg::AppendToLogAndScroll(CString str, COLORREF color)
{
	long nVisible = 0;
	long nInsertionPoint = 0;
	CHARFORMAT cf;

	// Initialize character format structure
	cf.cbSize = sizeof(CHARFORMAT);
	cf.dwMask = CFM_COLOR;
	cf.dwEffects = 0; // To disable CFE_AUTOCOLOR

	cf.crTextColor = color;

	// Set insertion point to end of text
	nInsertionPoint = m_LogEdit.GetWindowTextLength();
	m_LogEdit.SetSel(nInsertionPoint, -1);

	// Set the character format
	m_LogEdit.SetSelectionCharFormat(cf);

	// Replace selection. Because we have nothing
	// selected, this will simply insert
	// the string at the current caret position.
	m_LogEdit.ReplaceSel(str);

	// Get number of currently visible lines or maximum number of visible lines
	// (We must call GetNumVisibleLines() before the first call to LineScroll()!)
	nVisible = GetNumVisibleLines(&m_LogEdit);

	// Now this is the fix of CRichEditCtrl's abnormal behaviour when used
	// in an application not based on dialogs. Checking the focus prevents
	// us from scrolling when the CRichEditCtrl does so automatically,
	// even though ES_AUTOxSCROLL style is NOT set.
	if (&m_LogEdit != m_LogEdit.GetFocus())
	{
		m_LogEdit.LineScroll(INT_MAX);
		m_LogEdit.LineScroll(1 - nVisible);
	}

	// 내용이 너무 많으면 지운다.
	const int maximumLine = 100;
	if (m_LogEdit.GetLineCount() > maximumLine)
	{
		long nFirstChar = m_LogEdit.CharFromPos(CPoint(0, 0));
		m_LogEdit.SetSel(0, nFirstChar);
		m_LogEdit.ReplaceSel(L"");
	}

	return 0;
}


//http://www.codeproject.com/Articles/12093/Using-RichEditCtrl-to-Display-Formatted-Logs
int CUDPNetGraphDlg::GetNumVisibleLines(CRichEditCtrl* pCtrl)
{
	CRect rect;
	long nFirstChar, nLastChar;
	long nFirstLine, nLastLine;

	// Get client rect of rich edit control
	pCtrl->GetClientRect(rect);

	// Get character index close to upper left corner
	nFirstChar = pCtrl->CharFromPos(CPoint(0, 0));

	// Get character index close to lower right corner
	nLastChar = pCtrl->CharFromPos(CPoint(rect.right, rect.bottom));
	if (nLastChar < 0)
	{
		nLastChar = pCtrl->GetTextLength();
	}

	// Convert to lines
	nFirstLine = pCtrl->LineFromChar(nFirstChar);
	nLastLine = pCtrl->LineFromChar(nLastChar);

	return (nLastLine - nFirstLine);
}


void CUDPNetGraphDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	if (m_LogEdit.GetSafeHwnd())
	{
		CRect rect;
		m_LogEdit.GetWindowRect(rect);

		ScreenToClient(rect);

		rect.right = rect.left + cx - 20;
		rect.bottom = cy - 13;
		m_LogEdit.MoveWindow(rect);
	}
}


void CUDPNetGraphDlg::OnBnClickedCheckPrintMemory()
{
	UpdateData();
}

void CUDPNetGraphDlg::OnEnChangeEditMaxLine()
{
	UpdateData();
}

void CUDPNetGraphDlg::OnBnClickedCheckPrintHexa()
{
	UpdateData();
}

void CUDPNetGraphDlg::OnBnClickedRadioClient()
{
	UpdateData();

	m_IpAddr.ShowWindow(SW_SHOW);
	m_StaticIP.ShowWindow(SW_SHOW);
}

void CUDPNetGraphDlg::OnBnClickedRadioServer()
{
	UpdateData();

	m_IpAddr.ShowWindow(SW_HIDE);
	m_StaticIP.ShowWindow(SW_HIDE);
}
