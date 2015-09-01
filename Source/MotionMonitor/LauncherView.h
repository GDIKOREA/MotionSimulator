#pragma once
#include "afxwin.h"
#include "afxcmn.h"


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
	CSliderCtrl m_CamSensSlider;
	float m_CamSensEdit;
	afx_msg void OnNMCustomdrawSliderCamSens(NMHDR *pNMHDR, LRESULT *pResult);
};
