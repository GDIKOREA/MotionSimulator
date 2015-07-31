
// UDPAnalyzerView.cpp : implementation of the CUDPAnalyzerView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "UDPAnalyzer.h"
#endif

#include "UDPAnalyzerDoc.h"
#include "UDPAnalyzerView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CUDPAnalyzerView

IMPLEMENT_DYNCREATE(CUDPAnalyzerView, CView)

BEGIN_MESSAGE_MAP(CUDPAnalyzerView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CUDPAnalyzerView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CUDPAnalyzerView construction/destruction

CUDPAnalyzerView::CUDPAnalyzerView()
{
	// TODO: add construction code here

}

CUDPAnalyzerView::~CUDPAnalyzerView()
{
}

BOOL CUDPAnalyzerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CUDPAnalyzerView drawing

void CUDPAnalyzerView::OnDraw(CDC* /*pDC*/)
{
	CUDPAnalyzerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CUDPAnalyzerView printing


void CUDPAnalyzerView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CUDPAnalyzerView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CUDPAnalyzerView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CUDPAnalyzerView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CUDPAnalyzerView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CUDPAnalyzerView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CUDPAnalyzerView diagnostics

#ifdef _DEBUG
void CUDPAnalyzerView::AssertValid() const
{
	CView::AssertValid();
}

void CUDPAnalyzerView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CUDPAnalyzerDoc* CUDPAnalyzerView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CUDPAnalyzerDoc)));
	return (CUDPAnalyzerDoc*)m_pDocument;
}
#endif //_DEBUG


// CUDPAnalyzerView message handlers
