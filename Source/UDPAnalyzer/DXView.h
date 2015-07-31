#pragma once


// CDXView dialog
class CDXView : public CDockablePaneChildView
{
public:
	CDXView(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDXView();

// Dialog Data
	enum { IDD = IDD_DIALOG_3D };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
