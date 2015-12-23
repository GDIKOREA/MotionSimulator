#pragma once
#include "afxwin.h"


// CRealShotView dialog

class CRealShotView : public CDockablePaneChildView
{
public:
	CRealShotView(CWnd* pParent = NULL);   // standard constructor
	virtual ~CRealShotView();

// Dialog Data
	enum { IDD = IDD_DIALOG_REALSHOT };

	virtual void Update(const float deltaSeconds) override;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	bool m_isStart;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonCalibration();
	afx_msg void OnBnClickedButtonStart();
	CButton m_StartButton;
};
