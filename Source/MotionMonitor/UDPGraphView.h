#pragma once
#include "afxwin.h"
#include "afxeditbrowsectrl.h"


// CUDPGraphView dialog
class CMultiPlotWindow;
class CUDPGraphView : public CDockablePaneChildView
{
public:
	CUDPGraphView(CWnd* pParent = NULL);   // standard constructor
	virtual ~CUDPGraphView();

// Dialog Data
	enum { IDD = IDD_DIALOG_UDP_GRAPH };

	// UpdateObserver Interface
	virtual void Update(const float deltaSeconds) override;
	virtual void UpdateUDP(const char *buffer, const int bufferLen) override;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	bool WriteConfigFile(const string &fileName);
	void ReadHardwareSettingConfigFile(const string &fileName);


protected:
// 	SOCKET m_socket;
// 	bool m_isServerBind;
	bool m_isPause;
	CMultiPlotWindow *m_multiPlotWindows;
	string m_xmlFilePath;


	DECLARE_ANCHOR_MAP();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonHelp();
	afx_msg void OnBnClickedButtonUpdate();
	CEdit m_CommandEditor;
	afx_msg void OnBnClickedButtonUdpServerBind();
	int m_ServerPort;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	CButton m_UpdateButton;
	CButton m_HelpButton;
	afx_msg void OnBnClickedCheckFixedmode();
	CButton m_FixedModeButton;
	CButton m_ServerBindButton;
	CString m_PacketString;
	afx_msg void OnBnClickedButtonPause();
	CMFCEditBrowseCtrl m_fileBrowser;
	afx_msg void OnEnChangeMfceditbrowseXmlpath();
	afx_msg void OnDestroy();
};
