// C3DSimulView.cpp : implementation file
//

#include "stdafx.h"
#include "MotionMonitor.h"
#include "C3DSimulView.h"
#include "afxdialogex.h"


// C3DSimulView dialog
C3DSimulView::C3DSimulView(CWnd* pParent /*=NULL*/)
	: CDockablePaneChildView(C3DSimulView::IDD, pParent)
{

}

C3DSimulView::~C3DSimulView()
{
}

void C3DSimulView::DoDataExchange(CDataExchange* pDX)
{
	CDockablePaneChildView::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(C3DSimulView, CDockablePaneChildView)
	ON_BN_CLICKED(IDOK, &C3DSimulView::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &C3DSimulView::OnBnClickedCancel)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// C3DSimulView message handlers


void C3DSimulView::OnBnClickedOk()
{
}
void C3DSimulView::OnBnClickedCancel()
{
}


BOOL C3DSimulView::OnInitDialog()
{
	CDockablePaneChildView::OnInitDialog();
	return TRUE;
}


void C3DSimulView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePaneChildView::OnSize(nType, cx, cy);

}


void C3DSimulView::Update(const float deltaSeconds)
{
	const float yaw = cMotionController::Get()->m_yaw;
	const float pitch = cMotionController::Get()->m_pitch;
	const float roll = cMotionController::Get()->m_roll;
	const float heave = cMotionController::Get()->m_heave;

// 	g_3dMotionView->GetCar().SetEulerAngle(
// 		(roll-255.f) / 255.f, 
// 		(pitch-255.f) / 255.f, 
// 		(yaw-255.f) / 255);

	g_3dMotionView->GetCar().SetEulerAngle(
		(roll - 10000.f) / 10000.f,
		(pitch - 10000.f) / 10000.f,
		(yaw - 10000.f) / 10000.f, 
		(heave - 10000.f) / 10000.f);

}
