#pragma once
#include "afxwin.h"


// CPlotView dialog

class CPlotView : public CDockablePaneChildView
{
public:
	CPlotView(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPlotView();

// Dialog Data
	enum { IDD = IDD_DIALOG_PLOT };

	virtual void Update(const float deltaSeconds) override;
	void UpdateConfig();
	void SaveConfig();
	void SetAddPlotView(const  bool add);


protected:
	CMultiPlotWindow *m_multiPlotWindows;
	float m_incTime;
	bool m_isStart;
	bool m_addPlotView; // true 이면, 환경변수에 값을 저장하지 않는다. 환경변수는 처음에 생성된 PlotView만 저장된다.

	cPlotInputParser m_parser;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_ANCHOR_MAP();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonUpdate();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	CEdit m_PlotInputCommandEditor;
	CEdit m_PlotInputOut;
	CEdit m_PlotCommandEditor;
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedButtonNewplotwindow();
};


inline void CPlotView::SetAddPlotView(const  bool add) { m_addPlotView = add; }
