#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CDirt3View dialog

class CDirt3View : public CDockablePaneChildView
{
public:
	CDirt3View(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDirt3View();

// Dialog Data
	enum { IDD = IDD_DIALOG_DIRT3 };

	virtual void Update(const float deltaSeconds) override;
	virtual void UpdateUDP(const char *buffer, const int bufferLen) override;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	enum STATE {STOP,START,};
	enum {MAX_ACTUATOR_LIMIT=10000};

	STATE m_state;
	Gdiplus::Bitmap m_titleImage;
	int m_controllerState;
	int m_controllerSubState;


	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonStart();
	CButton m_StartButton;
	BOOL m_IsMotionSim;
	afx_msg void OnBnClickedCheckMotionSim();
	afx_msg void OnPaint();
	CStatic m_State1Text;
	CStatic m_State2Text;
	afx_msg void OnBnClickedButtonEmergency();
	afx_msg void OnBnClickedRadio4axis();
	afx_msg void OnBnClickedRadio3axis();
	CSliderCtrl m_actuatorLengthRange;
	afx_msg void OnNMCustomdrawSliderActionRange(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL OnInitDialog();
	CEdit m_editActuatorRange;
	int m_radioAxisType;
};
