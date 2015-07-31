#pragma once
#include "afxwin.h"
#include "protocolparser.h"


struct sFieldData
{
	union 
	{
		bool bVal;
		int iVal;
		u_int uVal;
		float fVal;
		double dVal;
		char buff[8];
	};

	FEILD_TYPE::TYPE type;
};


// CUDPView dialog
class CUDPView : public CDockablePaneChildView
{
public:
	CUDPView(CWnd* pParent = NULL);   // standard constructor
	virtual ~CUDPView();

// Dialog Data
	enum { IDD = IDD_DIALOG_UDP };

	virtual void UpdateUDP(const char *buffer, const int bufferLen) override;


protected:
	cProtocolParser m_protocolParser;
	vector<sFieldData> m_fieldsData;


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
};
