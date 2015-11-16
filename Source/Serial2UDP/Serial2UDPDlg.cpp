
#include "stdafx.h"
#include "Serial2UDP.h"
#include "Serial2UDPDlg.h"
#include "afxdialogex.h"
#include <mmsystem.h>
#include <sstream>

#pragma comment(lib, "winmm.lib")

using namespace common;


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CSerial2UDPDlg::CSerial2UDPDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSerial2UDPDlg::IDD, pParent)
	, m_loop(true)
	, m_ServerPort(8888)
	, m_RcvPort(8889)
//	, m_isServerConnect(false)
	, m_isComConnect(false)
	, m_serialRxCnt(0)
	, m_udpRxCnt(0)
	//, m_SerialReceiveText(_T(""))
	//, m_SerialReceiveCount(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSerial2UDPDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_LOG, m_LogList);
	DDX_Text(pDX, IDC_EDIT_UDP_PORT, m_ServerPort);
	DDX_Control(pDX, IDC_IPADDRESS_UDP_IP, m_ServerIP);
	DDX_Control(pDX, IDC_COMBO_SERIAL_PORT, m_ComPortComboBox);
	DDX_Control(pDX, IDC_COMBO_SERIAL_BAUDRATE, m_ComBaudrateComboBox);
	DDX_Control(pDX, IDC_BUTTON_START, m_StartButton);
	DDX_Control(pDX, IDC_STATIC_SERIAL, m_SerialReceiveText);
	DDX_Control(pDX, IDC_STATIC_SERIAL_COUNT, m_SerialReceiveCount);
	DDX_Text(pDX, IDC_EDIT_UDP_RCV_PORT, m_RcvPort);
	DDX_Control(pDX, IDC_STATIC_UDP_COUNT, m_UdpRxCount);
}

BEGIN_MESSAGE_MAP(CSerial2UDPDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CSerial2UDPDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CSerial2UDPDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_START, &CSerial2UDPDlg::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CSerial2UDPDlg::OnBnClickedButtonClear)
END_MESSAGE_MAP()


BOOL CSerial2UDPDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	//m_ComPortComboBox.SetOnlyPresent(FALSE);
	//m_ComPortComboBox.SetOnlyPhysical(FALSE);
	m_ComPortComboBox.InitList();
	m_ServerIP.SetAddress(127, 0, 0, 1);

	m_client.m_sleepMillis = 0;
	
	const int baudRate[] = { 9600, 14400, 19200, 38400, 56000, 57600, 115200, 912600 };
	for (int i = 0; i < ARRAYSIZE(baudRate); ++i)
	{
		TCHAR brateName[32];
		wsprintf(brateName, L"%d", baudRate[i]);
		m_ComBaudrateComboBox.InsertString(m_ComBaudrateComboBox.GetCount(), brateName);
	}
	m_ComBaudrateComboBox.SetCurSel(0);

	return TRUE;
}


void CSerial2UDPDlg::OnPaint()
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

HCURSOR CSerial2UDPDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CSerial2UDPDlg::OnBnClickedOk()
{
//	CDialogEx::OnOK();
}


void CSerial2UDPDlg::OnBnClickedCancel()
{
	CDialogEx::OnCancel();
	m_loop = false;
}


void CSerial2UDPDlg::MainLoop()
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

		const int curT = timeGetTime();
		const int deltaT = curT - oldT;
		//if (deltaT > 10)
		{
			oldT = curT;
			Process(deltaT);
		}
	}
}


