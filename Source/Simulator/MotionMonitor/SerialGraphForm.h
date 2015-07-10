#pragma once

#include "afxwin.h"


// CSerialGraphForm dialog
class CMultiPlotWindow;
class CSerialGraphForm : public CDockablePaneChildView
{
public:
	CSerialGraphForm(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSerialGraphForm();

// Dialog Data
	enum { IDD = IDD_DIALOG_SERIAL_GRAPH };

	// SerialObserver Interface
	virtual void UpdateSerial(char *buffer) override;
	// UpdateObserver Interface
	virtual void Update(const float deltaSeconds) override;


protected:
	CMultiPlotWindow *m_multiPlotWindows;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_ANCHOR_MAP();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
	CEdit m_CommandEditor;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedButtonUpdate();
	CButton m_UpdateButton;
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnBnClickedCheckFixedmode();
	CButton m_FixedModeButton;
	CStatic m_Static1;
	CButton m_HelpButton;
	afx_msg void OnBnClickedButtonHelp();
};
