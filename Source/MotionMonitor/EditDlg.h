#pragma once
#include "afxwin.h"


// 클래스 멤버 함수 포인터 선언
typedef void (CDockablePaneChildView::*UpdateFunc)(void);


// CModulatorEditDlg dialog
class CEditDlg : public CDialogEx
{
public:
	CEditDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CEditDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_EDIT };

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
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonUpdate();
	afx_msg void OnBnClickedButtonClose();
	CEdit m_Editor;
};
