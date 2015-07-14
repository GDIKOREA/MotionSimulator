#pragma once
#include "afxwin.h"


// CJoystickView dialog

class CJoystickView : public CDockablePaneChildView
{
public:
	CJoystickView(CWnd* pParent = NULL);   // standard constructor
	virtual ~CJoystickView();

// Dialog Data
	enum { IDD = IDD_DIALOG_JOYSTICK };

	virtual void Update(const float deltaSeconds) override;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	bool InitJoyStick();
	void ParseRawInput(PRAWINPUT pRawInput);


	CMultiPlotWindow *m_multiPlotWindows;
	float m_incTime;
	bool m_isStart;

	// JoyStick variable
	enum { MAX_BUTTONS=128 };

	BOOL m_bButtonStates[MAX_BUTTONS];
	LONG m_AxisX;
	LONG m_AxisY;
	LONG m_AxisZ;
	LONG m_AxisRz;
	LONG m_AxisH;
	LONG m_Hat;
	INT  m_NumberOfButtons;

	DECLARE_ANCHOR_MAP();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedButtonStart();
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	CButton m_StartButton;
};
