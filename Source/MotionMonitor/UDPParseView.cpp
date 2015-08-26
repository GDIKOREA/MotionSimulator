// UDPParseView.cpp : implementation file
//

#include "stdafx.h"
#include "MotionMonitor.h"
#include "UDPParseView.h"
#include "afxdialogex.h"


// CUDPParseView dialog

CUDPParseView::CUDPParseView(CWnd* pParent /*=NULL*/)
	: CDockablePaneChildView(CUDPParseView::IDD, pParent)
{

}

CUDPParseView::~CUDPParseView()
{
}

void CUDPParseView::DoDataExchange(CDataExchange* pDX)
{
	CDockablePaneChildView::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CUDPParseView, CDockablePaneChildView)
	ON_BN_CLICKED(IDCANCEL, &CUDPParseView::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CUDPParseView::OnBnClickedOk)
END_MESSAGE_MAP()


// CUDPParseView message handlers


void CUDPParseView::OnBnClickedCancel()
{
}


void CUDPParseView::OnBnClickedOk()
{
}

