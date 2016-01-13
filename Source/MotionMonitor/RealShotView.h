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


public:
	bool m_isCameraExeTerminate;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	bool m_isStart;
	cExeTracker m_cameraExeTracker;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonCalibration();
	afx_msg void OnBnClickedButtonStart();
	CButton m_StartButton;
	CStatic m_PrintStr;
	CButton m_CalibrationButton;
	virtual BOOL OnInitDialog();
};
