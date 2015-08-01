// MixingView.cpp : implementation file
//

#include "stdafx.h"
#include "UDPAnalyzer.h"
#include "MixingView.h"
#include "afxdialogex.h"


// CMixingView dialog
CMixingView::CMixingView(CWnd* pParent /*=NULL*/)
: CDockablePaneChildView(CMixingView::IDD, pParent)
, m_incTime(0)
{

}

CMixingView::~CMixingView()
{
}

void CMixingView::DoDataExchange(CDataExchange* pDX)
{
	CDockablePaneChildView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_COMMAND, m_CommandEditor);
}

BEGIN_ANCHOR_MAP(CMixingView)
	ANCHOR_MAP_ENTRY(IDC_EDIT_COMMAND, ANF_LEFT | ANF_RIGHT | ANF_TOP)
END_ANCHOR_MAP()

BEGIN_MESSAGE_MAP(CMixingView, CDockablePaneChildView)
	ON_BN_CLICKED(IDOK, &CMixingView::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CMixingView::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_UPDATE, &CMixingView::OnBnClickedButtonUpdate)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CMixingView message handlers

BOOL CMixingView::OnInitDialog()
{
	CDockablePaneChildView::OnInitDialog();

	InitAnchors();

	CString cmd =
		L"$var1 = $1 + $2 + $3\r\n"
		L"$var2 = $1 + $2 + $3\r\n"
		L"$var3 = $1 + $2 + $3\r\n";
	m_CommandEditor.SetWindowTextW(cmd);

	return TRUE;
}


void CMixingView::OnBnClickedOk()
{
}


void CMixingView::OnBnClickedCancel()
{
}


void CMixingView::OnBnClickedButtonUpdate()
{
	UpdateData();

	CString text;
	m_CommandEditor.GetWindowTextW(text);
	m_parser.ParseStr(wstr2str((LPCTSTR)text));
}


void CMixingView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePaneChildView::OnSize(nType, cx, cy);

	CRect rcWnd;
	GetWindowRect(&rcWnd);
	HandleAnchors(&rcWnd);
}


void CMixingView::Update(const float deltaSeconds)
{
	m_incTime += deltaSeconds;

	if (m_incTime > 0.033f)
	{
		m_interpreter.Excute(m_parser.m_stmt);
		m_incTime = 0;
	}
}
