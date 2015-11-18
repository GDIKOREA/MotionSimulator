#pragma once


// CNoticeDlg dialog

class CNoticeDlg : public CDialogEx
{
public:
	CNoticeDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CNoticeDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_SEARCH_PORT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
