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
	, m_EditYawProportion(0)
	, m_EditPitchProp(0)
	, m_EditRollProp(0)
	, m_EditYawScale2(0)
	, m_EditPitchScale2(0)
	, m_EditRollScale2(0)
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
	DDX_Control(pDX, IDC_SLIDER_YAW_PROPORTION, m_YawPropSlider);
	DDX_Control(pDX, IDC_SLIDER_YAW_INTEGRAL, m_YawIntegralSlider);
	DDX_Text(pDX, IDC_EDIT_YAW_PROPORTION, m_EditYawProportion);
	DDX_Control(pDX, IDC_SLIDER_PITCH_PROPORTION, m_PitchPropSlider);
	DDX_Text(pDX, IDC_EDIT_PITCH_PROPORTION, m_EditPitchProp);
	DDX_Control(pDX, IDC_SLIDER_ROLL_PROPORTION, m_RollPropSlider);
	DDX_Text(pDX, IDC_EDIT_ROLL_PROPORTION, m_EditRollProp);
	DDX_Control(pDX, IDC_SLIDER_YAW_SCALE2, m_YawScale2Slider);
	DDX_Text(pDX, IDC_EDIT_YAW_SCALE2, m_EditYawScale2);
	DDX_Control(pDX, IDC_SLIDER_PITCH_SCALE2, m_PitchScale2Slider);
	DDX_Text(pDX, IDC_EDIT_PITCH_SCALE2, m_EditPitchScale2);
	DDX_Control(pDX, IDC_SLIDER_ROLL_SCALE2, m_RollScale2Slider);
	DDX_Text(pDX, IDC_EDIT_ROLL_SCALE2, m_EditRollScale2);
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
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_PITCH_PROPORTION, &CMotionControlView::OnNMCustomdrawSliderPitchProportion)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_ROLL_PROPORTION, &CMotionControlView::OnNMCustomdrawSliderRollProportion)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_YAW_PROPORTION, &CMotionControlView::OnNMCustomdrawSliderYawProportion)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_YAW_SCALE2, &CMotionControlView::OnNMCustomdrawSliderYawScale2)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_PITCH_SCALE2, &CMotionControlView::OnNMCustomdrawSliderPitchScale2)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_ROLL_SCALE2, &CMotionControlView::OnNMCustomdrawSliderRollScale2)
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

	m_YawDerivSlider.SetRange(0, 15000);
	m_YawDerivSlider.SetPos(10000);
	m_YawPropSlider.SetRange(0, 1000);
	m_YawPropSlider.SetPos(100);
	m_YawScale2Slider.SetRange(0, 1000);
	m_YawScale2Slider.SetPos(100);

	m_PitchDerivSlider.SetRange(0, 15000);
	m_PitchDerivSlider.SetPos(10000);
	m_PitchPropSlider.SetRange(0, 1000);
	m_PitchPropSlider.SetPos(100);
	m_PitchScale2Slider.SetRange(0, 1000);
	m_PitchScale2Slider.SetPos(100);

	m_RollDerivSlider.SetRange(0, 15000);
	m_RollDerivSlider.SetPos(10000);
	m_RollPropSlider.SetRange(0, 1000);
	m_RollPropSlider.SetPos(100);
	m_RollScale2Slider.SetRange(0, 1000);
	m_RollScale2Slider.SetPos(100);

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
		m_multiPlotWindows->SetString( common::format("%f;%f;%f;", yaw, pitch, roll).c_str(), 1 );

		float originalRoll, originalPitch, originalYaw;
		cMotionController::Get()->GetOriginalMotion(originalYaw, originalPitch, originalRoll);
		m_multiPlotWindows->SetString(common::format("%f;%f;%f;", originalYaw, originalPitch, originalRoll).c_str(), 0);


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
	const float deriv = r * 1;

	m_EditYawMaxDeriv = deriv;
	cMotionController::Get()->SetYawScale(deriv);

	UpdateData(FALSE);

	*pResult = 0;
}


void CMotionControlView::OnNMCustomdrawSliderPitchDerivation(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);

 	const float r = m_PitchDerivSlider.GetPos() / 1000.f;
 	const float deriv = r * 1;
 
 	m_EditPitchMaxDeriv = deriv;
 	cMotionController::Get()->SetPitchScale(deriv);

 	UpdateData(FALSE);

	*pResult = 0;
}


void CMotionControlView::OnNMCustomdrawSliderRollDerivation(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);

	const float r = m_RollDerivSlider.GetPos() / 1000.f;
	const float deriv = r * 1;

	m_EditRollMaxDeriv = deriv;
	cMotionController::Get()->SetRollScale(deriv);

	UpdateData(FALSE);

	*pResult = 0;
}


void CMotionControlView::OnNMCustomdrawSliderYawProportion(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);

	const float r = m_YawPropSlider.GetPos() / 1000.f;
	const float deriv = r * 1;

	m_EditYawProportion = deriv;
	cMotionController::Get()->SetYawProportion(deriv);

	UpdateData(FALSE);

	*pResult = 0;
}


void CMotionControlView::OnNMCustomdrawSliderPitchProportion(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);

	const float r = m_PitchPropSlider.GetPos() / 1000.f;
	const float deriv = r * 1;

	m_EditPitchProp = deriv;
	cMotionController::Get()->SetPitchProportion(deriv);

	UpdateData(FALSE);

	*pResult = 0;
}


void CMotionControlView::OnNMCustomdrawSliderRollProportion(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);

	const float r = m_RollPropSlider.GetPos() / 1000.f;
	const float deriv = r * 1;

	m_EditRollProp = deriv;
	cMotionController::Get()->SetRollProportion(deriv);

	UpdateData(FALSE);

	*pResult = 0;
}


void CMotionControlView::OnNMCustomdrawSliderYawScale2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);

	const float r = m_YawScale2Slider.GetPos() / 1000.f;
	const float deriv = r * 1;

	m_EditYawScale2 = deriv;
	cMotionController::Get()->SetYawScale2(deriv);

	UpdateData(FALSE);

	*pResult = 0;
}


void CMotionControlView::OnNMCustomdrawSliderPitchScale2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);

	const float r = m_PitchScale2Slider.GetPos() / 1000.f;
	const float deriv = r * 1;

	m_EditPitchScale2 = deriv;
	cMotionController::Get()->SetPitchScale2(deriv);

	UpdateData(FALSE);

	*pResult = 0;
}


void CMotionControlView::OnNMCustomdrawSliderRollScale2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);

	const float r = m_RollScale2Slider.GetPos() / 1000.f;
	const float deriv = r * 1;

	m_EditRollScale2 = deriv;
	cMotionController::Get()->SetRollScale2(deriv);

	UpdateData(FALSE);
	
	*pResult = 0;
}


void CMotionControlView::OnEnChangeEditYawDerivation()
{
}


void CMotionControlView::OnEnChangeEditPitchDerivation()
{
}


void CMotionControlView::OnEnChangeEditRollDerivation()
{
}

