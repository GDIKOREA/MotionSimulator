// UDPEditorView.cpp : implementation file
//

#include "stdafx.h"
#include "MotionMonitor.h"
#include "UDPEditorView.h"
#include "afxdialogex.h"


// CUDPEditorView dialog
CUDPEditorView::CUDPEditorView(CWnd* pParent /*=NULL*/)
	: CDockablePaneChildView(CUDPEditorView::IDD, pParent)
	, m_ServerPort(20777)
	, m_PacketRcvCount(0)
	, m_IsPrintPacket(TRUE)
{
}

CUDPEditorView::~CUDPEditorView()
{
}

void CUDPEditorView::DoDataExchange(CDataExchange* pDX)
{
	CDockablePaneChildView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_UDP_SERVER_BIND, m_ServerStartButton);
	DDX_Text(pDX, IDC_EDIT_SERVER_PORT, m_ServerPort);
	DDX_Control(pDX, IDC_RICHEDIT2_UDP_LOG, m_LogEditor);
	DDX_Text(pDX, IDC_STATIC_PACKET_COUNT, m_PacketRcvCount);
	DDX_Check(pDX, IDC_CHECK_PRINT_PACKET, m_IsPrintPacket);
}



BEGIN_ANCHOR_MAP(CUDPEditorView)
	ANCHOR_MAP_ENTRY(IDC_RICHEDIT2_UDP_LOG, ANF_TOPLEFT | ANF_BOTTOMRIGHT)
	ANCHOR_MAP_ENTRY(IDC_BUTTON_CLEAR, ANF_TOP | ANF_RIGHT)
// 	ANCHOR_MAP_ENTRY(IDC_BUTTON_UPDATE, ANF_RIGHT | ANF_BOTTOM)
// 	ANCHOR_MAP_ENTRY(IDC_BUTTON_HELP, ANF_RIGHT | ANF_BOTTOM)
// 	ANCHOR_MAP_ENTRY(IDC_BUTTON_PAUSE, ANF_RIGHT | ANF_BOTTOM)
// 	ANCHOR_MAP_ENTRY(IDC_CHECK_FIXEDMODE, ANF_LEFT | ANF_BOTTOM)
// 	ANCHOR_MAP_ENTRY(IDC_STATIC1, ANF_LEFT | ANF_BOTTOM)
END_ANCHOR_MAP()


BEGIN_MESSAGE_MAP(CUDPEditorView, CDockablePaneChildView)
	ON_BN_CLICKED(IDOK, &CUDPEditorView::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CUDPEditorView::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_UDP_SERVER_BIND, &CUDPEditorView::OnBnClickedButtonUdpServerBind)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CUDPEditorView::OnBnClickedButtonClear)
	ON_BN_CLICKED(IDC_CHECK_PRINT_PACKET, &CUDPEditorView::OnBnClickedCheckPrintPacket)
END_MESSAGE_MAP()


// CUDPEditorView message handlers
BOOL CUDPEditorView::OnInitDialog()
{
	CDockablePaneChildView::OnInitDialog();

	InitAnchors();

	m_LogEditor.SetBackgroundColor(FALSE, RGB(0, 0, 0));
	
	return TRUE;
}



void CUDPEditorView::OnBnClickedOk()
{
	//CDockablePaneChildView::OnOK();
}


void CUDPEditorView::OnBnClickedCancel()
{
	//CDockablePaneChildView::OnCancel();
}


void CUDPEditorView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePaneChildView::OnSize(nType, cx, cy);

	CRect rcWnd;
	GetWindowRect(&rcWnd);
	HandleAnchors(&rcWnd);
}


void CUDPEditorView::Update(const float deltaSeconds)
{

}


void CUDPEditorView::OnBnClickedButtonUdpServerBind()
{
	UpdateData();
	cController::Get()->GetUDPComm().InitServer(m_ServerPort);
}


void CUDPEditorView::OnBnClickedButtonClear()
{
	m_LogEditor.SetSel(0, m_LogEditor.GetTextLength());
	m_LogEditor.ReplaceSel(L"");
}


void CUDPEditorView::OnBnClickedCheckPrintPacket()
{
	UpdateData();
}


void CUDPEditorView::UpdateUDP(const char *buffer, const int bufferLen)
{
	if (m_IsPrintPacket)
	{
		CString str = common::str2wstr(buffer).c_str();
		AppendToLogAndScroll(&m_LogEditor, str, RGB(200,200,200));
	}

	++m_PacketRcvCount;
	UpdateData(FALSE);
}


