
// MainFrm.h : interface of the CMainFrame class
//

#pragma once
#include "Cube3DPane.h"


class CMainFrame : public CFrameWndEx
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)


public:
	void UpdateConfig(bool IsSaveAndValidate = true);


// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
 	CMFCMenuBar       m_wndMenuBar;
	CMFCToolBarImages m_UserImages;

	list<CDockablePaneBase*> m_viewList;
	CCube3DPane *m_wndCube3DView;
	CDockablePaneBase *m_motionInputView;
	CDockablePaneBase *m_motionOutputView;
	CDockablePaneBase *m_joystickView;
	CDockablePaneBase *m_motionWaveView;
	CDockablePaneBase *m_mixingView;
	CDockablePaneBase *m_controlBoardView;


// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	DECLARE_MESSAGE_MAP()

	BOOL CreateDockingWindows();
	void SetDockingWindowIcons(BOOL bHiColorIcons);
public:
	afx_msg void OnViewViewinitialize();
};
