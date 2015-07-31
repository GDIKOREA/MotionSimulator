// DXView.cpp : implementation file
//

#include "stdafx.h"
#include "UDPAnalyzer.h"
#include "DXView.h"
#include "afxdialogex.h"


// CDXView dialog
CDXView::CDXView(CWnd* pParent /*=NULL*/)
	: CDockablePaneChildView(CDXView::IDD, pParent)
{

}

CDXView::~CDXView()
{
}

void CDXView::DoDataExchange(CDataExchange* pDX)
{
	CDockablePaneChildView::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDXView, CDockablePaneChildView)
	ON_BN_CLICKED(IDOK, &CDXView::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDXView::OnBnClickedCancel)
END_MESSAGE_MAP()


// CDXView message handlers


void CDXView::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
}


void CDXView::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
}
