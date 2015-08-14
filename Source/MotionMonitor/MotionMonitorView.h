#pragma once

class CMotionMonitorView : public CView
{
protected:
	CMotionMonitorView();
	DECLARE_DYNCREATE(CMotionMonitorView)

public:
	CMotionMonitorDoc* GetDocument() const;
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
	virtual ~CMotionMonitorView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in MotionMonitorView.cpp
inline CMotionMonitorDoc* CMotionMonitorView::GetDocument() const
   { return reinterpret_cast<CMotionMonitorDoc*>(m_pDocument); }
#endif

