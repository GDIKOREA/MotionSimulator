// UDPView.cpp : implementation file
//

#include "stdafx.h"
#include "UDPAnalyzer.h"
#include "UDPView.h"
#include "afxdialogex.h"
#include "script.h"


// CUDPView dialog
CUDPView::CUDPView(CWnd* pParent /*=NULL*/)
	: CDockablePaneChildView(CUDPView::IDD, pParent)
	, m_Port(0)
	, m_IsDump(TRUE)
	, m_IsASCII(FALSE)
	, m_dumpWindow(NULL)
	, m_IsDisplaySymbol(FALSE)
	, m_recvCount(0)
{
}

CUDPView::~CUDPView()
{
}

void CUDPView::DoDataExchange(CDataExchange* pDX)
{
	CDockablePaneChildView::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_PORT, m_Port);
	DDX_Control(pDX, IDC_BUTTON_START, m_StartButton);
	DDX_Control(pDX, IDC_EDIT_PROTOCOL, m_ProtocolEditor);
	DDX_Check(pDX, IDC_CHECK_DUMP, m_IsDump);
	DDX_Check(pDX, IDC_CHECK_ASCII, m_IsASCII);
	DDX_Check(pDX, IDC_CHECK_SYMBOL, m_IsDisplaySymbol);
	DDX_Control(pDX, IDC_STATIC_RCVCOUNT, m_ReceiveCount);
}


BEGIN_ANCHOR_MAP(CUDPView)
	ANCHOR_MAP_ENTRY(IDC_EDIT_PROTOCOL, ANF_LEFT | ANF_RIGHT | ANF_TOP )
	ANCHOR_MAP_ENTRY(IDC_STATIC_DUMP, ANF_LEFT | ANF_RIGHT | ANF_TOP | ANF_BOTTOM)
END_ANCHOR_MAP()


BEGIN_MESSAGE_MAP(CUDPView, CDockablePaneChildView)
	ON_BN_CLICKED(IDOK, &CUDPView::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CUDPView::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_START, &CUDPView::OnBnClickedButtonStart)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_UPDATE, &CUDPView::OnBnClickedButtonUpdate)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_CHECK_DUMP, &CUDPView::OnBnClickedCheckDump)
	ON_BN_CLICKED(IDC_CHECK_ASCII, &CUDPView::OnBnClickedCheckAscii)
	ON_BN_CLICKED(IDC_CHECK_SYMBOL, &CUDPView::OnBnClickedCheckSymbol)
END_MESSAGE_MAP()


// CUDPView message handlers
BOOL CUDPView::OnInitDialog()
{
	CDockablePaneChildView::OnInitDialog();

	InitAnchors();

	UpdateConfig();

	CRect rect;
	GetClientRect(rect);

	m_dumpWindow = new cMemDumpWindow();
	BOOL result = m_dumpWindow->Create(NULL, NULL, WS_VISIBLE | WS_CHILD,
		CRect(0, 0, 20, 40), this, AFX_IDW_PANE_FIRST);
	m_dumpWindow->SetScrollSizes(MM_TEXT, CSize(rect.Width() - 30, 100));
	m_dumpWindow->ShowWindow(SW_SHOW);

	return TRUE;
}


void CUDPView::OnBnClickedOk()
{
}


void CUDPView::OnBnClickedCancel()
{
}


void CUDPView::OnBnClickedButtonStart()
{
	UpdateData();

	if (cController::Get()->GetUDPComm().IsConnect())
	{
		cController::Get()->GetUDPComm().Close();
		m_StartButton.SetWindowTextW(L"Server Start");

		SetBackgroundColor(g_grayColor);
	}
	else
	{
		if (cController::Get()->GetUDPComm().InitServer(m_Port))
		{
			m_StartButton.SetWindowTextW(L"Server Stop");

			CString text;
			m_ProtocolEditor.GetWindowTextW(text);
			m_protocolParser.ParseStr(wstr2str((LPCTSTR)text));

			SetBackgroundColor(g_blueColor);
 		}
		else
		{
			//m_LogList.InsertString(m_LogList.GetCount(), L"접속 실패");
		}
	}
}


