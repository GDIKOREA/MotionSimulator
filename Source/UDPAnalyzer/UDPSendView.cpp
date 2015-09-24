// UDPSend.cpp : implementation file
//

#include "stdafx.h"
#include "UDPAnalyzer.h"
#include "UDPSendView.h"
#include "afxdialogex.h"


// CUDPSend dialog

CUDPSendView::CUDPSendView(CWnd* pParent /*=NULL*/)
	: CDockablePaneChildView(CUDPSendView::IDD, pParent)
	, m_Port(8888)
	, m_IsSyncUDPView(FALSE)
	, m_IsConnect(false)
	, m_IsConvertCR(TRUE)
{
}

CUDPSendView::~CUDPSendView()
{
}

void CUDPSendView::DoDataExchange(CDataExchange* pDX)
{
	CDockablePaneChildView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPADDRESS_IP, m_IP);
	DDX_Text(pDX, IDC_EDIT_PORT, m_Port);
	DDX_Check(pDX, IDC_CHECK_SYNC_UDPVIEW, m_IsSyncUDPView);
	DDX_Control(pDX, IDC_BUTTON_CONNECT, m_ConnectButton);
	DDX_Check(pDX, IDC_CHECK_CR, m_IsConvertCR);
}


BEGIN_ANCHOR_MAP(CUDPSendView)
	ANCHOR_MAP_ENTRY(IDC_BUTTON1, ANF_RIGHT | ANF_TOP)
	ANCHOR_MAP_ENTRY(IDC_BUTTON2, ANF_RIGHT | ANF_TOP)
	ANCHOR_MAP_ENTRY(IDC_BUTTON3, ANF_RIGHT | ANF_TOP)
	ANCHOR_MAP_ENTRY(IDC_BUTTON4, ANF_RIGHT | ANF_TOP)
	ANCHOR_MAP_ENTRY(IDC_BUTTON5, ANF_RIGHT | ANF_TOP)
	ANCHOR_MAP_ENTRY(IDC_BUTTON6, ANF_RIGHT | ANF_TOP)
	ANCHOR_MAP_ENTRY(IDC_BUTTON7, ANF_RIGHT | ANF_TOP)
	ANCHOR_MAP_ENTRY(IDC_BUTTON8, ANF_RIGHT | ANF_TOP)
	ANCHOR_MAP_ENTRY(IDC_BUTTON9, ANF_RIGHT | ANF_TOP)
	ANCHOR_MAP_ENTRY(IDC_BUTTON10, ANF_RIGHT | ANF_TOP)
	ANCHOR_MAP_ENTRY(IDC_EDIT1, ANF_LEFT | ANF_RIGHT | ANF_TOP)
	ANCHOR_MAP_ENTRY(IDC_EDIT2, ANF_LEFT | ANF_RIGHT | ANF_TOP)
	ANCHOR_MAP_ENTRY(IDC_EDIT3, ANF_LEFT | ANF_RIGHT | ANF_TOP)
	ANCHOR_MAP_ENTRY(IDC_EDIT4, ANF_LEFT | ANF_RIGHT | ANF_TOP)
	ANCHOR_MAP_ENTRY(IDC_EDIT5, ANF_LEFT | ANF_RIGHT | ANF_TOP)
	ANCHOR_MAP_ENTRY(IDC_EDIT6, ANF_LEFT | ANF_RIGHT | ANF_TOP)
	ANCHOR_MAP_ENTRY(IDC_EDIT7, ANF_LEFT | ANF_RIGHT | ANF_TOP)
	ANCHOR_MAP_ENTRY(IDC_EDIT8, ANF_LEFT | ANF_RIGHT | ANF_TOP)
	ANCHOR_MAP_ENTRY(IDC_EDIT9, ANF_LEFT | ANF_RIGHT | ANF_TOP)
	ANCHOR_MAP_ENTRY(IDC_EDIT10, ANF_LEFT | ANF_RIGHT | ANF_TOP)
END_ANCHOR_MAP()


