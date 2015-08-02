// ProxyWindow.cpp : implementation file
//

#include "stdafx.h"
#include "UDP2UDP.h"
#include "ProxyWindow.h"
#include "afxdialogex.h"


// CProxyWindow dialog
CProxyWindow::CProxyWindow(CWnd* pParent /*=NULL*/)
	: CDialogEx(CProxyWindow::IDD, pParent)
	, m_RcvPort(0)
	, m_SndPort(0)
	, m_ReceiveCount(0)
	, m_isStart(false)
{
}

CProxyWindow::~CProxyWindow()
{
	m_udpReceiveServer.Close();
	m_udpSendClient.Close();
}

void CProxyWindow::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_START, m_StartButton);
	DDX_Text(pDX, IDC_EDIT_RCV_PORT, m_RcvPort);
	DDX_Text(pDX, IDC_EDIT_SND_PORT, m_SndPort);
	DDX_Text(pDX, IDC_STATIC_RCV_COUNT, m_ReceiveCount);
	DDX_Control(pDX, IDC_IPADDRESS_SND_IP, m_SndIP);
	DDX_Control(pDX, IDC_BUTTON_REMOVE, m_RemoveButton);
	DDX_Control(pDX, IDC_EDIT_RCV_PORT, m_EditReceivePort);
	DDX_Control(pDX, IDC_EDIT_SND_PORT, m_EditSendPort);
}


BEGIN_MESSAGE_MAP(CProxyWindow, CDialogEx)
	ON_BN_CLICKED(IDOK, &CProxyWindow::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CProxyWindow::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_START, &CProxyWindow::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CProxyWindow::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_REMOVE, &CProxyWindow::OnBnClickedButtonRemove)
END_MESSAGE_MAP()


// CProxyWindow message handlers
BOOL CProxyWindow::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_SndIP.SetAddress(127, 0, 0, 1);
	return TRUE;
}


void CProxyWindow::OnBnClickedOk()
{
	//CDialogEx::OnOK();
}


void CProxyWindow::OnBnClickedCancel()
{
	//CDialogEx::OnCancel();
}


// 주기적으로 패킷을 받았는지 체크한다.
void CProxyWindow::Update()
{
	RET(!m_isStart);

	using namespace network;

	if (m_udpReceiveServer.m_isReceiveData)
	{
		char buff[cUDPServer::BUFLEN];
		if (const int buffLen = m_udpReceiveServer.GetRecvData(buff, cUDPServer::BUFLEN))
		{
			m_udpSendClient.SendData(buff, buffLen);

			++m_ReceiveCount;
			UpdateData(FALSE);
		}
	}
}


void CProxyWindow::OnBnClickedButtonStart()
{
	UpdateData();

	if (m_isStart)
	{
		m_isStart = false;
		m_udpReceiveServer.Close();
		m_udpSendClient.Close();

		m_StartButton.SetWindowTextW(L"Start");
		m_SndIP.EnableWindow(TRUE);
		m_EditReceivePort.EnableWindow(TRUE);
		m_EditSendPort.EnableWindow(TRUE);

		GetParent()->PostMessageW(WM_UDP2UDP_STOP, 0, (LPARAM)GetSafeHwnd());
	}
	else
	{
		const string ip = GetSendIP();

		if (!m_udpReceiveServer.Init(0, m_RcvPort))
		{
			GetParent()->PostMessageW(WM_UDP2UDP_START_FAIL, 0, (LPARAM)GetSafeHwnd());
			::AfxMessageBox(L"서버 바인드 에러 !!");
			return;
		}

		if (!m_udpSendClient.Init(ip, m_SndPort))
		{
			GetParent()->PostMessageW(WM_UDP2UDP_START_FAIL, 0, (LPARAM)GetSafeHwnd());
			::AfxMessageBox(L"클라이언트 접속 에러 !!");
			m_udpReceiveServer.Close();
			return;
		}

		m_isStart = true;
		m_StartButton.SetWindowTextW(L"Stop");
		m_SndIP.EnableWindow(FALSE);
		m_EditReceivePort.EnableWindow(FALSE);
		m_EditSendPort.EnableWindow(FALSE);

		GetParent()->PostMessageW(WM_UDP2UDP_START_SUCCESS, 0, (LPARAM)GetSafeHwnd());
	}
}


// 파라메터 초기화.
void CProxyWindow::Init(const int rcvPort, const string &sendIP, const int sendPort)
// showRemoveButton = true
{
	m_RcvPort = rcvPort;
	m_SndPort = sendPort;

	int ip1, ip2, ip3, ip4;
	const int result = sscanf_s(sendIP.c_str(), "%d.%d.%d.%d", &ip1, &ip2, &ip3, &ip4);
	if (result >= 4)
	{
		m_SndIP.SetAddress(ip1, ip2, ip3, ip4);
	}
	else
	{
		m_SndIP.SetAddress(127, 0, 0, 1);
	}

	UpdateData(FALSE);
}


// 송신 클라이언트가 접속할 IP 주소를 리턴한다.
string CProxyWindow::GetSendIP()
{
	DWORD address;
	m_SndIP.GetAddress(address);
	std::stringstream ss;
	ss << ((address & 0xff000000) >> 24) << "."
		<< ((address & 0x00ff0000) >> 16) << "."
		<< ((address & 0x0000ff00) >> 8) << "."
		<< (address & 0x000000ff);
	const string ip = ss.str();
	return ip;
}


void CProxyWindow::HideRemoveButton()
{
	m_RemoveButton.ShowWindow(SW_HIDE);
}


void CProxyWindow::OnBnClickedButtonAdd()
{
	GetParent()->PostMessageW(WM_COMMAND, MAKELONG(IDC_BUTTON_ADD, 0), (WPARAM)GetSafeHwnd());
}


void CProxyWindow::OnBnClickedButtonRemove()
{
	GetParent()->PostMessageW(WM_COMMAND, MAKELONG(IDC_BUTTON_REMOVE, 0), (WPARAM)GetSafeHwnd());
}

