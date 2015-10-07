#pragma once

#include "EditDlg.h"
#include "afxwin.h"


// CPlotInputDlg dialog

class CPlotInputDlg : public CDialogEx
{
public:
	CPlotInputDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPlotInputDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_PLOTINPUT };

	bool Init(const CString &windowName, CWnd *linkEditControl,
		CDockablePaneChildView *owner, UpdateFunc func);


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	CWnd *m_linkEditControl;
	CDockablePaneChildView *m_funcOwner;
	UpdateFunc m_updateFunc;


	DECLARE_ANCHOR_MAP();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonUpdate();
	afx_msg void OnBnClickedButtonClose();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	CEdit m_Editor;
};
