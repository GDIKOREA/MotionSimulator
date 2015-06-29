// SerialGraphForm.cpp : implementation file
//

#include "stdafx.h"
#include "MotionMonitor.h"
#include "SerialGraphForm.h"
#include "afxdialogex.h"



// CSerialGraphForm dialog
CSerialGraphForm::CSerialGraphForm(CWnd* pParent /*=NULL*/)
: CDockablePaneChildView(CSerialGraphForm::IDD, pParent)
, m_multiPlotWindows(NULL)
{
}

CSerialGraphForm::~CSerialGraphForm()
{
}

void CSerialGraphForm::DoDataExchange(CDataExchange* pDX)
{
	CDockablePaneChildView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_COMMAND, m_CommandEditor);
	DDX_Control(pDX, IDC_BUTTON_UPDATE, m_UpdateButton);
	DDX_Control(pDX, IDC_CHECK_FIXEDMODE, m_FixedModeButton);
	DDX_Control(pDX, IDC_STATIC1, m_Static1);
	DDX_Control(pDX, IDC_BUTTON_HELP, m_HelpButton);
}

BEGIN_ANCHOR_MAP(CSerialGraphForm)
	ANCHOR_MAP_ENTRY(IDC_EDIT_COMMAND, ANF_LEFT | ANF_RIGHT | ANF_BOTTOM)
	ANCHOR_MAP_ENTRY(IDC_BUTTON_UPDATE, ANF_RIGHT | ANF_BOTTOM)
	ANCHOR_MAP_ENTRY(IDC_BUTTON_HELP, ANF_RIGHT | ANF_BOTTOM)
	ANCHOR_MAP_ENTRY(IDC_CHECK_FIXEDMODE, ANF_LEFT | ANF_BOTTOM)
	ANCHOR_MAP_ENTRY(IDC_STATIC1, ANF_LEFT | ANF_BOTTOM)
END_ANCHOR_MAP()


BEGIN_MESSAGE_MAP(CSerialGraphForm, CDockablePaneChildView)
	ON_BN_CLICKED(IDOK, &CSerialGraphForm::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CSerialGraphForm::OnBnClickedCancel)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_UPDATE, &CSerialGraphForm::OnBnClickedButtonUpdate)
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_CHECK_FIXEDMODE, &CSerialGraphForm::OnBnClickedCheckFixedmode)
	ON_BN_CLICKED(IDC_BUTTON_HELP, &CSerialGraphForm::OnBnClickedButtonHelp)
END_MESSAGE_MAP()


// CSerialGraphForm message handlers


void CSerialGraphForm::OnBnClickedOk()
{
}


void CSerialGraphForm::OnBnClickedCancel()
{
}


BOOL CSerialGraphForm::OnInitDialog()
{
	CDockablePaneChildView::OnInitDialog();

	InitAnchors();

	m_CommandEditor.SetWindowTextW(
		//		L"plot1 = 0,0,0,0,0\n\rstring1 = %*s %f, %f, %*f");
L"plot1 = 0, 0, 0, 0, 0\r\n\
string1 = %*s %f, %f, %*f\r\n\
plot2 = 0, 0, 0, 0, 0\r\n\
string2 = %*s %f, %*f, %f\r\n\
plot3 = 0, 0, 0, 0, 0\r\n\
string3 = %*s %f, %*f, %*f, %f\r\n");


	CRect rect;
	GetClientRect(rect);

	m_multiPlotWindows = new CMultiPlotWindow();
	BOOL result = m_multiPlotWindows->Create(NULL, NULL, WS_VISIBLE | WS_CHILD,
		CRect(0, 0, rect.Width(), 400), this, AFX_IDW_PANE_FIRST);

	m_multiPlotWindows->SetScrollSizes(MM_TEXT, CSize(rect.Width() - 30, 100));
	m_multiPlotWindows->ShowWindow(SW_SHOW);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CSerialGraphForm::OnSize(UINT nType, int cx, int cy)
{
	CDockablePaneChildView::OnSize(nType, cx, cy);

	CRect rcWnd;
	GetWindowRect(&rcWnd);
	HandleAnchors(&rcWnd);


	if (m_multiPlotWindows && m_multiPlotWindows->GetSafeHwnd())
	{
		CRect hwr;
		m_HelpButton.GetWindowRect(hwr);
		ScreenToClient(hwr);
		m_multiPlotWindows->MoveWindow(CRect(0, 0, cx, hwr.top));
	}
}


void CSerialGraphForm::OnBnClickedButtonUpdate()
{
	CString str;
	m_CommandEditor.GetWindowText(str);

	m_multiPlotWindows->ProcessPlotCommand(str);
	 
	// 업데이트 될 때마다, Plot 명령어를 저장한다.
	//g_MainDlg->SaveConfigFile();
}


// 시리얼 정보가 넘어올 때, 호출된다.
void CSerialGraphForm::UpdateSerial(char *buffer)
{
	if (m_multiPlotWindows)
		m_multiPlotWindows->SetString(buffer);
}


// 매프레임 마다 호출된다.
void CSerialGraphForm::Update(const float deltaSeconds)
{
	if (m_multiPlotWindows)
		m_multiPlotWindows->DrawGraph(deltaSeconds);
}


BOOL CSerialGraphForm::OnEraseBkgnd(CDC* pDC)
{
	// 백그라운드는 갱신하지 않는다. (속도를 빠르게하기위해)
	return CDockablePaneChildView::OnEraseBkgnd(pDC);
	//return TRUE;
}


void CSerialGraphForm::OnBnClickedCheckFixedmode()
{
	if (m_multiPlotWindows)
		m_multiPlotWindows->SetFixedWidthMode(m_FixedModeButton.GetCheck() > 0);
}


void CSerialGraphForm::OnBnClickedButtonHelp()
{
 	AfxMessageBox(
 L"\n\
plot = x-range, y-range, x-visible-range, y-visible-range, option \n\
	- x-range: 0 - auto \n\
	- y-range : 0 - auto \n\
	- x-visible - range : 0 - auto \n\
	- y-visible - range : 0 - auto \n\
	- option : \n\
	 	- x, y scroll \n\
	 	- line weight\n\
\n\
string = sscanf 의 인자로 들어갈 파싱 포맷\n\
\n\
예제)\n\
	- Serial Data\n\
		- Start, 1234, 1.11, 2.34, 3.33, 4.44\n\
\n\
	- Plot Command Script\n\
		plot1 = 0, 0, 0, 0, 0\n\
		string1 = %*s %f, %f, %*f, %*f, %*f\n\
\n\
		plot2 = 0, 0, 0, 0, 0\n\
		string2 = %*s %f, %*f, %f, %*f, %*f\n\
");
}
