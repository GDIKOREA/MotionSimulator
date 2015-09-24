
// MainFrm.h : interface of the CMainFrame class
//

#pragma once


class CUDPView;
class CUDPSendView;
class CPlotView;
class CMixingView;
class COutputView;
class CUDPPlayerView;

class CMainFrame : public CFrameWndEx
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:
	list<CDockablePaneBase*> m_viewList;
	CUDPView *m_udpView;
	CPlotView *m_plotView;
	CMixingView *m_mixingView;
	COutputView *m_outputView;
	C3DView *m_dxView;
	CUDPPlayerView *m_udpPlayerView;
	CUDPSendView *m_udpSendView;


// Operations
public:
	BOOL NewPlotWindow();


// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);
	void UpdateViewConfig();
	void SaveViewConfig(const string fileName);


// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CMFCMenuBar       m_wndMenuBar;

// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()

	BOOL CreateDockingWindows();
public:
	afx_msg void OnDestroy();
	afx_msg void OnFileOpen();
	afx_msg void OnFileSaveAs();
	afx_msg void OnFileNew();
	afx_msg void OnUpdateFileSaveas(CCmdUI *pCmdUI);
	afx_msg void OnViewInitdockingwindows();
};


