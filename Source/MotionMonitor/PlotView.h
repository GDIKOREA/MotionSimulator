#pragma once
#include "afxwin.h"

#include "plotinputparser.h"

// CPlotView dialog

class CPlotView : public CDockablePaneChildView
{
public:
	CPlotView(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPlotView();

// Dialog Data
	enum { IDD = IDD_DIALOG_PLOT };

	virtual void UpdateConfig(bool IsSaveAndValidate = true) override;
	void SetAddPlotView(const bool add);

	virtual void Update(const float deltaSeconds) override;


protected:
	CMultiPlotWindow *m_multiPlotWindows;
	float m_incTime;
	bool m_isStart;
	bool m_addPlotView; // true �̸�, ȯ�溯���� ���� �������� �ʴ´�. ȯ�溯���� ó���� ������ PlotView�� ����ȴ�.

	cPlotInputParser m_parser;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_ANCHOR_MAP();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CEdit m_PlotInputCommandEditor;
	CEdit m_PlotInputOut;
	CEdit m_PlotCommandEditor;
	afx_msg void OnBnClickedButtonUpdate();
	afx_msg void OnBnClickedButtonNewplot();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
};


inline void CPlotView::SetAddPlotView(const  bool add) { m_addPlotView = add; }
