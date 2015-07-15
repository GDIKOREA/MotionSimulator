#pragma once
#include "afxcmn.h"
#include "motionwave.h"
#include "afxwin.h"

// CMotionWaveView dialog

class CMotionWaveView : public CDockablePaneChildView
{

public:
	CMotionWaveView(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMotionWaveView();

// Dialog Data
	enum { IDD = IDD_DIALOG_MWAVE };

	virtual void Update(const float deltaSeconds) override;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support


protected:
	bool m_isPlay;
	CMultiPlotWindow *m_multiPlotWindows;
	float m_incTime;
	cMotionWave m_mwaveFile;


	DECLARE_ANCHOR_MAP();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CFileTreeCtrl m_FileTree;
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSelchangedTreeFile(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonPlay();
	CButton m_PlayButton;
	afx_msg void OnBnClickedButtonClear();
};
