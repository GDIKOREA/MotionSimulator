// ModulatorEditDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MotionMonitor.h"
#include "EditDlg.h"
#include "afxdialogex.h"


// CModulatorEditDlg dialog
CEditDlg::CEditDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CEditDlg::IDD, pParent)
	, m_linkEditControl(NULL)
{
}

CEditDlg::~CEditDlg()
{
}

void CEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_Editor);
}



BEGIN_ANCHOR_MAP(CEditDlg)
	ANCHOR_MAP_ENTRY(IDC_EDIT1, ANF_LEFT | ANF_TOP | ANF_BOTTOM | ANF_RIGHT)
	ANCHOR_MAP_ENTRY(IDC_BUTTON_UPDATE, ANF_BOTTOM | ANF_RIGHT)
	ANCHOR_MAP_ENTRY(IDC_BUTTON_CLOSE, ANF_BOTTOM | ANF_RIGHT)
END_ANCHOR_MAP()


BEGIN_MESSAGE_MAP(CEditDlg, CDialogEx)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDOK, &CEditDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CEditDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_UPDATE, &CEditDlg::OnBnClickedButtonUpdate)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CEditDlg::OnBnClickedButtonClose)
END_MESSAGE_MAP()


// CModulatorEditDlg message handlers

BOOL CEditDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	InitAnchors();

	return TRUE;
}


void CEditDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	CRect rcWnd;
	GetWindowRect(&rcWnd);
	HandleAnchors(&rcWnd);
}


// 클래스 초기화
bool CEditDlg::Init(const CString &windowName, CWnd *linkEditControl,
	CDockablePaneChildView *owner, UpdateFunc func)
{
	RETV(!linkEditControl, false);

	m_linkEditControl = linkEditControl;
	m_funcOwner = owner;
	m_updateFunc = func;

	CString text;
	linkEditControl->GetWindowTextW(text);
	m_Editor.SetWindowTextW(text);

	linkEditControl->EnableWindow(FALSE);
	SetWindowTextW(windowName);

	return true;
}


void CEditDlg::OnBnClickedOk()
{ // 아무일 없음
}
void CEditDlg::OnBnClickedCancel()
{
	CDialogEx::OnCancel();
	if (m_linkEditControl)
		m_linkEditControl->EnableWindow(TRUE);
}


void CEditDlg::OnBnClickedButtonUpdate()
{
	if (m_linkEditControl)
	{
		CString text;
		m_Editor.GetWindowTextW(text);
		m_linkEditControl->SetWindowTextW(text);
		(m_funcOwner->*m_updateFunc)();
	}
}


void CEditDlg::OnBnClickedButtonClose()
{
	OnBnClickedCancel();
}
