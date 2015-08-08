#pragma once
#include "afxwin.h"
#include "protocolparser.h"
#include "script.h"



// CUDPView dialog
class cMemDumpWindow;

class CUDPView : public CDockablePaneChildView
{
public:
	CUDPView(CWnd* pParent = NULL);   // standard constructor
	virtual ~CUDPView();

// Dialog Data
	enum { IDD = IDD_DIALOG_UDP };

	virtual void UpdateUDP(const char *buffer, const int bufferLen) override;
	void UpdateConfig();
	void SaveConfig();


protected:
	cProtocolParser m_protocolParser;
	cMemDumpWindow *m_dumpWindow;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_ANCHOR_MAP();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonStart();
	int m_Port;
	CButton m_StartButton;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	CEdit m_ProtocolEditor;
	afx_msg void OnBnClickedButtonUpdate();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedCheckDump();
	BOOL m_IsDump;
	BOOL m_IsASCII;
	afx_msg void OnBnClickedCheckAscii();
	afx_msg void OnBnClickedCheckSymbol();
	BOOL m_IsDisplaySymbol;
};
