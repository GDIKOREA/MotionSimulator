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
}


BEGIN_ANCHOR_MAP(CUDPView)
	ANCHOR_MAP_ENTRY(IDC_EDIT_PROTOCOL, ANF_LEFT | ANF_RIGHT | ANF_TOP )
END_ANCHOR_MAP()


BEGIN_MESSAGE_MAP(CUDPView, CDockablePaneChildView)
	ON_BN_CLICKED(IDOK, &CUDPView::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CUDPView::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_START, &CUDPView::OnBnClickedButtonStart)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_UPDATE, &CUDPView::OnBnClickedButtonUpdate)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CUDPView message handlers
BOOL CUDPView::OnInitDialog()
{
	CDockablePaneChildView::OnInitDialog();

	InitAnchors();


	CString command =
L"2, uint #test\r\n"
L"4, float\r\n"
L"4, float\r\n"
L"4, float\r\n"
L"4, float\r\n";

	CString cmdStr;
	std::ifstream cfgfile("udpanalyzer_udp.cfg");
	if (cfgfile.is_open())
	{
		std::string str((std::istreambuf_iterator<char>(cfgfile)), std::istreambuf_iterator<char>());
		cmdStr = str2wstr(str).c_str();
	}
	else
	{
		cmdStr = command;
	}
	m_ProtocolEditor.SetWindowTextW(cmdStr);

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
	}
	else
	{
		if (cController::Get()->GetUDPComm().InitServer(m_Port))
		{
			m_StartButton.SetWindowTextW(L"Server Stop");

			CString text;
			m_ProtocolEditor.GetWindowTextW(text);
			m_protocolParser.ParseStr(wstr2str((LPCTSTR)text));
// 			CString str;
// 			m_CommandEditor.GetWindowText(str);
// 			m_multiPlotWindows->ProcessPlotCommand(str);
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

	if (m_protocolParser.m_fieldsByteSize > bufferLen)
		return;

	int i = 0;
	int index = 0;
	const char *pmem = buffer;
	for each (auto &field in m_protocolParser.m_fields)
	{
		script::sFieldData data;
		ZeroMemory(data.buff, sizeof(data));
		memcpy(data.buff, pmem, field.bytes);
		data.type = field.type;

		const string id = format("$%d", i + 1);
		script::g_symbols[id] = data;
		
		index += field.bytes;
		pmem += field.bytes;
		++i;
	}
}


void CUDPView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePaneChildView::OnSize(nType, cx, cy);

	CRect rcWnd;
	GetWindowRect(&rcWnd);
	HandleAnchors(&rcWnd);
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
	UpdateData();

	// 환경파일 저장
	std::ofstream cfgfile("udpanalyzer_udp.cfg");
	if (cfgfile.is_open())
	{
		CString command;
		m_ProtocolEditor.GetWindowTextW(command);
		string str = wstr2str((LPCTSTR)command);
		cfgfile << str;
	}

	CDockablePaneChildView::OnDestroy();	
}
