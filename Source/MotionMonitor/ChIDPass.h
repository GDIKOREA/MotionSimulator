#pragma once


// CChIDPass dialog

class CChIDPass : public CDialogEx
{
	DECLARE_DYNAMIC(CChIDPass)

public:
	CChIDPass(CWnd* pParent = NULL);   // standard constructor
	virtual ~CChIDPass();

// Dialog Data
	enum { IDD = IDD_DIALOG_CHANGEIDPASS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	CString m_NewId;
	CString m_NewPsswd;
	CString m_NewPasswdConfirm;
};
