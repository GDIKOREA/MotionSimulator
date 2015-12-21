// VarModulationView.cpp : implementation file
//

#include "stdafx.h"
#include "MotionMonitor.h"
#include "VarModulationView.h"
#include "afxdialogex.h"


// CVarModulationView dialog
CVarModulationView::CVarModulationView(CWnd* pParent /*=NULL*/)
	: CDockablePaneChildView(CVarModulationView::IDD, pParent)
	, m_state(STOP)
	, m_incTime(0)
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
	DDX_Control(pDX, IDC_BUTTON_START, m_StartButton);
	DDX_Control(pDX, IDC_EDIT_INPUTVAR2, m_editInputVar2);
	DDX_Control(pDX, IDC_EDIT_OUTPUTVAR2, m_editOutputVar2);
	DDX_Control(pDX, IDC_EDIT_SCRIPT2, m_editScript2);
	DDX_Control(pDX, IDC_EDIT_INPUTVAR3, m_editInputVar3);
	DDX_Control(pDX, IDC_EDIT_OUTPUTVAR3, m_editOutputVar3);
	DDX_Control(pDX, IDC_EDIT_SCRIPT3, m_editScript3);
}


BEGIN_ANCHOR_MAP(CVarModulationView)
	ANCHOR_MAP_ENTRY(IDC_EDIT_INPUTVAR, ANF_LEFT | ANF_TOP | ANF_RIGHT)
	ANCHOR_MAP_ENTRY(IDC_EDIT_OUTPUTVAR, ANF_LEFT | ANF_TOP | ANF_RIGHT)
	ANCHOR_MAP_ENTRY(IDC_EDIT_SCRIPT, ANF_LEFT | ANF_TOP | ANF_RIGHT)
	ANCHOR_MAP_ENTRY(IDC_EDIT_INPUTVAR2, ANF_LEFT | ANF_TOP | ANF_RIGHT)
	ANCHOR_MAP_ENTRY(IDC_EDIT_OUTPUTVAR2, ANF_LEFT | ANF_TOP | ANF_RIGHT)
	ANCHOR_MAP_ENTRY(IDC_EDIT_SCRIPT2, ANF_LEFT | ANF_TOP | ANF_RIGHT)
	ANCHOR_MAP_ENTRY(IDC_EDIT_INPUTVAR3, ANF_LEFT | ANF_TOP | ANF_RIGHT)
	ANCHOR_MAP_ENTRY(IDC_EDIT_OUTPUTVAR3, ANF_LEFT | ANF_TOP | ANF_RIGHT)
	ANCHOR_MAP_ENTRY(IDC_EDIT_SCRIPT3, ANF_LEFT | ANF_TOP | ANF_RIGHT)
	ANCHOR_MAP_ENTRY(IDC_BUTTON_UPDATE, ANF_TOP | ANF_RIGHT)
END_ANCHOR_MAP()

BEGIN_MESSAGE_MAP(CVarModulationView, CDockablePaneChildView)
	ON_BN_CLICKED(IDOK, &CVarModulationView::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CVarModulationView::OnBnClickedCancel)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_START, &CVarModulationView::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_UPDATE, &CVarModulationView::OnBnClickedButtonUpdate)
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

	UpdateConfig();

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

		m_editInputVar2.SetWindowText(common::str2wstr(cMotionController::Get()->m_config.m_varModViewInputVar2).c_str());
		m_editOutputVar2.SetWindowText(common::str2wstr(cMotionController::Get()->m_config.m_varModViewOutputVar2).c_str());
		m_editScript2.SetWindowText(common::str2wstr(cMotionController::Get()->m_config.m_varModViewScript2).c_str());

		m_editInputVar3.SetWindowText(common::str2wstr(cMotionController::Get()->m_config.m_varModViewInputVar3).c_str());
		m_editOutputVar3.SetWindowText(common::str2wstr(cMotionController::Get()->m_config.m_varModViewOutputVar3).c_str());
		m_editScript3.SetWindowText(common::str2wstr(cMotionController::Get()->m_config.m_varModViewScript3).c_str());

		UpdateVariable();
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


		CString strInputVar2;
		m_editInputVar2.GetWindowTextW(strInputVar2);
		cMotionController::Get()->m_config.m_varModViewInputVar2 = common::wstr2str((LPCTSTR)strInputVar2);

		CString strOutputVar2;
		m_editOutputVar2.GetWindowTextW(strOutputVar2);
		cMotionController::Get()->m_config.m_varModViewOutputVar2 = common::wstr2str((LPCTSTR)strOutputVar2);

		CString strScript2;
		m_editScript2.GetWindowTextW(strScript2);
		cMotionController::Get()->m_config.m_varModViewScript2 = common::wstr2str((LPCTSTR)strScript2);


		CString strInputVar3;
		m_editInputVar3.GetWindowTextW(strInputVar3);
		cMotionController::Get()->m_config.m_varModViewInputVar3 = common::wstr2str((LPCTSTR)strInputVar3);

		CString strOutputVar3;
		m_editOutputVar3.GetWindowTextW(strOutputVar3);
		cMotionController::Get()->m_config.m_varModViewOutputVar3 = common::wstr2str((LPCTSTR)strOutputVar3);

		CString strScript3;
		m_editScript3.GetWindowTextW(strScript3);
		cMotionController::Get()->m_config.m_varModViewScript3 = common::wstr2str((LPCTSTR)strScript3);

	}
}


