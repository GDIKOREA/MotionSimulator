#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CProxyWindow dialog

class CProxyWindow : public CDialogEx
{
public:
	CProxyWindow(CWnd* pParent = NULL);   // standard constructor
	virtual ~CProxyWindow();

// Dialog Data
	enum { IDD = IDD_DIALOG_PROXY };

	void Update();
	void Init(const int rcvPort, const string &sendIP, const int sendPort);
	void HideRemoveButton();
	string GetSendIP();


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	network::cUDPServer m_udpReceiveServer;
	network::cUDPClient m_udpSendClient;


	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonStart();
	CButton m_StartButton;
	int m_RcvPort;
	int m_SndPort;
	int m_ReceiveCount;
	virtual BOOL OnInitDialog();
	CIPAddressCtrl m_SndIP;
	bool m_isStart;
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonRemove();
	CButton m_RemoveButton;
	CEdit m_EditReceivePort;
	CEdit m_EditSendPort;
};
