#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CUDPSend dialog

class CUDPSendView : public CDockablePaneChildView
{
public:
	CUDPSendView(CWnd* pParent = NULL);   // standard constructor
	virtual ~CUDPSendView();

// Dialog Data
	enum { IDD = IDD_DIALOG_SENDUDP };

	void UpdateConfig();
	void SaveConfig();


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void SendString(const int editIndex, const string &str);


	network::cUDPClient m_udpClient;
	bool m_IsConnect;
	cPlotInputParser m_edit[10];

	
	DECLARE_ANCHOR_MAP();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CIPAddressCtrl m_IP;
	int m_Port;
	BOOL m_IsSyncUDPView;
	CButton m_ConnectButton;
	afx_msg void OnBnClickedButtonConnect();
	afx_msg void OnBnClickedCheckSyncUdpview();
	afx_msg void OnBnClickedSendButton(UINT);
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	BOOL m_IsConvertCR;
	afx_msg void OnBnClickedCheckCr();
};