// UDP Network Packet Update
void CUDPView::UpdateUDP(const char *buffer, const int bufferLen)
{
	RET(m_protocolParser.m_fields.empty());

	int i = 0;
	int index = 0;
	const char *pmem = buffer;
	for each (auto &field in m_protocolParser.m_fields)
	{
		if (index > bufferLen)
			break;

		script::sFieldData data;
		ZeroMemory(data.buff, sizeof(data));
		memcpy(data.buff, pmem, field.bytes);
		data.type = field.type;

		const string id = format("$%d", i + 1); // $1 ,$2, $3 ~
		const string oldId = format("@%d", i + 1); // @1, @2, @3 ~

		script::g_symbols[oldId] = script::g_symbols[id]; // 전 데이타를 저장한다. 
		script::g_symbols[id] = data;
		
		index += field.bytes;
		pmem += field.bytes;
		++i;
	}

	if (m_IsDump && m_dumpWindow)
		m_dumpWindow->UpdateDump(buffer, bufferLen);

	++m_recvCount;
	m_ReceiveCount.SetWindowTextW(common::formatw("%d", m_recvCount).c_str());

}


void CUDPView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePaneChildView::OnSize(nType, cx, cy);

	CRect rcWnd;
	GetWindowRect(&rcWnd);
	HandleAnchors(&rcWnd);

	if (m_dumpWindow && m_dumpWindow->GetSafeHwnd())
	{
		CRect pwr;
		GetDlgItem(IDC_STATIC_DUMP)->GetWindowRect(pwr);
		ScreenToClient(pwr);
		m_dumpWindow->MoveWindow(pwr);
	}
}


void CUDPView::OnBnClickedButtonUpdate()
{
	UpdateData();

	CString text;
	m_ProtocolEditor.GetWindowTextW(text);
	m_protocolParser.ParseStr(wstr2str((LPCTSTR)text));

	script::ClearSymbols();
}


void CUDPView::OnDestroy()
{
	SaveConfig();
	CDockablePaneChildView::OnDestroy();	
}

// UI에 설정된 값을 환경변수에 저장한다.
void CUDPView::SaveConfig()
{
	UpdateData();

	// 환경파일 저장
	CString command;
	m_ProtocolEditor.GetWindowTextW(command);
	g_option.m_udpProtocolCmd = wstr2str((LPCTSTR)command);
	g_option.m_udpPort = m_Port;
}


void CUDPView::OnBnClickedCheckDump()
{
	UpdateData();
}


void CUDPView::OnBnClickedCheckAscii()
{
	UpdateData();
	m_dumpWindow->SetDisplayASCII(m_IsASCII? true : false);
	m_dumpWindow->InvalidateRect(NULL);
}


void CUDPView::UpdateConfig()
{
	const CString command =
		L"2, uint #test\r\n"
		L"4, float\r\n"
		L"4, float\r\n"
		L"4, float\r\n"
		L"4, float\r\n";

	CString cmdStr;
	if (!g_option.m_udpProtocolCmd.empty())
	{
		cmdStr = str2wstr(g_option.m_udpProtocolCmd).c_str();
	}
	else
	{
		cmdStr = command;
	}
	m_ProtocolEditor.SetWindowTextW(cmdStr);

	m_Port = g_option.m_udpPort;

	m_protocolParser.ParseStr(wstr2str((LPCTSTR)cmdStr));


	// 심볼 업데이트
	script::ClearSymbols();
	int i = 0;
	for each (auto &field in m_protocolParser.m_fields)
	{
		script::sFieldData data;
		ZeroMemory(data.buff, sizeof(data));
		data.type = field.type;

		const string id = format("$%d", i + 1); // $1 ,$2, $3 ~
		const string oldId = format("@%d", i + 1); // @1, @2, @3 ~

		script::g_symbols[oldId] = data;
		script::g_symbols[id] = data;

		++i;
	}	
	
	UpdateData(FALSE);
}


void CUDPView::OnBnClickedCheckSymbol()
{
	UpdateData();

	m_dumpWindow->SetDisplaySymbol(m_IsDisplaySymbol? true : false, &m_protocolParser);
	m_dumpWindow->InvalidateRect(NULL);
}
