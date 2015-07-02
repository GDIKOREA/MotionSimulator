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
		AppendToLogAndScroll(str, RGB(200,200,200));
	}

	++m_PacketRcvCount;
	UpdateData(FALSE);
}


// http://www.codeproject.com/Articles/12093/Using-RichEditCtrl-to-Display-Formatted-Logs
int CUDPEditorView::AppendToLogAndScroll(CString str, COLORREF color)
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
	nInsertionPoint = m_LogEditor.GetWindowTextLength();
	m_LogEditor.SetSel(nInsertionPoint, -1);

	// Set the character format
	m_LogEditor.SetSelectionCharFormat(cf);

	// Replace selection. Because we have nothing
	// selected, this will simply insert
	// the string at the current caret position.
	m_LogEditor.ReplaceSel(str);

	// Get number of currently visible lines or maximum number of visible lines
	// (We must call GetNumVisibleLines() before the first call to LineScroll()!)
	nVisible = GetNumVisibleLines(&m_LogEditor);

	// Now this is the fix of CRichEditCtrl's abnormal behaviour when used
	// in an application not based on dialogs. Checking the focus prevents
	// us from scrolling when the CRichEditCtrl does so automatically,
	// even though ES_AUTOxSCROLL style is NOT set.
	if (&m_LogEditor != m_LogEditor.GetFocus())
	{
		m_LogEditor.LineScroll(INT_MAX);
		m_LogEditor.LineScroll(1 - nVisible);
	}

	// 내용이 너무 많으면 지운다.
	const int maximumLine = 100;
	if (m_LogEditor.GetLineCount() > maximumLine)
	{
		long nFirstChar = m_LogEditor.CharFromPos(CPoint(0, 0));
		m_LogEditor.SetSel(0, nFirstChar);
		m_LogEditor.ReplaceSel(L"");
	}

	return 0;
}


//http://www.codeproject.com/Articles/12093/Using-RichEditCtrl-to-Display-Formatted-Logs
int CUDPEditorView::GetNumVisibleLines(CRichEditCtrl* pCtrl)
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
