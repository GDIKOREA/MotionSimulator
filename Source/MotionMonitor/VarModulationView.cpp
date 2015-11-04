// VarModulationView.cpp : implementation file
//

#include "stdafx.h"
#include "MotionMonitor.h"
#include "VarModulationView.h"
#include "afxdialogex.h"


// CVarModulationView dialog
CVarModulationView::CVarModulationView(CWnd* pParent /*=NULL*/)
	: CDockablePaneChildView(CVarModulationView::IDD, pParent)
{
}

CVarModulationView::~CVarModulationView()
{
}

void CVarModulationView::DoDataExchange(CDataExchange* pDX)
{
	CDockablePaneChildView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_INPUTVAR, m_editInputVar);
	DDX_Control(pDX, IDC_EDIT_OUTPUTVAR, m_editOutputVar);
	DDX_Control(pDX, IDC_EDIT_SCRIPT, m_editScript);
}


BEGIN_ANCHOR_MAP(CVarModulationView)
	ANCHOR_MAP_ENTRY(IDC_EDIT_INPUTVAR, ANF_LEFT | ANF_TOP | ANF_RIGHT)
	ANCHOR_MAP_ENTRY(IDC_EDIT_OUTPUTVAR, ANF_LEFT | ANF_TOP | ANF_RIGHT)
	ANCHOR_MAP_ENTRY(IDC_EDIT_SCRIPT, ANF_LEFT | ANF_TOP | ANF_RIGHT)
	ANCHOR_MAP_ENTRY(IDC_BUTTON_UPDATE, ANF_TOP | ANF_RIGHT)
END_ANCHOR_MAP()

BEGIN_MESSAGE_MAP(CVarModulationView, CDockablePaneChildView)
	ON_BN_CLICKED(IDOK, &CVarModulationView::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CVarModulationView::OnBnClickedCancel)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CVarModulationView message handlers
void CVarModulationView::OnBnClickedOk()
{
}


void CVarModulationView::OnBnClickedCancel()
{
}


BOOL CVarModulationView::OnInitDialog()
{
	CDockablePaneChildView::OnInitDialog();

	InitAnchors();

	return TRUE;
}


void CVarModulationView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePaneChildView::OnSize(nType, cx, cy);

	CRect rcWnd;
	GetWindowRect(&rcWnd);
	HandleAnchors(&rcWnd);
}


void CVarModulationView::UpdateConfig(bool IsSaveAndValidate)
{
	if (IsSaveAndValidate)
	{
		m_editInputVar.SetWindowText(common::str2wstr(cMotionController::Get()->m_config.m_varModViewInputVar).c_str());
		m_editOutputVar.SetWindowText(common::str2wstr(cMotionController::Get()->m_config.m_varModViewOutputVar).c_str());
		m_editScript.SetWindowText(common::str2wstr(cMotionController::Get()->m_config.m_varModViewScript).c_str());
	}
	else
	{
		CString strInputVar;
		m_editInputVar.GetWindowTextW(strInputVar);
		cMotionController::Get()->m_config.m_varModViewInputVar = common::wstr2str((LPCTSTR)strInputVar);

		CString strOutputVar;
		m_editOutputVar.GetWindowTextW(strOutputVar);
		cMotionController::Get()->m_config.m_varModViewOutputVar = common::wstr2str((LPCTSTR)strOutputVar);

		CString strScript;
		m_editScript.GetWindowTextW(strScript);
		cMotionController::Get()->m_config.m_varModViewScript = common::wstr2str((LPCTSTR)strScript);
	}
}


void CVarModulationView::Update(const float deltaSeconds)
{

}
