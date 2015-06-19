// GraphWindow.cpp : implementation file
//

#include "stdafx.h"
#include "UDPNetGraph.h"
#include "GraphWindow.h"
#include "afxdialogex.h"
#include "PlotWindow.h"


// CGraphWindow dialog

IMPLEMENT_DYNAMIC(CGraphWindow, CDialogEx)

CGraphWindow::CGraphWindow(CWnd* pParent /*=NULL*/)
	: CDialogEx(CGraphWindow::IDD, pParent)
	, m_plotP(NULL)
{
	m_incSeconds = 0;
}

CGraphWindow::~CGraphWindow()
{
}

void CGraphWindow::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CGraphWindow, CDialogEx)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CGraphWindow message handlers


BOOL CGraphWindow::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CRect pr;
// 	GetWindowRect(pr);
// 	ScreenToClient(pr);
	GetClientRect(pr);

	m_plotP = new CPlotWindow();
	m_plotP->Create(NULL, NULL, AFX_WS_DEFAULT_VIEW | WS_CHILD, pr,
		this, 10001);
	m_plotP->SetScrollSizes(MM_TEXT, CSize(pr.Width() - 30, 100));
	m_plotP->ShowWindow(SW_SHOW);
	m_plotP->SetPlot(0, 0, 0, 0, 0, 3);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CGraphWindow::Update(const float deltaSeconds)
{
	// 30프레임 단위로 값을 저장하게 한다.
	m_incSeconds += deltaSeconds;
	if (m_incSeconds < 0.033f)
		return;

	// Z축과 벗어난 각도 값을 cos(theta) 로 변환해 저장한다.
// 	const Vector3 machineZ = Vector3(0, 0, 1).MultiplyNormal(g_physxView->m_machine.m_sensor.m_tm);
// 	float delta = acos(machineZ.DotProduct(Vector3(0, 0, 1)));
// 	Vector3 axis(0, 0, m_motor->m_location.z);
// 	axis.Normalize();
// 	delta = (axis.CrossProduct(machineZ).x > 0) ? -delta : delta;

	if (m_plotP)
	{
		//m_plotP->SetPlotXY(0, delta);
		//m_plotP->SetPlotXY(0, delta*m_motor->m_p, 1);
		m_plotP->DrawPlot(m_incSeconds);
	}
}


void CGraphWindow::SetGraphValue(const string &str)
{
//	int x, y, z;
//	const int ret = sscanf_s(str.c_str(), "%d;%d;%d", &x, &y, &z);
// 	if (ret >= 3)
// 	{
// 		m_plotP->SetPlotXY(0, (float)x, 0);
// 		m_plotP->SetPlotXY(0, (float)y, 1);
// 		m_plotP->SetPlotXY(0, (float)z, 2);
// 	}

	m_plotP->SetPlotXY(0, (float)str[16], 0);
}

void CGraphWindow::SetGraphValue2(float v)
{
	m_plotP->SetPlotXY(0, v, 0);
}

void CGraphWindow::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	if (m_plotP && m_plotP->GetSafeHwnd())
	{
		m_plotP->MoveWindow(CRect(0, 0, cx, cy));
		m_plotP->SetScrollSizes(MM_TEXT, CSize(cx - 30, 100));
	}
}
