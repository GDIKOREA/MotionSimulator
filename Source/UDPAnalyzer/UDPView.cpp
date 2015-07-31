// UDPView.cpp : implementation file
//

#include "stdafx.h"
#include "UDPAnalyzer.h"
#include "UDPView.h"
#include "afxdialogex.h"


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
END_MESSAGE_MAP()


// CUDPView message handlers
BOOL CUDPView::OnInitDialog()
{
	CDockablePaneChildView::OnInitDialog();

	InitAnchors();

	CString cmd =
		L"2, uint #test\r\n"
L"4, float\r\n"
L"4, float\r\n"
L"4, float\r\n"
L"4, float\r\n";
	m_ProtocolEditor.SetWindowTextW(cmd);

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
	RET(m_fieldsData.empty());

	if (m_protocolParser.m_fieldsByteSize > bufferLen)
		return;

	int i = 0;
	int index = 0;
	for each (auto &field in m_protocolParser.m_fields)
	{
		ZeroMemory(m_fieldsData[i].buff, sizeof(m_fieldsData[i]));
		memcpy(m_fieldsData[i].buff, buffer, field.bytes);
		m_fieldsData[i].type = field.type;
		
		index += field.bytes;
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
	CString text;
	m_ProtocolEditor.GetWindowTextW(text);
	m_protocolParser.ParseStr(wstr2str((LPCTSTR)text));

	if (!m_protocolParser.m_fields.empty())
	{
		m_fieldsData.clear();
		m_fieldsData.resize(m_protocolParser.m_fields.size());
	}
}
