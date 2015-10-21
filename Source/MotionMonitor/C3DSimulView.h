#pragma once


// C3DSimulView dialog

class C3DSimulView : public CDockablePaneChildView
{
public:
	C3DSimulView(CWnd* pParent = NULL);   // standard constructor
	virtual ~C3DSimulView();

// Dialog Data
	enum { IDD = IDD_DIALOG_SIMUL };

	virtual void Update(const float deltaSeconds) override;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
};
