#pragma once
#include "afxcmn.h"
#include "mixingconfig.h"

// CMixingView dialog

class CMixingView : public CDockablePaneChildView
{
public:
	CMixingView(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMixingView();

// Dialog Data
	enum { IDD = IDD_DIALOG_MIXING };

	virtual void Update(const float deltaSeconds) override;
	void Mixing(const float deltaSeconds, OUT float &yaw, OUT float &pitch, OUT float &roll, OUT float &heave);


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support


	CMultiPlotWindow *m_multiPlotWindows;
	float m_incTime;
	cMixingConfig m_config;
	bool m_isStart;


	DECLARE_ANCHOR_MAP();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonUpdate();
	CRichEditCtrl m_EditCommand;
};
