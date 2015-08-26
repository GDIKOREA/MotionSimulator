#pragma once


// CUDPParseView dialog

class CUDPParseView : public CDockablePaneChildView
{
public:
	CUDPParseView(CWnd* pParent = NULL);   // standard constructor
	virtual ~CUDPParseView();

// Dialog Data
	enum { IDD = IDD_DIALOG_UDPPARSE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
};
