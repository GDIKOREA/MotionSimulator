// GraphWindow.cpp : implementation file
//

#include "stdafx.h"
#include "UDPNetGraph.h"
#include "GraphWindow.h"
#include "afxdialogex.h"


// CGraphWindow dialog

//IMPLEMENT_DYNAMIC(CGraphWindow, CDialogEx)

CGraphWindow::CGraphWindow(CWnd* pParent /*=NULL*/)
	: CDialogEx(CGraphWindow::IDD, pParent)
	, m_multiPlotWindows(NULL)
	, m_PlotCommand(_T(""))
	, m_FixedMode(FALSE)
{
	m_incSeconds = 0;
}

CGraphWindow::~CGraphWindow()
{
}

void CGraphWindow::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_COMMAND, m_PlotCommand);
	DDX_Check(pDX, IDC_CHECK_FIXEDMODE, m_FixedMode);
}


BEGIN_ANCHOR_MAP(CGraphWindow)
	ANCHOR_MAP_ENTRY(IDC_STATIC_GRAPH, ANF_LEFT | ANF_RIGHT | ANF_BOTTOM | ANF_TOP)
	ANCHOR_MAP_ENTRY(IDC_EDIT_COMMAND, ANF_LEFT | ANF_RIGHT | ANF_BOTTOM)
	ANCHOR_MAP_ENTRY(IDC_BUTTON_UPDATE, ANF_RIGHT | ANF_BOTTOM)
	ANCHOR_MAP_ENTRY(IDC_BUTTON_HELP, ANF_RIGHT | ANF_BOTTOM)
	ANCHOR_MAP_ENTRY(IDC_CHECK_FIXEDMODE, ANF_RIGHT | ANF_BOTTOM)
	ANCHOR_MAP_ENTRY(IDC_STATIC2, ANF_LEFT | ANF_BOTTOM)
END_ANCHOR_MAP()


BEGIN_MESSAGE_MAP(CGraphWindow, CDialogEx)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_UPDATE, &CGraphWindow::OnBnClickedButtonUpdate)
	ON_BN_CLICKED(IDC_CHECK_FIXEDMODE, &CGraphWindow::OnBnClickedCheckFixedmode)
	ON_BN_CLICKED(IDC_BUTTON_HELP, &CGraphWindow::OnBnClickedButtonHelp)
END_MESSAGE_MAP()


// CGraphWindow message handlers


BOOL CGraphWindow::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	InitAnchors();

	CRect hwr;
	GetDlgItem(IDC_STATIC_GRAPH)->GetWindowRect(hwr);
	ScreenToClient(hwr);

	m_multiPlotWindows = new CMultiPlotWindow();
	BOOL result = m_multiPlotWindows->Create(NULL, NULL, WS_VISIBLE | WS_CHILD,
		hwr, this, AFX_IDW_PANE_FIRST);

	m_multiPlotWindows->SetScrollSizes(MM_TEXT, CSize(hwr.Width() - 30, 100));
	m_multiPlotWindows->ShowWindow(SW_SHOW);


	const string plotCommand = g_config.m_plotCommand;
	CString strComandEditor;
	if (plotCommand.empty())
	{
		strComandEditor = L"plot1 = 0, 0, 0, 0, 0\r\n\
string1 = %f;\r\n\
plot2 = 0, 0, 0, 0, 0\r\n\
string2 = %*f; %f;\r\n\
plot3 = 0, 0, 0, 0, 0\r\n\
string3 = %*f; %*f; %f;\r\n\
plot4 = 0, 0, 0, 0, 0\r\n\
string4 = %*f; %*f; %*f; %f;\r\n";
	}
	else
	{
		strComandEditor = common::str2wstr(plotCommand).c_str();
	}

	m_PlotCommand = strComandEditor;	


	UpdateData(FALSE);

	return TRUE;
}


void CGraphWindow::Update(const float deltaSeconds)
{
	// 30프레임 단위로 값을 저장하게 한다.
	m_incSeconds += deltaSeconds;
	if (m_incSeconds < 0.033f)
		return;

	if (m_multiPlotWindows)
		m_multiPlotWindows->DrawGraph(deltaSeconds);
}


void CGraphWindow::SetGraphValue(const string &str)
{
	if (m_multiPlotWindows)
		m_multiPlotWindows->SetString(str.c_str());
}

void CGraphWindow::SetGraphValue2(float v)
{
	if (m_multiPlotWindows)
		m_multiPlotWindows->SetXY(0, v, 0);
}

void CGraphWindow::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	CRect rcWnd;
	GetWindowRect(&rcWnd);
	HandleAnchors(&rcWnd);

	if (m_multiPlotWindows && m_multiPlotWindows->GetSafeHwnd())
	{
		CRect hwr;
		GetDlgItem(IDC_STATIC_GRAPH)->GetWindowRect(hwr);
		ScreenToClient(hwr);
		m_multiPlotWindows->MoveWindow(hwr);
	}
}


void CGraphWindow::OnBnClickedButtonUpdate()
{
	UpdateData();
	m_multiPlotWindows->ProcessPlotCommand(m_PlotCommand);

	g_config.m_plotCommand = common::wstr2str((LPCTSTR)m_PlotCommand);
}


void CGraphWindow::OnBnClickedCheckFixedmode()
{
	UpdateData();
	m_multiPlotWindows->SetFixedWidthMode(m_FixedMode? true : false);
}


void CGraphWindow::OnBnClickedButtonHelp()
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
name = plotname\n\
\n\
예제)\n\
	- Serial Data\n\
		- Start, 1234, 1.11, 2.34, 3.33, 4.44\n\
\n\
	- Plot Command Script\n\
		plot1 = 0, 0, 0, 0, 0\n\
		string1 = %*s %f, %f, %*f, %*f, %*f\n\
		name1 = cos\n\
\n\
		plot2 = 0, 0, 0, 0, 0\n\
		string2 = %*s %f, %*f, %f, %*f, %*f\n\
		name2 = sin\n\
");

}
