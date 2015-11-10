
// UDP2UDPDlg.h : header file
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "../Common/UIComponent/BPCtrlAnchorMap.h"


// CUDP2UDPDlg dialog
class CProxyWindow;
class cMultiProxy;
class CUDP2UDPDlg : public CDialogEx
{
// Construction
public:
	CUDP2UDPDlg(CWnd* pParent = NULL);	// standard constructor
	virtual ~CUDP2UDPDlg();

// Dialog Data
	enum { IDD = IDD_UDP2UDP_DIALOG };

	void MainLoop();


protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
//	void InitProxyWindows();
// 	CProxyWindow* AddProxyWindow(const HWND instHwnd = NULL);
// 	void RemoveProxyWindow(const HWND removeHwnd);
//	CProxyWindow* FindProxyWindow(const HWND hWnd);
//	void CalculateWindowSize();
	void Log(const string &str);
	void InitListControl();
	void UpdateListControl();
	void CloseAll();


// Implementation
protected:
	enum STATE {
		STOP,
		START
	};

	STATE m_state;
	HICON m_hIcon;
	bool m_loop;
	vector<cMultiProxy*> m_multiProxy;
	int m_RecvCountBuffer[100];


	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();

	DECLARE_ANCHOR_MAP();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CListBox m_LogList;
	afx_msg void OnButtonAdd();
	afx_msg void OnButtonRemove();
	afx_msg void OnAppCommand(CWnd* pWnd, UINT nCmd, UINT nDevice, UINT nKey);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButtonHelp();
	CEdit m_editScript;
	CListCtrl m_listState;
	afx_msg void OnBnClickedButtonPasteSample();
	CButton m_StartButton;
};
