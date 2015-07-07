// X2GraphDialog.cpp : implementation file
//

#include "stdafx.h"
#include "MotionMonitor.h"
#include "X2GraphDialog.h"
#include "afxdialogex.h"


// CX2GraphDialog dialog

IMPLEMENT_DYNAMIC(CX2GraphDialog, CDialogEx)

CX2GraphDialog::CX2GraphDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CX2GraphDialog::IDD, pParent)
{

}

CX2GraphDialog::~CX2GraphDialog()
{
}

void CX2GraphDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CX2GraphDialog, CDialogEx)
	ON_BN_CLICKED(IDOK, &CX2GraphDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CX2GraphDialog::OnBnClickedCancel)
END_MESSAGE_MAP()


// CX2GraphDialog message handlers


void CX2GraphDialog::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
}


void CX2GraphDialog::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnCancel();
}
