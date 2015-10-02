
// MainFrm.h : interface of the CMainFrame class
//

#pragma once
#include "Cube3DPane.h"


class CCube3DPane;
class CUDPInputView;
class CUDPParseView;
class CMotionOutputView;
class CJoystickView;
class CMotionWaveView;
class CMixingView;
class CControlBoard;
class CPlotView;
class CLauncherView;

class CMainFrame : public CFrameWndEx
{	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)


public:
	void Init();
	void UpdateConfig(bool IsSaveAndValidate = true);
	void LoadConfigFile(const string &fileName);
	BOOL NewPlotWindow();
	void SetContainerSize(CDockablePane* targetPane, UINT cx, UINT cy);


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


public:
	CMFCMenuBar       m_wndMenuBar;
	CMFCToolBarImages m_UserImages;

	list<CDockablePaneBase*> m_viewList;
	list<CDockablePaneChildView*> m_childViewList;

	C3DView *m_wndCube3DView;
	CUDPInputView *m_udpInputView;
	CUDPParseView *m_udpParseView;
	CMotionOutputView *m_motionOutputView;
	CJoystickView *m_joystickView;
	CMotionWaveView *m_motionWaveView;
	CMixingView *m_mixingView;
	CControlBoard *m_controlBoardView;
	CPlotView *m_plotView;
	CLauncherView *m_launcherView;


// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	DECLARE_MESSAGE_MAP()

	BOOL CreateDockingWindows();
	void SetDockingWindowIcons(BOOL bHiColorIcons);
public:
	afx_msg void OnViewViewinitialize();
	afx_msg void OnClose();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
