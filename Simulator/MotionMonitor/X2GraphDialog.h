#pragma once


// CX2GraphDialog dialog

class CX2GraphDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CX2GraphDialog)

public:
	CX2GraphDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CX2GraphDialog();

// Dialog Data
	enum { IDD = IDD_DIALOG_X2GRAPH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