BEGIN_MESSAGE_MAP(CUDPSendView, CDockablePaneChildView)
	ON_BN_CLICKED(IDOK, &CUDPSendView::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CUDPSendView::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_CONNECT, &CUDPSendView::OnBnClickedButtonConnect)
	ON_BN_CLICKED(IDC_CHECK_SYNC_UDPVIEW, &CUDPSendView::OnBnClickedCheckSyncUdpview)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BUTTON1, IDC_BUTTON10, &CUDPSendView::OnBnClickedSendButton)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_CHECK_CR, &CUDPSendView::OnBnClickedCheckCr)
END_MESSAGE_MAP()


// CUDPSend message handlers
void CUDPSendView::OnBnClickedOk()
{
}
void CUDPSendView::OnBnClickedCancel()
{
}

BOOL CUDPSendView::OnInitDialog()
{
	CDockablePaneChildView::OnInitDialog();

	InitAnchors();

	m_IP.SetAddress(127, 0, 0, 1);

	for (int i = IDC_BUTTON1; i <= IDC_BUTTON10; ++i)
		GetDlgItem(i)->EnableWindow(FALSE);

	return TRUE;
}


void CUDPSendView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePaneChildView::OnSize(nType, cx, cy);

	CRect rcWnd;
	GetWindowRect(&rcWnd);
	HandleAnchors(&rcWnd);
}


void CUDPSendView::OnBnClickedButtonConnect()
{
	UpdateData();

	if (m_IsConnect)
	{
		m_IsConnect = false;
		if (!m_IsSyncUDPView)
		{
			m_udpClient.Close();
		}

		m_ConnectButton.SetWindowTextW(L"Connect");
		SetBackgroundColor(g_grayColor);
	}
	else
	{

		m_IsSyncUDPView = FALSE;
		UpdateData(FALSE);

		const string ip = GetIP(m_IP);
		if (m_udpClient.Init(ip, m_Port))
		{
			m_IsConnect = true;

			m_ConnectButton.SetWindowTextW(L"Stop");
			SetBackgroundColor(g_blueColor);
		}
		else
		{
			AfxMessageBox(L"접속 실패");
		}

	}

	for (int i = IDC_BUTTON1; i <= IDC_BUTTON10; ++i)
		GetDlgItem(i)->EnableWindow(m_IsConnect? TRUE:FALSE);

}


void CUDPSendView::OnBnClickedCheckSyncUdpview()
{
	UpdateData();

	if (m_IsSyncUDPView)
	{
		if (cController::Get()->GetUDPComm().IsConnect())
		{
			m_IsConnect = true;
			SetBackgroundColor(g_blueColor);
		}
		else
		{
			AfxMessageBox(L"UDP View를 먼저 연결해야 합니다.");
		}
	}
	else
	{
		m_IsConnect = false;
		SetBackgroundColor(g_grayColor);
	}

	for (int i = IDC_BUTTON1; i <= IDC_BUTTON10; ++i)
		GetDlgItem(i)->EnableWindow(m_IsConnect ? TRUE : FALSE);
}


void CUDPSendView::OnBnClickedCheckCr()
{
	UpdateData();
}


void CUDPSendView::OnBnClickedSendButton(UINT id)
{
	RET(!m_IsConnect);

	const int index = id - IDC_BUTTON1;
	const int editBoxIndex = IDC_EDIT1 + index;
	CString text;
	GetDlgItem(editBoxIndex)->GetWindowTextW(text);

	SendString(index, wstr2str((LPCTSTR)text));
}


// editIndex 번째 에디트박스의 문자열을 udp 통신으로 전송한다.
void CUDPSendView::SendString(const int editIndex, const string &str)
{
	RET((editIndex >= 10) || (editIndex < 0));
	RET(str.length() <= 0);

	m_edit[editIndex].ParseStr(str);
	string packet = m_edit[editIndex].Execute();

	if (m_IsConvertCR)
		replaceAll(packet, "\\n", "\n");

	// 이미 연결된 UDP로 전송한다.
	if (m_IsSyncUDPView)
	{
		cController::Get()->GetUDPComm().SendData(packet.c_str(), packet.length());
	}
	else
	{
		// 직접 연결한 UDP로 전송한다.
		m_udpClient.SendData(packet.c_str(), packet.length());
	}

}
