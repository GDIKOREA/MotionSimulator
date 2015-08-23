#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CUDPInputView dialog

class CUDPInputView : public CDockablePaneChildView
{
public:
	CUDPInputView(CWnd* pParent = NULL);   // standard constructor
	virtual ~CUDPInputView();

// Dialog Data
	enum { IDD = IDD_DIALOG_MOTION_INPUT };

	virtual void UpdateConfig(bool IsSaveAndValidate = true) override;
	virtual void Start() override;
	virtual void Stop() override;

	virtual void Update(const float deltaSeconds) override;
	virtual void UpdateUDP(const char *buffer, const int bufferLen) override;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	enum STATE {
		STOP,
		START,
	};

	STATE m_state;
	bool m_isPause;
	CMultiPlotWindow *m_multiPlotWindows;
	float m_incTime;
	float m_updateIncTime;


	DECLARE_ANCHOR_MAP();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonUpdate();
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CButton m_UpdateButton;
	CRichEditCtrl m_EditCommand;
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedButtonUdpServerBind();
	CButton m_ServerBindButton;
	int m_ServerPort;
	CString m_PacketString;
	afx_msg void OnBnClickedButtonPlotupdate();
	afx_msg void OnBnClickedButtonHelp();
	afx_msg void OnBnClickedButtonPlotpause();
	afx_msg void OnBnClickedCheckFixedmode();
	CEdit m_PlotCommandEditor;
	CButton m_FixedModeButton;
	int m_PacketRecvCount;
	CString m_PlotInputString;
};