// 시작
// 시리얼 포트, UDP 통신 시작.
void CSerial2UDPDlg::OnBnClickedButtonStart()
{
	UpdateData();

	//if (m_isServerConnect)
	if (m_client.IsConnect())
	{
		// 서버와 접속을 종료한다.
		m_client.Close();
		m_server.Close();
// 		closesocket(m_socket);
// 		m_isServerConnect = false;
		m_StartButton.SetWindowTextW(L"Start");

		Log(format("접속 종료"));
	}
	else
	{
		// 서버에 접속을 시도한다.
		DWORD address;
		m_ServerIP.GetAddress(address);

		std::stringstream ss;
		ss << ((address & 0xff000000) >> 24) << "."
			<< ((address & 0x00ff0000) >> 16) << "."
			<< ((address & 0x0000ff00) >> 8) << "."
			<< (address & 0x000000ff);

		const string ip = ss.str();

		//if (network::LaunchUDPClient(ip, m_ServerPort, m_sockAddr, m_socket))
		if (m_client.Init(ip, m_ServerPort))
		{
			Log(common::format("서버 접속 성공, ip = %s, port = %d", ip.c_str(), m_ServerPort));

			const int portNumber = m_ComPortComboBox.GetPortNum();
			CString baudRate;
			m_ComBaudrateComboBox.GetWindowTextW(baudRate);

			// Open Serial Port
			if (m_serial.Open(portNumber, _wtoi(baudRate)))
			{
				Log(common::format("COM%d 접속 성공, baudRate = %d", portNumber, _wtoi(baudRate)));

				//				m_isServerConnect = true;
				m_StartButton.SetWindowTextW(L"Stop");
			}
			else
			{
				Log(common::format("COM%d 접속 실패", portNumber));
			}
		}
		else
		{
			Log(common::format("서버 접속 실패!!, ip = %s, port = %d", ip.c_str(), m_ServerPort));
		}

		if (m_server.Init(0, m_RcvPort))
		{
			Log(common::format("바인드 성공, port = %d", m_RcvPort));
		}
		else
		{
			Log(common::format("바인드 실패!!, port = %d", m_RcvPort));
		}
	}
}


// 로그 출력.
void CSerial2UDPDlg::Log(const string &str)
{
	m_LogList.InsertString(m_LogList.GetCount(), common::str2wstr(str).c_str());
	m_LogList.SetTopIndex(m_LogList.GetCount()-1);
}


// 로그 초기화.
void CSerial2UDPDlg::OnBnClickedButtonClear()
{
	for (int i = m_LogList.GetCount() - 1; i >= 0; --i)
		m_LogList.DeleteString(i);
}


// 시리얼로 부터 정보를 받는다.
// 받은 정보를 UDP로 전송한다.
void CSerial2UDPDlg::Process(const int deltaMilliseconds)
{
	//if (!m_isServerConnect)
	if (!m_client.IsConnect())
	{
		// 포트와 연결되어 있지 않다면, CPU 부담을 줄이기 위해 조금 쉬었다 넘어간다.
		Sleep(10);
		return;
	}

	string readStr;
	if (!m_serial.ReadStringUntil('\n', readStr))
	{
		// 에러 발생. 시리얼 포트 연결과 끊는다.
		//OnBnClickedButtonStart();
		//return;
	}

	if (!readStr.empty())
	{
		CString str = str2wstr(readStr).c_str();
		m_SerialReceiveText.SetWindowTextW(str);
		++m_serialRxCnt;
		CString rxCnt;
		rxCnt.Format(L"%d", m_serialRxCnt);
		m_SerialReceiveCount.SetWindowTextW(rxCnt);

		// 시리얼로 받은 정보를 UDP 네트워크를 통해 전송한다.
		//int slen = sizeof(m_sockAddr);
		char buffer[512];
		if (readStr.size() < sizeof(buffer))
		{
			const int len = readStr.size();
			memcpy(buffer, &readStr[0], len);
			buffer[len - 1] = NULL;
		}
		else
		{
			memcpy(buffer, &readStr[0], sizeof(buffer)-1);
			buffer[sizeof(buffer) - 1] = NULL;
		}

		m_client.SendData(buffer, sizeof(buffer));

// 	if (sendto(m_socket, buffer, sizeof(buffer), 0, (struct sockaddr *) &m_sockAddr, slen) == SOCKET_ERROR)
//  	{
//   		//Log( format("sendto() failed with error code : %d", WSAGetLastError()) );
//  		//OnBnClickedButtonStart();
//  	}
	}

	char rcvBuffer[512];
	const int len = m_server.GetRecvData(rcvBuffer, sizeof(rcvBuffer));
	if (len > 0)
	{
		// UDP로 부터 정보가 수신되면, 시리얼통신으로 보낸다.
		m_serial.SendData(rcvBuffer, len);

		++m_udpRxCnt;
		CString rxCnt;
		rxCnt.Format(L"%d", m_udpRxCnt);
		m_UdpRxCount.SetWindowTextW(rxCnt);
	}

	//UpdateData(FALSE);
}
