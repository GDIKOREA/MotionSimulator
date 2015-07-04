#pragma once

#include "afxwin.h"
#include "afxcmn.h"


// CUDPEditorView dialog

class CUDPEditorView : public CDockablePaneChildView
{
public:
	CUDPEditorView(CWnd* pParent = NULL);   // standard constructor
	virtual ~CUDPEditorView();

// Dialog Data
	enum { IDD = IDD_DIALOG_UDP_EDITOR };

	// UpdateObserver Interface
	virtual void Update(const float deltaSeconds) override;
	virtual void UpdateUDP(const char *buffer, const int bufferLen) override;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
// 	int GetNumVisibleLines(CRichEditCtrl* pCtrl);
// 	int AppendToLogAndScroll(CString str, COLORREF color);


	DECLARE_MESSAGE_MAP()
	DECLARE_ANCHOR_MAP();
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonUdpServerBind();
	CButton m_ServerStartButton;
	int m_ServerPort;
	CRichEditCtrl m_LogEditor;
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	int m_PacketRcvCount;
	afx_msg void OnBnClickedButtonClear();
	afx_msg void OnBnClickedCheckPrintPacket();
	BOOL m_IsPrintPacket;
};
