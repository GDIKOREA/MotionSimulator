#pragma once
#include "afxwin.h"


// CMotionInputView dialog

class CMotionInputView : public CDockablePaneChildView
{
public:
	CMotionInputView(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMotionInputView();

// Dialog Data
	enum { IDD = IDD_DIALOG_MOTION_INPUT };

	virtual void Update(const float deltaSeconds) override;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	CMultiPlotWindow *m_multiPlotWindows;
	float m_incTime;


	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonUpdate();
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CButton m_UpdateButton;
};
