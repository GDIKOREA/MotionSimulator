#pragma once
#include "afxwin.h"
#include "protocolparser.h"
#include "mathinterpreter.h"
#include "mathparser.h"


// CVarModulationView dialog

class CVarModulationView : public CDockablePaneChildView
{
public:
	CVarModulationView(CWnd* pParent = NULL);   // standard constructor
	virtual ~CVarModulationView();

// Dialog Data
	enum { IDD = IDD_DIALOG_MODULATION };

	virtual void UpdateConfig(bool IsSaveAndValidate = true) override;
	virtual void Update(const float deltaSeconds) override;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	cMathParser m_inputVarParser;
	cMathParser m_ouputVarParser;


	DECLARE_MESSAGE_MAP()
	DECLARE_ANCHOR_MAP();
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CEdit m_editInputVar;
	CEdit m_editOutputVar;
	CEdit m_editScript;
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
