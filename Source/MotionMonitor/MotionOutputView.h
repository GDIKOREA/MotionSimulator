#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CMotionOutputView dialog

class CMotionOutputView : public CDockablePaneChildView
{
public:
	CMotionOutputView(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMotionOutputView();

// Dialog Data
	enum { IDD = IDD_DIALOG_MOTION_OUTPUT };

	virtual void Update(const float deltaSeconds) override;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void SendMotionControllSwitchMessage(const int state);

	CMultiPlotWindow *m_multiPlotWindows;
	bool m_isStartSendMotion;
	float m_incTime;
	float m_totalIncTime;
	float m_incSerialTime;


	DECLARE_ANCHOR_MAP();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CComPortCombo m_COMPortComboBox;
	CComboBox m_BaudRateComboBox;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonConnect();
	CButton m_ConnectButton;
	afx_msg void OnBnClickedButtonStartSendMotion();
	CButton m_StartSendMotionButton;
	CRichEditCtrl m_Log;
	CRichEditCtrl m_OutputLog;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButtonStop();
	afx_msg void OnBnClickedButtonOrigin();
	afx_msg void OnBnClickedButtonEmergency();
	afx_msg void OnBnClickedButtonServoOn();
	afx_msg void OnBnClickedButtonServoOff();
	BOOL m_IsOnlyEmergency;
	afx_msg void OnBnClickedCheck1();
};
