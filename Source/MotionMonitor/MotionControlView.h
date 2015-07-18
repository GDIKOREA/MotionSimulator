#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CMotionControlView dialog

class CMotionControlView : public CDockablePaneChildView
{
public:
	CMotionControlView(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMotionControlView();

// Dialog Data
	enum { IDD = IDD_DIALOG_MOTION_CONTROL };

	virtual void Update(const float deltaSeconds) override;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	CMultiPlotWindow *m_multiPlotWindows;
	float m_incTime;


	DECLARE_ANCHOR_MAP();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedButtonUpdate();
	CButton m_UpdateButton;
	CRichEditCtrl m_EditCommand;
};