void CVarModulationView::Update(const float deltaSeconds)
{
	RET(m_state == STOP);

	m_incTime += deltaSeconds;

	if (m_incTime > 0.033f)
	{
		const float input1 = m_interpreter.Excute(m_inputVarParser1.m_stmt);
		const float input2 = m_interpreter.Excute(m_inputVarParser2.m_stmt);
		const float input3 = m_interpreter.Excute(m_inputVarParser3.m_stmt);
		cMotionController::Get()->m_varModulator1.Update(m_incTime, input1);
		cMotionController::Get()->m_varModulator2.Update(m_incTime, input2);
		cMotionController::Get()->m_varModulator3.Update(m_incTime, input3);

		if (!m_outputVar1.empty())
		{
			float x1;
			cMotionController::Get()->m_varModulator1.GetFinal(x1);

			script::sFieldData data;
			data.fVal = x1;
			data.type = script::FEILD_TYPE::T_FLOAT;
			script::g_symbols[m_outputVar1] = data;
		}

		if (!m_outputVar2.empty())
		{
			float x2;
			cMotionController::Get()->m_varModulator2.GetFinal(x2);

			script::sFieldData data;
			data.fVal = x2;
			data.type = script::FEILD_TYPE::T_FLOAT;
			script::g_symbols[m_outputVar2] = data;
		}

		if (!m_outputVar3.empty())
		{
			float x3;
			cMotionController::Get()->m_varModulator3.GetFinal(x3);

			script::sFieldData data;
			data.fVal = x3;
			data.type = script::FEILD_TYPE::T_FLOAT;
			script::g_symbols[m_outputVar3] = data;
		}

		m_incTime = 0;
	}
}


void CVarModulationView::OnBnClickedButtonStart()
{
	if (m_state == STOP)
	{
		Start();
	}
	else
	{
		Stop();
	}
}


void CVarModulationView::UpdateVariable()
{
	CString strInputVar;
	m_editInputVar.GetWindowTextW(strInputVar);
	m_inputVarParser1.ParseStr(common::wstr2str((LPCTSTR)strInputVar));

	CString strOutputVar;
	m_editOutputVar.GetWindowTextW(strOutputVar);
	m_outputVar1 = common::wstr2str((LPCTSTR)strOutputVar);

	CString strScript;
	m_editScript.GetWindowTextW(strScript);
	cMotionController::Get()->m_varModulator1.ParseStr(wstr2str((LPCTSTR)strScript));


	CString strInputVar2;
	m_editInputVar2.GetWindowTextW(strInputVar2);
	m_inputVarParser2.ParseStr(common::wstr2str((LPCTSTR)strInputVar2));

	CString strOutputVar2;
	m_editOutputVar2.GetWindowTextW(strOutputVar2);
	m_outputVar2 = common::wstr2str((LPCTSTR)strOutputVar2);

	CString strScript2;
	m_editScript2.GetWindowTextW(strScript2);
	cMotionController::Get()->m_varModulator2.ParseStr(wstr2str((LPCTSTR)strScript2));


	CString strInputVar3;
	m_editInputVar3.GetWindowTextW(strInputVar3);
	m_inputVarParser3.ParseStr(common::wstr2str((LPCTSTR)strInputVar3));

	CString strOutputVar3;
	m_editOutputVar3.GetWindowTextW(strOutputVar3);
	m_outputVar3 = common::wstr2str((LPCTSTR)strOutputVar3);

	CString strScript3;
	m_editScript3.GetWindowTextW(strScript3);
	cMotionController::Get()->m_varModulator3.ParseStr(wstr2str((LPCTSTR)strScript3));
}


void CVarModulationView::OnBnClickedButtonUpdate()
{
	UpdateVariable();
}


void CVarModulationView::Start()
{
	UpdateVariable();

	m_state = START;
	m_StartButton.SetWindowTextW(L"Stop");
	SetBackgroundColor(g_blueColor);
}


void CVarModulationView::Stop()
{
	m_state = STOP;
	m_StartButton.SetWindowTextW(L"Start");
	SetBackgroundColor(g_grayColor);
}
