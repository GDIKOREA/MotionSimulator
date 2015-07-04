
// UDP2UDPDlg.h : header file
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CUDP2UDPDlg dialog
class CUDP2UDPDlg : public CDialogEx
{
// Construction
public:
	CUDP2UDPDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_UDP2UDP_DIALOG };

	void MainLoop();


protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	void PacketProcess();
	void Log(const string &str);


// Implementation
protected:
	HICON m_hIcon;
	bool m_loop;
	SOCKET m_rcvSocket;
	SOCKET m_sndSocket;
	SOCKADDR_IN m_sockSndAddr;
	bool m_isConnect;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonStart();
	int m_RcvPort;
	CIPAddressCtrl m_SndIP;
	int m_SndPort;
	CButton m_StartButton;
	int m_RcvCount;
	CListBox m_LogList;
};
