// PlotInputDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MotionMonitor.h"
#include "PlotInputDlg.h"
#include "afxdialogex.h"


// CPlotInputDlg dialog

CPlotInputDlg::CPlotInputDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPlotInputDlg::IDD, pParent)
	, m_linkEditControl(NULL)
{
}

CPlotInputDlg::~CPlotInputDlg()
{
}

void CPlotInputDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_PLOTINPUT_FORMAT, m_Editor);
}

BEGIN_ANCHOR_MAP(CPlotInputDlg)
	ANCHOR_MAP_ENTRY(IDC_EDIT_PLOTINPUT_FORMAT, ANF_LEFT | ANF_TOP | ANF_BOTTOM | ANF_RIGHT)
	ANCHOR_MAP_ENTRY(IDC_EDIT_INPUTSTR1, ANF_BOTTOM | ANF_LEFT)
	ANCHOR_MAP_ENTRY(IDC_EDIT_INPUTSTR2, ANF_BOTTOM | ANF_LEFT)
	ANCHOR_MAP_ENTRY(IDC_EDIT_INPUTSTR3, ANF_BOTTOM | ANF_LEFT)
	ANCHOR_MAP_ENTRY(IDC_EDIT_INPUTSTR4, ANF_BOTTOM | ANF_LEFT)
	ANCHOR_MAP_ENTRY(IDC_BUTTON_UPDATE, ANF_BOTTOM | ANF_RIGHT)
	ANCHOR_MAP_ENTRY(IDC_BUTTON_CLOSE, ANF_BOTTOM | ANF_RIGHT)
END_ANCHOR_MAP()

BEGIN_MESSAGE_MAP(CPlotInputDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CPlotInputDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CPlotInputDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_UPDATE, &CPlotInputDlg::OnBnClickedButtonUpdate)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CPlotInputDlg::OnBnClickedButtonClose)
	ON_WM_SIZE()
END_MESSAGE_MAP()



BOOL CPlotInputDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	InitAnchors();

	return TRUE;
}


// CPlotInputDlg message handlers
void CPlotInputDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	CRect rcWnd;
	GetWindowRect(&rcWnd);
	HandleAnchors(&rcWnd);
}


// 클래스 초기화
bool CPlotInputDlg::Init(const CString &windowName, CWnd *linkEditControl,
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



void CPlotInputDlg::OnBnClickedOk()
{
//	CDialogEx::OnOK();
}


void CPlotInputDlg::OnBnClickedCancel()
{
	CDialogEx::OnCancel();
	if (m_linkEditControl)
		m_linkEditControl->EnableWindow(TRUE);
}


void CPlotInputDlg::OnBnClickedButtonUpdate()
{
	if (m_linkEditControl)
	{
		CString text;
		m_Editor.GetWindowTextW(text);
		m_linkEditControl->SetWindowTextW(text);
		(m_funcOwner->*m_updateFunc)();
	}

}


void CPlotInputDlg::OnBnClickedButtonClose()
{
	OnBnClickedCancel();
}
