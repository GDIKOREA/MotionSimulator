
// UDP2UDPDlg.cpp : implementation file
//

#include "stdafx.h"
#include "UDP2UDP.h"
#include "UDP2UDPDlg.h"
#include "afxdialogex.h"
#include <mmsystem.h>

#pragma comment(lib, "winmm.lib")

using namespace common;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CUDP2UDPDlg dialog
CUDP2UDPDlg::CUDP2UDPDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CUDP2UDPDlg::IDD, pParent)
	, m_loop(true)
	, m_RcvPort(0)
	, m_SndPort(0)
	, m_isConnect(false)
	, m_RcvCount(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CUDP2UDPDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_RCV_PORT, m_RcvPort);
	DDX_Control(pDX, IDC_IPADDRESS_SND_IP, m_SndIP);
	DDX_Text(pDX, IDC_EDIT_SND_PORT, m_SndPort);
	DDX_Control(pDX, IDC_BUTTON_START, m_StartButton);
	DDX_Text(pDX, IDC_STATIC_RCV_COUNT, m_RcvCount);
	DDX_Control(pDX, IDC_LIST_LOG, m_LogList);
}

BEGIN_MESSAGE_MAP(CUDP2UDPDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CUDP2UDPDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CUDP2UDPDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_START, &CUDP2UDPDlg::OnBnClickedButtonStart)
END_MESSAGE_MAP()


// CUDP2UDPDlg message handlers

BOOL CUDP2UDPDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	m_SndIP.SetAddress(127, 0, 0, 1);

	return TRUE;  // return TRUE  unless you set the focus to a control
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
	// TODO: Add your control notification handler code here
	//CDialogEx::OnOK();
}


void CUDP2UDPDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnCancel();
	m_loop = false;
}


void CUDP2UDPDlg::MainLoop()
{
	//메시지 구조체
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	int oldT = 0;
	while (m_loop)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		PacketProcess();
	}
}


void CUDP2UDPDlg::OnBnClickedButtonStart()
{
	UpdateData();

	if (m_isConnect)
	{
		closesocket(m_rcvSocket);
		Log("서버 종료");
 		m_StartButton.SetWindowTextW(L"Start");
		m_isConnect = false;
	}
	else
	{
		if (network::LaunchUDPServer(m_RcvPort, m_rcvSocket))
		{
			// 서버에 접속을 시도한다.
			DWORD address;
			m_SndIP.GetAddress(address);

			std::stringstream ss;
			ss << ((address & 0xff000000) >> 24) << "."
				<< ((address & 0x00ff0000) >> 16) << "."
				<< ((address & 0x0000ff00) >> 8) << "."
				<< (address & 0x000000ff);
			const string ip = ss.str();

			if (network::LaunchUDPClient(ip, m_SndPort, m_sockSndAddr, m_sndSocket))
			{
				Log("서버 실행 성공");
 				m_StartButton.SetWindowTextW(L"Stop");
				m_isConnect = true;
			}
			else
			{
				Log("Error!! 서버 실행 실패");
				closesocket(m_rcvSocket);
			}
		}
		else
		{
 			Log("Error!! 서버 실행 실패");
		}
	}
}


void CUDP2UDPDlg::PacketProcess()
{
	if (!m_isConnect)
		return;

	const timeval t = { 0, 1 }; // 10 millisecond
	fd_set readSockets;
	FD_ZERO(&readSockets);
	FD_SET(m_rcvSocket, &readSockets);

	const int ret = select(readSockets.fd_count, &readSockets, NULL, NULL, &t);
	if (ret != 0 && ret != SOCKET_ERROR)
	{
		char buff[128];
		const int result = recv(readSockets.fd_array[0], buff, sizeof(buff), 0);
		if (result == SOCKET_ERROR || result == 0) // 받은 패킷사이즈가 0이면 서버와 접속이 끊겼다는 의미다.
		{
			Log("서버와 연결이 끊김");
			closesocket(m_rcvSocket);
		}
		else
		{
			//buff[result] = NULL;
			//ParsePacket(buff);
			// 시리얼로 받은 정보를 UDP 네트워크를 통해 전송한다.
			int slen = sizeof(m_sockSndAddr);
			if (sendto(m_sndSocket, buff, result, 0, (struct sockaddr *) &m_sockSndAddr, slen) == SOCKET_ERROR)
			{
				//Log( format("sendto() failed with error code : %d", WSAGetLastError()) );
			}

			++m_RcvCount;
			UpdateData(FALSE);
		}
	}
}


// 로그 출력.
void CUDP2UDPDlg::Log(const string &str)
{
	m_LogList.InsertString(m_LogList.GetCount(), common::str2wstr(str).c_str());
	m_LogList.SetTopIndex(m_LogList.GetCount() - 1);
}
