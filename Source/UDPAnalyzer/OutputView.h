#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// COutputView dialog

class COutputView : public CDockablePaneChildView
{
public:
	COutputView(CWnd* pParent = NULL);   // standard constructor
	virtual ~COutputView();

// Dialog Data
	enum { IDD = IDD_DIALOG_OUTPUT };

	virtual void Update(const float deltaSeconds) override;


protected:
	CMultiPlotWindow *m_multiPlotWindows;
	float m_incTime;
	bool m_isStart;
	float m_incSerialTime;
	float m_incUDPTime;

	cPlotInputParser m_sendFormatParser;

	cMathParser m_rollParser;
	cMathParser m_pitchParser;
	cMathParser m_yawParser;
	mathscript::cMathInterpreter m_interpreter;

	network::cUDPClient m_udpSendClient;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	string GetSendIP();


	DECLARE_ANCHOR_MAP();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedRadioSerial();
	afx_msg void OnBnClickedRadioUdp();
	afx_msg void OnBnClickedButtonConnect();
	int m_UDPPort;
	CIPAddressCtrl m_UDPIP;
	CComPortCombo m_ComPort;
	CComboBox m_BaudRateCombobox;
	CString m_RollCommand;
	CString m_PitchCommand;
	CString m_YawCommand;
	afx_msg void OnBnClickedButton3dupdate();
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CEdit m_PlotCommand;
	int m_SendType;
	CString m_SendFormat;
	afx_msg void OnBnClickedButtonPlotUpdate();
	afx_msg void OnBnClickedButtonUpdateSendformat();
	CStatic m_SendString;
	afx_msg void OnBnClickedRadioNone();
	CButton m_ConnectButton;
};
