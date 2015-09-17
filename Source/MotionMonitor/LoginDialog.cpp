// LoginDialog.cpp : implementation file
//

#include "stdafx.h"
#include "MotionMonitor.h"
#include "LoginDialog.h"
#include "afxdialogex.h"


// CLoginDialog dialog
CLoginDialog::CLoginDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLoginDialog::IDD, pParent)
	, m_ID(_T(""))
	, m_Password(_T(""))
{

}

CLoginDialog::~CLoginDialog()
{
}

void CLoginDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_ID, m_ID);
	DDX_Text(pDX, IDC_EDIT_PASS, m_Password);
}


BEGIN_MESSAGE_MAP(CLoginDialog, CDialogEx)
	ON_BN_CLICKED(IDOK, &CLoginDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CLoginDialog::OnBnClickedCancel)
END_MESSAGE_MAP()


// CLoginDialog message handlers


void CLoginDialog::OnBnClickedOk()
{
	UpdateData();

	int errorCode = 0;
	if (!CheckID(wstr2str((LPCTSTR)m_ID), wstr2str((LPCTSTR)m_Password), errorCode))
	{
		switch (errorCode)
		{
		case 1: ::AfxMessageBox(L"Not registered admin name. "); break;
		case 2: ::AfxMessageBox(L"Please inquire to company if you lost password."); break;
		case 3: ::AfxMessageBox(L"Please inquire to company if you lost data."); break;
		}
		return;
	}
	
	CDialogEx::OnOK();
}


void CLoginDialog::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnCancel();
}


// 아이디, 패스워드 검사
// auth.txt 
// errorCode = 0 : success
//			   1 : fail id
//			   2 : fail passwd
//			   3 : auth db error
bool CLoginDialog::CheckID(const string &id, const string &passwd, int &errorCode)
{
	using namespace std;
	ifstream ifs("../media/machinegun/auth.dat");
	if (!ifs.is_open())
	{
		errorCode = 3;
		return false;
	}

	while (!ifs.eof())
	{
		string str1, str2;
		ifs >> str1;
		ifs >> str2;

		if (str1.empty() || str2.empty())
		{
			errorCode = 1; // eof
			return false;
		}

		if (id == str1)
		{
			if (passwd == str2)
			{
				errorCode = 0;
				return true;
			}
			else
			{
				// passwd error
				errorCode = 2;
				return false;
			}
		}
	}	

	errorCode = 1; // not found id
	return false;
}
