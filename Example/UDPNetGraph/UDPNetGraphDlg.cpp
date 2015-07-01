
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
	, m_IsPrintPacket(TRUE)
	, m_RcvCount(0)
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
	DDX_Control(pDX, IDC_RICHEDIT2_LOG, m_LogEdit);
	DDX_Check(pDX, IDC_CHECK_PRINT_PACKET, m_IsPrintPacket);
	DDX_Text(pDX, IDC_STATIC_RCV_COUNT, m_RcvCount);
	DDX_Control(pDX, IDC_BUTTON_CONNECT, m_ServerStartButton);
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
			buff[result] = NULL;
			ParsePacket(buff);
		}
	}
}


void CUDPNetGraphDlg::ParsePacket(char buff[128])
{
	if (m_IsPrintPacket)
	{
		CString str = str2wstr(buff).c_str();
		AppendToLogAndScroll(str, RGB(200, 200, 200));
	}

	++m_RcvCount;

	float v = *(float*)&buff[24];
	if (m_graphDlg)
	{
		m_graphDlg->SetGraphValue2(v);
	}

	UpdateData(FALSE);
}


void CUDPNetGraphDlg::OnBnClickedButtonConnect()
{
	UpdateData();

	if (m_isConnect)
	{
		closesocket(m_socket);
		AppendToLogAndScroll(L"서버 종료\n", RGB(255, 255, 255));
		m_ServerStartButton.SetWindowTextW(L"Server Start");
		m_isConnect = false;
	}
	else
	{
		if (network::LaunchUDPServer(m_Port, m_socket))
		{
			AppendToLogAndScroll(L"서버 실행 성공\n", RGB(255, 255, 255));
			m_ServerStartButton.SetWindowTextW(L"Server Stop");
			m_isConnect = true;
		}
		else
		{
			AppendToLogAndScroll(L"Error!! 서버 실행 실패\n", RGB(255, 255, 255));
		}
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
