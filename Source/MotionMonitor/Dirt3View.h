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

	virtual void UpdateConfig(bool IsSaveAndValidate = true) override;
	virtual void Update(const float deltaSeconds) override;
	virtual void UpdateUDP(const char *buffer, const int bufferLen) override;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	enum STATE {STOP,START,};
	enum {MAX_SLIDER_LIMIT=10000};

	STATE m_state;
	Gdiplus::Bitmap m_titleImage;
	int m_controllerMainState;
	int m_controllerState;
	int m_controllerSubState;
	int m_displayPlayTime;
	bool m_enChange1;
	bool m_enChange2;
	bool m_enChange3;


	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonStart();
	CButton m_StartButton;
	BOOL m_IsMotionSim;
	afx_msg void OnBnClickedCheckMotionSim();
	afx_msg void OnPaint();
	CStatic m_State0Text;
	CStatic m_State1Text;
	CStatic m_State2Text;
	CStatic m_PlayTimeText;
	afx_msg void OnBnClickedButtonEmergency();
	afx_msg void OnBnClickedRadio4axis();
	afx_msg void OnBnClickedRadio3axis();
	CSliderCtrl m_sliderActuatorRange;
	CSliderCtrl m_sliderActuatorYawRange;
	CSliderCtrl m_sliderActuatorSpeed;
	afx_msg void OnNMCustomdrawSliderActionRange(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL OnInitDialog();
	CEdit m_editActuatorRange;
	CEdit m_editActuatorYawRange;
	CEdit m_editActuatorSpeed;
	CEdit m_editPlayTime;
	int m_radioAxisType;
	afx_msg void OnNMCustomdrawSliderActionSpeed(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeEditActuator();
	afx_msg void OnEnChangeEditActuatorSpeed();
	afx_msg void OnEnChangeEditPlaytime();
	afx_msg void OnNMCustomdrawSliderYawRange(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeEditActuatorYaw();
};
