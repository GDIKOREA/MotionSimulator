#pragma once

#include "BPCtrlAnchorMap.h"


// CGraphWindow dialog
class CPlotWindow;
class CMultiPlotWindow;
class CGraphWindow : public CDialogEx
{
//	DECLARE_DYNAMIC(CGraphWindow)

public:
	CGraphWindow(CWnd* pParent = NULL);   // standard constructor
	virtual ~CGraphWindow();

// Dialog Data
	enum { IDD = IDD_DIALOG_GRAPH };


	void Update(const float deltaSeconds);
	void SetGraphValue(const string &str);
	void SetGraphValue2(float v);

	CMultiPlotWindow *m_multiPlotWindows;
	float m_incSeconds;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_ANCHOR_MAP();
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedButtonUpdate();
	CString m_PlotCommand;
	afx_msg void OnBnClickedCheckFixedmode();
	BOOL m_FixedMode;
	afx_msg void OnBnClickedButtonHelp();
};
