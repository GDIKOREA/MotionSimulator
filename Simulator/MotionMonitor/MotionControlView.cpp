// MotionControlView.cpp : implementation file
//

#include "stdafx.h"
#include "MotionMonitor.h"
#include "MotionControlView.h"
#include "afxdialogex.h"
#include "MotionController.h"


// CMotionControlView dialog
CMotionControlView::CMotionControlView(CWnd* pParent /*=NULL*/)
	: CDockablePaneChildView(CMotionControlView::IDD, pParent)
	, m_multiPlotWindows(NULL)
	, m_incTime(0)
	, m_EditYawMaxDeriv(0)
	, m_EditPitchMaxDeriv(0)
	, m_EditRollMaxDeriv(0)
{

}

CMotionControlView::~CMotionControlView()
{
}

void CMotionControlView::DoDataExchange(CDataExchange* pDX)
{
	CDockablePaneChildView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_UPDATE, m_UpdateButton);
	DDX_Control(pDX, IDC_SLIDER_YAW_DERIVATION, m_YawDerivSlider);
	DDX_Control(pDX, IDC_SLIDER_PITCH_DERIVATION, m_PitchDerivSlider);
	DDX_Control(pDX, IDC_SLIDER_ROLL_DERIVATION, m_RollDerivSlider);
	DDX_Text(pDX, IDC_EDIT_YAW_DERIVATION, m_EditYawMaxDeriv);
	DDX_Text(pDX, IDC_EDIT_PITCH_DERIVATION, m_EditPitchMaxDeriv);
	DDX_Text(pDX, IDC_EDIT_ROLL_DERIVATION, m_EditRollMaxDeriv);
}


BEGIN_ANCHOR_MAP(CMotionControlView)
	ANCHOR_MAP_ENTRY(IDC_STATIC_GRAPH, ANF_LEFT | ANF_TOP | ANF_RIGHT | ANF_BOTTOM)
END_ANCHOR_MAP()

BEGIN_MESSAGE_MAP(CMotionControlView, CDockablePaneChildView)
	ON_BN_CLICKED(IDOK, &CMotionControlView::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CMotionControlView::OnBnClickedCancel)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_UPDATE, &CMotionControlView::OnBnClickedButtonUpdate)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_YAW_DERIVATION, &CMotionControlView::OnNMCustomdrawSliderYawDerivation)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_PITCH_DERIVATION, &CMotionControlView::OnNMCustomdrawSliderPitchDerivation)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_ROLL_DERIVATION, &CMotionControlView::OnNMCustomdrawSliderRollDerivation)
	ON_EN_CHANGE(IDC_EDIT_YAW_DERIVATION, &CMotionControlView::OnEnChangeEditYawDerivation)
	ON_EN_CHANGE(IDC_EDIT_PITCH_DERIVATION, &CMotionControlView::OnEnChangeEditPitchDerivation)
	ON_EN_CHANGE(IDC_EDIT_ROLL_DERIVATION, &CMotionControlView::OnEnChangeEditRollDerivation)
END_MESSAGE_MAP()


// CMotionControlView message handlers


void CMotionControlView::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//CDockablePaneChildView::OnOK();
}


void CMotionControlView::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	//CDockablePaneChildView::OnCancel();
}


BOOL CMotionControlView::OnInitDialog()
{
	CDockablePaneChildView::OnInitDialog();

	InitAnchors();

	// PlotÃ¢ »ý¼º.
	CRect rect;
	GetClientRect(rect);

	m_multiPlotWindows = new CMultiPlotWindow();
	BOOL result = m_multiPlotWindows->Create(NULL, NULL, WS_VISIBLE | WS_CHILD,
		CRect(0, 0, rect.Width(), 400), this, AFX_IDW_PANE_FIRST);

	m_multiPlotWindows->SetScrollSizes(MM_TEXT, CSize(rect.Width() - 30, 100));
	m_multiPlotWindows->ShowWindow(SW_SHOW);

	m_YawDerivSlider.SetRange(0, 2000);
	m_YawDerivSlider.SetPos(500);

	m_PitchDerivSlider.SetRange(0, 2000);
	m_PitchDerivSlider.SetPos(500);

	m_RollDerivSlider.SetRange(0, 2000);
	m_RollDerivSlider.SetPos(500);


	return TRUE;
}


