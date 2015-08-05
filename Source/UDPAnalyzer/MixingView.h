#pragma once
#include "afxcmn.h"


// CMixingView dialog

class CMixingView : public CDockablePaneChildView
{
public:
	CMixingView(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMixingView();

// Dialog Data
	enum { IDD = IDD_DIALOG_MIXING };

	virtual void Update(const float deltaSeconds) override;


protected:
	cMathParser m_parser;
	mathscript::cMathInterpreter m_interpreter;
	float m_incTime;

	int m_symbolCount; // 이 값과 현재 심볼 개수와 다르다면, SymbolList를 업데이트 한다.
	map<string, int> m_symLookup;
	map<string, script::sFieldData> m_cloneSymbols;

protected:
	void InitSymbolList();
	void UpdateSymbolList();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_ANCHOR_MAP();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CEdit m_CommandEditor;
	afx_msg void OnBnClickedButtonUpdate();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	CListCtrl m_SymbolList;
	afx_msg void OnBnClickedCheckSymbol();
	BOOL m_IsUpdateSymbolList;
};
