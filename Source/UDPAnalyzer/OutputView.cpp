// OutputView.cpp : implementation file
//

#include "stdafx.h"
#include "UDPAnalyzer.h"
#include "OutputView.h"
#include "afxdialogex.h"


// COutputView dialog
COutputView::COutputView(CWnd* pParent /*=NULL*/)
	: CDockablePaneChildView(COutputView::IDD, pParent)
{

}

COutputView::~COutputView()
{
}

void COutputView::DoDataExchange(CDataExchange* pDX)
{
	CDockablePaneChildView::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(COutputView, CDockablePaneChildView)
	ON_BN_CLICKED(IDOK, &COutputView::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &COutputView::OnBnClickedCancel)
END_MESSAGE_MAP()


// COutputView message handlers


void COutputView::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//CDockablePaneChildView::OnOK();
}


void COutputView::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	//CDockablePaneChildView::OnCancel();
}