void CMotionControlView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePaneChildView::OnSize(nType, cx, cy);

	CRect rcWnd;
	GetWindowRect(&rcWnd);
	HandleAnchors(&rcWnd);

	if (m_multiPlotWindows && m_multiPlotWindows->GetSafeHwnd())
	{
		CRect pwr;
		GetDlgItem(IDC_STATIC_GRAPH)->GetWindowRect(pwr);

// 		CRect hwr;
// 		m_UpdateButton.GetWindowRect(hwr);
 		ScreenToClient(pwr);
		m_multiPlotWindows->MoveWindow(pwr);// CRect(0, hwr.bottom + 5, cx, cy));
//		m_multiPlotWindows->MoveWindow(CRect(0, 0, cx, cy));
	}
}


void CMotionControlView::Update(const float deltaSeconds)
{
	if (!m_multiPlotWindows)
		return;

	m_incTime += deltaSeconds;

	if (m_incTime > 0.033f)
	{
		float roll, pitch, yaw;
		cMotionController::Get()->GetMotion(yaw, pitch, roll);
		m_multiPlotWindows->SetString( common::format("%f;%f;%f;", yaw, pitch, roll).c_str() );

		float originalRoll, originalPitch, originalYaw;
		cMotionController::Get()->GetOriginalMotion(originalYaw, originalPitch, originalRoll);
		m_multiPlotWindows->SetString(common::format("%f;%f;%f;", originalYaw, originalPitch, originalRoll).c_str(), 1);


		m_multiPlotWindows->DrawGraph(m_incTime);

		m_incTime = 0.f;
	}
}


void CMotionControlView::OnBnClickedButtonUpdate()
{
	CString plotCommand = L"plot1 = 0, 0, 0, 0, 0\r\n\
string1 = %f;\r\n\
plot2 = 0, 0, 0, 0, 0\r\n\
string2 = %*f; %f;\r\n\
plot3 = 0, 0, 0, 0, 0\r\n\
string3 = %*f; %*f; %f;\r\n";
	m_multiPlotWindows->ProcessPlotCommand(plotCommand, 2);
	m_multiPlotWindows->SetFixedWidthMode(true);

	vector<string> names;
	names.push_back("Yaw");
	names.push_back("Pitch");
	names.push_back("Roll");
	names.push_back("Heave");
	m_multiPlotWindows->SetPlotName(names);
}


void CMotionControlView::OnNMCustomdrawSliderYawDerivation(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	
	const float r = m_YawDerivSlider.GetPos() / 1000.f;
	const float deriv = r * 10;

	m_EditYawMaxDeriv = deriv;
	cMotionController::Get()->SetMaxYawDerivation(deriv);

	UpdateData(FALSE);

	*pResult = 0;
}


void CMotionControlView::OnNMCustomdrawSliderPitchDerivation(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);

	const float r = m_PitchDerivSlider.GetPos() / 1000.f;
	const float deriv = r * 10;

	m_EditPitchMaxDeriv = deriv;
	cMotionController::Get()->SetMaxPitchDerivation(deriv);
	 
	UpdateData(FALSE);

	*pResult = 0;
}


void CMotionControlView::OnNMCustomdrawSliderRollDerivation(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);

	const float r = m_RollDerivSlider.GetPos() / 1000.f;
	const float deriv = r * 10;

	m_EditRollMaxDeriv = deriv;
	cMotionController::Get()->SetMaxRollDerivation(deriv);

	UpdateData(FALSE);


	*pResult = 0;
}


void CMotionControlView::OnEnChangeEditYawDerivation()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDockablePaneChildView::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}


void CMotionControlView::OnEnChangeEditPitchDerivation()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDockablePaneChildView::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}


void CMotionControlView::OnEnChangeEditRollDerivation()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDockablePaneChildView::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}
