#pragma once


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


protected:
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
};
