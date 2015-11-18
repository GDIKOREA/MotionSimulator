
#include "stdafx.h"
#include "Serial2UDP.h"
#include "Serial2UDPDlg.h"
#include "afxdialogex.h"
#include <mmsystem.h>
#include <sstream>
#include "NoticeDlg.h"

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
	, m_isComConnect(false)
	, m_serialRxCnt(0)
	, m_udpRxCnt(0)
	, m_errCnt(0)
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
	//m_ComPortComboBox.InitList();
	m_ServerIP.SetAddress(127, 0, 0, 1);

	m_client.m_sleepMillis = 1;
	
	const int baudRate[] = { 9600, 14400, 19200, 38400, 56000, 57600, 115200, 912600 };
	for (int i = 0; i < ARRAYSIZE(baudRate); ++i)
	{
		TCHAR brateName[32];
		wsprintf(brateName, L"%d", baudRate[i]);
		m_ComBaudrateComboBox.InsertString(m_ComBaudrateComboBox.GetCount(), brateName);
	}
	m_ComBaudrateComboBox.SetCurSel(0);

	m_serial.SetMaxWaitTime(20);

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

		static bool onlyOne = true;
		if (onlyOne)
		{
			onlyOne = false;

			CNoticeDlg *dlg = new CNoticeDlg();
			dlg->Create(CNoticeDlg::IDD, this);
			dlg->CenterWindow();
			dlg->ShowWindow(SW_SHOW);
			m_ComPortComboBox.InitList();
			dlg->ShowWindow(SW_HIDE);
			delete dlg;
		}
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
	//�޽��� ����ü
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


// ����
// �ø��� ��Ʈ, UDP ��� ����.
void CSerial2UDPDlg::OnBnClickedButtonStart()
{
	UpdateData();

	if (m_client.IsConnect())
	{
		m_errCnt = 0;

		// ������ ������ �����Ѵ�.
		m_client.Close();
		m_server.Close();
		m_serial.Close();
		m_StartButton.SetWindowTextW(L"Start");

		SetBackgroundColor(g_grayColor);

		Log(format("���� ����"));
	}
	else
	{
		// ������ ������ �õ��Ѵ�.
		DWORD address;
		m_ServerIP.GetAddress(address);

		std::stringstream ss;
		ss << ((address & 0xff000000) >> 24) << "."
			<< ((address & 0x00ff0000) >> 16) << "."
			<< ((address & 0x0000ff00) >> 8) << "."
			<< (address & 0x000000ff);

		const string ip = ss.str();
		bool startOk = true;

		if (m_client.Init(ip, m_ServerPort, 10))
		{
			Log(common::format("���� ���� ����, ip = %s, port = %d", ip.c_str(), m_ServerPort));

			const int portNumber = m_ComPortComboBox.GetPortNum();
			CString baudRate;
			m_ComBaudrateComboBox.GetWindowTextW(baudRate);

			// Open Serial Port
			if (m_serial.Open(portNumber, _wtoi(baudRate)))
			{
				Log(common::format("COM%d ���� ����, baudRate = %d", portNumber, _wtoi(baudRate)));

				if (m_server.Init(0, m_RcvPort))
				{
					Log(common::format("���ε� ����, port = %d", m_RcvPort));
					m_StartButton.SetWindowTextW(L"Stop");
				}
				else
				{
					startOk = false;
					Log(common::format("���ε� ����!!, port = %d", m_RcvPort));
				}
			}
			else
			{
				startOk = false;
				Log(common::format("COM%d ���� ����", portNumber));
			}
		}
		else
		{
			startOk = false;
			Log(common::format("���� ���� ����!!, ip = %s, port = %d", ip.c_str(), m_ServerPort));
		}

		if (startOk)
		{
			SetBackgroundColor(g_blueColor);
		}
		else
		{
			m_client.Close();
			m_server.Close();
			m_serial.Close();
		}

	}
}


// �α� ���.
void CSerial2UDPDlg::Log(const string &str)
{
	m_LogList.InsertString(m_LogList.GetCount(), common::str2wstr(str).c_str());
	m_LogList.SetTopIndex(m_LogList.GetCount()-1);
}


// �α� �ʱ�ȭ.
void CSerial2UDPDlg::OnBnClickedButtonClear()
{
	for (int i = m_LogList.GetCount() - 1; i >= 0; --i)
		m_LogList.DeleteString(i);
}


// �ø���� ���� ������ �޴´�.
// ���� ������ UDP�� �����Ѵ�.
void CSerial2UDPDlg::Process(const int deltaMilliseconds)
{
	if (!m_client.IsConnect())
	{
		// ��Ʈ�� ����Ǿ� ���� �ʴٸ�, CPU �δ��� ���̱� ���� ���� ������ �Ѿ��.
		Sleep(10);
		return;
	}

	char readStr[512];
	int readLen=0;
	if (!m_serial.ReadStringUntil('\n', readStr, readLen, sizeof(readStr)))
	{
		// ���� �߻�. �ø��� ��Ʈ ����� ���´�.
		//OnBnClickedButtonStart();
		//return;
	}

	if (readLen > 0)
	{
		if (sizeof(readStr) > readLen + 1)
			readStr[readLen] = NULL;

		CString str = str2wstr(readStr).c_str();
		m_SerialReceiveText.SetWindowTextW(str);
		++m_serialRxCnt;
		CString rxCnt;
		rxCnt.Format(L"%d", m_serialRxCnt);
		m_SerialReceiveCount.SetWindowTextW(rxCnt);

		// �ø���� ���� ������ UDP ��Ʈ��ũ�� ���� �����Ѵ�.
		char buffer[256];
		if (readLen < sizeof(buffer))
		{
			memcpy(buffer, &readStr[0], readLen);
			if (readLen + 1 < sizeof(buffer))
				buffer[readLen] = NULL;
		}
		else
		{
			memcpy(buffer, &readStr[0], sizeof(buffer)-1);
			buffer[sizeof(buffer) - 1] = NULL;
		}

		m_client.SendData(buffer, sizeof(buffer));
	}

	char rcvBuffer[512];
	const int len = m_server.GetRecvData(rcvBuffer, sizeof(rcvBuffer));
	if (len > 0)
	{
		// UDP�� ���� ������ ���ŵǸ�, �ø���������� ������.
		const int t0 = timeGetTime();
		m_serial.SendData(rcvBuffer, len);

		// ������ �����µ� �ʹ� ���� �ɸ���, ������ ���´�.
		const int t1 = timeGetTime();
		if (t1 - t0 > 1000)
		{
			++m_errCnt;
			if (m_errCnt > 1)
				OnBnClickedButtonStart(); // Toggle Swich
		}

		++m_udpRxCnt;
		CString rxCnt;
		rxCnt.Format(L"%d", m_udpRxCnt);
		m_UdpRxCount.SetWindowTextW(rxCnt);
	}
}

