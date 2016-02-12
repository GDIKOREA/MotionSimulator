#pragma once
#include "afxwin.h"
#include "afxcmn.h"


#include "sharedmemcommunication.h"


// CLauncherView dialog

class CLauncherView : public CDockablePaneChildView
{
public:
	CLauncherView(CWnd* pParent = NULL);   // standard constructor
	virtual ~CLauncherView();

// Dialog Data
	enum { IDD = IDD_DIALOG_LAUNCHER };
	
	bool Init();

	virtual void Update(const float deltaSeconds) override;
	void UpdateCoin(const int credit, const int coin, const int coinPerGame);
	void UpdateGameInfo(const int creditCount, const int gameCount);
	bool m_isGameExeTerminate;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	const float m_camSensitiveMid; // 카메라 감도 중간 값 = 2.3
	float m_camSens1;
	float m_camSens2;
	cExeTracker m_gameExeTracker;
	cExeTracker m_cameraExeTracker;
	cSharedMemoryCommunication m_shMem;

	Gdiplus::Bitmap m_titleImage;


	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonGamestart();
	afx_msg void OnBnClickedButtonPlayer1();
	afx_msg void OnBnClickedButtonPlayer2();
	afx_msg void OnBnClickedButtonCamAdjustmentP1();
	afx_msg void OnBnClickedButtonCamAdjustmentP2();
	//CComboBox m_DifficultCombo;
	CComboBox m_DifficultCombo2;
	CSliderCtrl m_CamSensSlider;
	float m_CamSens1Edit;
	float m_CamSens2Edit;
	afx_msg void OnNMCustomdrawSliderCamSens(NMHDR *pNMHDR, LRESULT *pResult);
	CSliderCtrl m_CamSens2Slider;
	afx_msg void OnNMCustomdrawSliderCamSens2(NMHDR *pNMHDR, LRESULT *pResult);
	int m_Credit;
	int m_Coin;
	int m_CoinPerGame;
	afx_msg void OnEnChangeEditCredit();
	afx_msg void OnEnChangeEditCoin();
	afx_msg void OnEnChangeEditCoinpergame();
	afx_msg void OnBnClickedButtonBoardcheck();
	CButton m_StartButton;
	int m_GameCount;
	int m_CreditCount;
	afx_msg void OnPaint();
	afx_msg void OnBnClickedButtonChangeAccount();
	afx_msg void OnBnClickedButtonHelp();
	CButton m_HelpButton;
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedButtonBoardactive();
};
