#pragma once

#include "afxwin.h"
#include "afxcmn.h"
#include "ComPortCombo.h"


class CSerial2UDPDlg : public CDialogEx
{
// Construction
public:
	CSerial2UDPDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_MINIMFC_DIALOG };

	void MainLoop();


protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	void Process(const int deltaMilliseconds);
	void Log(const string &str);


// Implementation
protected:
	HICON m_hIcon;
	network::cUDPClient m_client;
	network::cUDPServer m_server;
 	common::CBufferedSerial m_serial;
	bool m_loop;
	bool m_isComConnect;
	int m_serialRxCnt;
	int m_udpRxCnt;
	int m_errCnt;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonStart();
	CListBox m_LogList;
	int m_ServerPort;
	CIPAddressCtrl m_ServerIP;
	CComPortCombo m_ComPortComboBox;
	CComboBox m_ComBaudrateComboBox;
	CButton m_StartButton;
	afx_msg void OnBnClickedButtonClear();
	CStatic m_SerialReceiveText;
	CStatic m_SerialReceiveCount;
	int m_RcvPort;
	CStatic m_UdpRxCount;
};
