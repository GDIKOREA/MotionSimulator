
#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "MotionMonitor.h"
#endif
#include "MotionMonitorDoc.h"
#include "MotionMonitorView.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
IMPLEMENT_DYNCREATE(CMotionMonitorView, CView)
BEGIN_MESSAGE_MAP(CMotionMonitorView, CView)
END_MESSAGE_MAP()

CMotionMonitorView::CMotionMonitorView()
{
}
CMotionMonitorView::~CMotionMonitorView()
{
}
BOOL CMotionMonitorView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CView::PreCreateWindow(cs);
}

// CMotionMonitorView drawing
void CMotionMonitorView::OnDraw(CDC* /*pDC*/)
{
	CMotionMonitorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
}

// 
// void CMotionMonitorView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
// {
// #ifndef SHARED_HANDLERS
// 	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
// #endif
// }
// CMotionMonitorView diagnostics
#ifdef _DEBUG
void CMotionMonitorView::AssertValid() const
{
	CView::AssertValid();
}
void CMotionMonitorView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
CMotionMonitorDoc* CMotionMonitorView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMotionMonitorDoc)));
	return (CMotionMonitorDoc*)m_pDocument;
}
#endif //_DEBUG

// CMotionMonitorView message handlers
