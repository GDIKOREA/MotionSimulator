
// UDPNetGraphDlg.h : header file
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CUDPNetGraphDlg dialog
class CGraphWindow;
class CUDPNetGraphDlg : public CDialogEx
{
// Construction
public:
	CUDPNetGraphDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_UDPNETGRAPH_DIALOG };

	SOCKET m_socket;
	void MainLoop();
	void PacketProcess();
	void ParsePacket(char buff[128], const int buffLen);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	int AppendToLogAndScroll(CString str, COLORREF color);
	int GetNumVisibleLines(CRichEditCtrl* pCtrl);


// Implementation
protected:
	HICON m_hIcon;
	bool m_loop;
	bool m_isConnect;
	CGraphWindow *m_graphDlg;
	CFont m_font;


	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonConnect();
	long m_Port;
	afx_msg void OnBnClickedButtonShowgraph();
	afx_msg void OnBnClickedCheckPrintPacket();
	CRichEditCtrl m_LogEdit;
	BOOL m_IsPrintPacket;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	int m_RcvCount;
	CButton m_ServerStartButton;
	BOOL m_IsPrintMemory;
	afx_msg void OnBnClickedCheckPrintMemory();
	int m_MaxLine;
	afx_msg void OnEnChangeEditMaxLine();
};
