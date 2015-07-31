// PlotView.cpp : implementation file
//

#include "stdafx.h"
#include "UDPAnalyzer.h"
#include "PlotView.h"
#include "afxdialogex.h"


// CPlotView dialog

CPlotView::CPlotView(CWnd* pParent /*=NULL*/)
	: CDockablePaneChildView(CPlotView::IDD, pParent)
{

}

CPlotView::~CPlotView()
{
}

void CPlotView::DoDataExchange(CDataExchange* pDX)
{
	CDockablePaneChildView::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPlotView, CDockablePaneChildView)
	ON_BN_CLICKED(IDOK, &CPlotView::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CPlotView::OnBnClickedCancel)
END_MESSAGE_MAP()


// CPlotView message handlers


void CPlotView::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//CDockablePaneChildView::OnOK();
}


void CPlotView::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	//CDockablePaneChildView::OnCancel();
}
