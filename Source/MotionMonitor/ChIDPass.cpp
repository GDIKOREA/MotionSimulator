// ChIDPass.cpp : implementation file
//

#include "stdafx.h"
#include "MotionMonitor.h"
#include "ChIDPass.h"
#include "afxdialogex.h"


// CChIDPass dialog

IMPLEMENT_DYNAMIC(CChIDPass, CDialogEx)

CChIDPass::CChIDPass(CWnd* pParent /*=NULL*/)
	: CDialogEx(CChIDPass::IDD, pParent)
	, m_NewId(_T(""))
	, m_NewPsswd(_T(""))
	, m_NewPasswdConfirm(_T(""))
{

}

CChIDPass::~CChIDPass()
{
}

void CChIDPass::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_NEWID, m_NewId);
	DDX_Text(pDX, IDC_EDIT_NEWPASS, m_NewPsswd);
	DDX_Text(pDX, IDC_EDIT_NEWPASS_CONFIRM, m_NewPasswdConfirm);
}


BEGIN_MESSAGE_MAP(CChIDPass, CDialogEx)
	ON_BN_CLICKED(IDCANCEL, &CChIDPass::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CChIDPass::OnBnClickedOk)
END_MESSAGE_MAP()


// CChIDPass message handlers

BOOL CChIDPass::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_NewId = str2wstr(cMotionController::Get()->m_loginId).c_str();	
	UpdateData(FALSE);
	return TRUE;
}


void CChIDPass::OnBnClickedCancel()
{	
	CDialogEx::OnCancel();
}


void CChIDPass::OnBnClickedOk()
{
	UpdateData();

	if (m_NewId.IsEmpty() ||
		m_NewPsswd.IsEmpty() ||
		m_NewPasswdConfirm.IsEmpty())
	{
		::AfxMessageBox(L"Please Input ID, Pass Word");
		return;
	}

	if (m_NewPasswdConfirm != m_NewPsswd)
	{
		::AfxMessageBox(L"Not Match Password String");
		return;
	}

	using namespace std;
	ofstream ofs("../media/machinegun/auth.dat");
	if (!ofs.is_open())
	{
		::AfxMessageBox(L"Error!! Save");
		return;
	}

	const string id = wstr2str((LPCTSTR)m_NewId);
	const string passwd = wstr2str((LPCTSTR)m_NewPsswd);

	ofs << id << endl;
	ofs << passwd << endl;

	::AfxMessageBox(L"Success Change ID / Pass Word");

	CDialogEx::OnOK();
}

