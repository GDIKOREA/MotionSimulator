#pragma once
#include "afxwin.h"


// CDirt3View dialog

class CDirt3View : public CDockablePaneChildView
{
public:
	CDirt3View(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDirt3View();

// Dialog Data
	enum { IDD = IDD_DIALOG_DIRT3 };

	virtual void Update(const float deltaSeconds) override;
	virtual void UpdateUDP(const char *buffer, const int bufferLen) override;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	enum STATE {STOP,START,};
	STATE m_state;


	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonStart();
	CButton m_StartButton;
	BOOL m_IsMotionSim;
	afx_msg void OnBnClickedCheckMotionSim();
};
