#pragma once


// COutputView dialog

class COutputView : public CDockablePaneChildView
{
public:
	COutputView(CWnd* pParent = NULL);   // standard constructor
	virtual ~COutputView();

// Dialog Data
	enum { IDD = IDD_DIALOG_OUTPUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
