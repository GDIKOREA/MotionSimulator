
// MotionMonitorView.cpp : implementation of the CMotionMonitorView class
//

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


// CMotionMonitorView

IMPLEMENT_DYNCREATE(CMotionMonitorView, CView)

BEGIN_MESSAGE_MAP(CMotionMonitorView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMotionMonitorView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CMotionMonitorView construction/destruction

CMotionMonitorView::CMotionMonitorView()
{
	// TODO: add construction code here

}

CMotionMonitorView::~CMotionMonitorView()
{
}

BOOL CMotionMonitorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CMotionMonitorView drawing

void CMotionMonitorView::OnDraw(CDC* /*pDC*/)
{
	CMotionMonitorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CMotionMonitorView printing


void CMotionMonitorView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CMotionMonitorView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CMotionMonitorView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CMotionMonitorView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CMotionMonitorView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CMotionMonitorView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


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
