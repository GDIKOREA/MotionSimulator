#pragma once


// CControlBoard dialog

class CControlBoard : public CDockablePaneChildView
{
public:
	CControlBoard(CWnd* pParent = NULL);   // standard constructor
	virtual ~CControlBoard();

// Dialog Data
	enum { IDD = IDD_DIALOG_CONTROL };

	virtual void Update(const float deltaSeconds) override;
	virtual void UpdateUDP(const char *buffer, const int bufferLen) override;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support


	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedRadioDirt3();
	afx_msg void OnBnClickedRadioMg();
	int m_GameType;
	CString m_GameState;
	CString m_GameTime;
	CString m_GameMission;
	BOOL m_IsMotionSimulation;
	afx_msg void OnBnClickedCheckMotionSimulation();
	afx_msg void OnBnClickedButtonEmergency();
	CString m_UDPUpdateTime;
	CString m_InnerState;
};
