#pragma once
#include "afxwin.h"
#include "afxcmn.h"

#include "ExeTracker.h"

// CLauncherView dialog

class CLauncherView : public CDockablePaneChildView
{
public:
	CLauncherView(CWnd* pParent = NULL);   // standard constructor
	virtual ~CLauncherView();

// Dialog Data
	enum { IDD = IDD_DIALOG_LAUNCHER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	const float m_camSensitiveMid; // 카메라 감도 중간 값 = 2.3
	float m_camSens1;
	float m_camSens2;

	cExeTracker m_gameExeTracker;


	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonGamestart();
	afx_msg void OnBnClickedButtonPlayer1();
	afx_msg void OnBnClickedButtonPlayer2();
	afx_msg void OnBnClickedButtonCamAdjustmentP1();
	afx_msg void OnBnClickedButtonCamAdjustmentP2();
	CComboBox m_DifficultCombo;
	CComboBox m_DifficultCombo2;
	CSliderCtrl m_CamSensSlider;
	float m_CamSens1Edit;
	float m_CamSens2Edit;
	afx_msg void OnNMCustomdrawSliderCamSens(NMHDR *pNMHDR, LRESULT *pResult);
	CSliderCtrl m_CamSens2Slider;
	afx_msg void OnNMCustomdrawSliderCamSens2(NMHDR *pNMHDR, LRESULT *pResult);
};
