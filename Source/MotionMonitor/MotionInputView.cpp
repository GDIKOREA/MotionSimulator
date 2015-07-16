// MotionInputView.cpp : implementation file
//

#include "stdafx.h"
#include "MotionMonitor.h"
#include "MotionInputView.h"
#include "afxdialogex.h"
#include "MotionController.h"


// CMotionInputView dialog
CMotionInputView::CMotionInputView(CWnd* pParent /*=NULL*/)
	: CDockablePaneChildView(CMotionInputView::IDD, pParent)
	, m_multiPlotWindows(NULL)
	, m_incTime(0)
{

}

CMotionInputView::~CMotionInputView()
{
}

void CMotionInputView::DoDataExchange(CDataExchange* pDX)
{
	CDockablePaneChildView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_UPDATE, m_UpdateButton);
}


BEGIN_MESSAGE_MAP(CMotionInputView, CDockablePaneChildView)
	ON_BN_CLICKED(IDOK, &CMotionInputView::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CMotionInputView::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_UPDATE, &CMotionInputView::OnBnClickedButtonUpdate)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CMotionInputView message handlers


void CMotionInputView::OnBnClickedOk()
{
	//CDockablePaneChildView::OnOK();
}


void CMotionInputView::OnBnClickedCancel()
{
	//CDockablePaneChildView::OnCancel();
}


BOOL CMotionInputView::OnInitDialog()
{
	CDockablePaneChildView::OnInitDialog();

	// PlotÃ¢ »ý¼º.
	CRect rect;
	GetClientRect(rect);

	m_multiPlotWindows = new CMultiPlotWindow();
	BOOL result = m_multiPlotWindows->Create(NULL, NULL, WS_VISIBLE | WS_CHILD,
		CRect(0, 0, rect.Width(), 400), this, AFX_IDW_PANE_FIRST);

	m_multiPlotWindows->SetScrollSizes(MM_TEXT, CSize(rect.Width() - 30, 100));
	m_multiPlotWindows->ShowWindow(SW_SHOW);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CMotionInputView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePaneChildView::OnSize(nType, cx, cy);

	CRect rcWnd;
	GetWindowRect(&rcWnd);
//	HandleAnchors(&rcWnd);

	if (m_multiPlotWindows && m_multiPlotWindows->GetSafeHwnd())
	{
		CRect hwr;
		m_UpdateButton.GetWindowRect(hwr);
		ScreenToClient(hwr);
		m_multiPlotWindows->MoveWindow(CRect(0, hwr.bottom + 5, cx, cy));
	}
}


void CMotionInputView::OnBnClickedButtonUpdate()
{
		CString plotCommand = L"plot1 = 0, 0, 0, 0, 0\r\n\
string1 = %f;\r\n\
name1 = Yaw\r\n\
plot2 = 0, 0, 0, 0, 0\r\n\
string2 = %*f; %f;\r\n\
name2 = Pitch\r\n\
plot3 = 0, 0, 0, 0, 0\r\n\
string3 = %*f; %*f; %f;\r\n\
name3 = Roll\r\n";
	m_multiPlotWindows->ProcessPlotCommand(plotCommand);	
	m_multiPlotWindows->SetFixedWidthMode(true);
}


// Update
void CMotionInputView::Update(const float deltaSeconds)
{
	if (!m_multiPlotWindows)
		return;

	m_incTime += deltaSeconds;

	if (m_incTime > 0.033f)
	{
		float roll, pitch, yaw;
		cMotionController::Get()->GetUDPOriginalMotion(yaw, pitch, roll);
		m_multiPlotWindows->SetString(common::format("%f;%f;%f;", yaw, pitch, roll).c_str());

		m_multiPlotWindows->DrawGraph(m_incTime);

		m_incTime = 0.f;
	}
}
