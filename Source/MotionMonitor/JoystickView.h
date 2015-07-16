#pragma once
#include "afxwin.h"
#include "afxcmn.h"


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


protected:
	CMultiPlotWindow *m_multiPlotWindows;
	float m_incTime;
	bool m_isStart;
	bool m_isMotionOutputStart;
	bool m_isRecord;
	cMotionWave m_recordMWave;


	
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
	afx_msg void OnBnClickedButtonMotionOutputStart();
	CButton m_MotionOutputStartButton;
	CSliderCtrl m_SliderYawOffset;
	CSliderCtrl m_SliderPitchOffset;
	CSliderCtrl m_SliderRollOffset;
	CSliderCtrl m_SliderHeaveOffset;
	CSliderCtrl m_SliderYawScale;
	CSliderCtrl m_SliderPitchScale;
	CSliderCtrl m_SliderRollScale;
	CSliderCtrl m_SliderHeaveScale;
	CString m_EditYawOffset;
	CString m_EditPitchOffset;
	CString m_EditRollOffset;
	CString m_EditHeaveOffset;
	CString m_EditYawScale;
	CString m_EditPitchScale;
	CString m_EditRollScale;
	CString m_EditHeaveScale;
	float m_YawOffset;
	float m_PitchOffset;
	float m_RollOffset;
	float m_HeaveOffset;
	float m_YawScale;
	float m_PitchScale;
	float m_RollScale;
	float m_HeaveScale;
	afx_msg void OnBnClickedButtonRecord();
	afx_msg void OnNMCustomdrawSliderYawOffset(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSliderPitchOffset(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSliderRollOffset(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSliderYaw(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSliderPitch(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSliderRoll(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSliderHeaveOffset(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSliderHeave(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeEditYawOffset();
	afx_msg void OnEnChangeEditPitchOffset();
	afx_msg void OnEnChangeEditRollOffset();
	afx_msg void OnEnChangeEditHeaveOffset();
	afx_msg void OnEnChangeEditYaw();
	afx_msg void OnEnChangeEditPitch();
	afx_msg void OnEnChangeEditRoll();
	afx_msg void OnEnChangeEditHeave();
	CButton m_RecordButton;
};
