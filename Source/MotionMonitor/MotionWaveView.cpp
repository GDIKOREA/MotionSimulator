// MotionWaveView.cpp : implementation file
//

#include "stdafx.h"
#include "MotionMonitor.h"
#include "MotionWaveView.h"
#include "afxdialogex.h"


const static CString g_motionwavePlotCommand = L"plot1 = 0, 0, 0, 0, 0\r\n\
string1 = %f;\r\n\
name1 = Yaw\r\n\
plot2 = 0, 0, 0, 0, 0\r\n\
string2 = %*f; %f;\r\n\
name2 = Pitch\r\n\
plot3 = 0, 0, 0, 0, 0\r\n\
string3 = %*f; %*f; %f;\r\n\
name3 = Roll\r\n\
plot4 = 0, 0, 0, 0, 0\r\n\
string4 = %*f; %*f; %f; \r\n\
name4 = Heave\r\n";


// CMotionWaveView dialog
CMotionWaveView::CMotionWaveView(CWnd* pParent /*=NULL*/)
	: CDockablePaneChildView(CMotionWaveView::IDD, pParent)
	, m_multiPlotWindows(NULL)
	, m_incTime(0)
	, m_isPlay(false)
{

}

CMotionWaveView::~CMotionWaveView()
{
}

void CMotionWaveView::DoDataExchange(CDataExchange* pDX)
{
	CDockablePaneChildView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_FILE, m_FileTree);
	DDX_Control(pDX, IDC_BUTTON_PLAY, m_PlayButton);
}


BEGIN_ANCHOR_MAP(CMotionWaveView)
	ANCHOR_MAP_ENTRY(IDC_TREE_FILE, ANF_LEFT | ANF_TOP | ANF_RIGHT )
	ANCHOR_MAP_ENTRY(IDC_STATIC_GRAPH, ANF_LEFT | ANF_TOP | ANF_RIGHT | ANF_BOTTOM)
	ANCHOR_MAP_ENTRY(IDC_BUTTON_CLEAR, ANF_TOP | ANF_RIGHT)
END_ANCHOR_MAP()



BEGIN_MESSAGE_MAP(CMotionWaveView, CDockablePaneChildView)
	ON_BN_CLICKED(IDOK, &CMotionWaveView::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CMotionWaveView::OnBnClickedCancel)
	ON_WM_SIZE()
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_FILE, &CMotionWaveView::OnSelchangedTreeFile)
	ON_BN_CLICKED(IDC_BUTTON_PLAY, &CMotionWaveView::OnBnClickedButtonPlay)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CMotionWaveView::OnBnClickedButtonClear)
END_MESSAGE_MAP()


// CMotionWaveView message handlers

BOOL CMotionWaveView::OnInitDialog()
{
	CDockablePaneChildView::OnInitDialog();

	InitAnchors();

	list<string> extList;
	extList.push_back("mwav");
	m_FileTree.Update("./", extList);
	m_FileTree.ExpandAll();


	// Plot창 생성.
	CRect rect;
	GetClientRect(rect);

	m_multiPlotWindows = new CMultiPlotWindow();
	BOOL result = m_multiPlotWindows->Create(NULL, NULL, WS_VISIBLE | WS_CHILD,
		CRect(0, 0, rect.Width(), 400), this, AFX_IDW_PANE_FIRST);

	m_multiPlotWindows->SetScrollSizes(MM_TEXT, CSize(rect.Width() - 30, 100));
	m_multiPlotWindows->ShowWindow(SW_SHOW);
	m_multiPlotWindows->SetFixedWidthMode(true);


	m_multiPlotWindows->ProcessPlotCommand(g_motionwavePlotCommand, 1);
	m_multiPlotWindows->SetFixedWidthMode(true);

	return TRUE;
}


void CMotionWaveView::OnBnClickedOk()
{
}


void CMotionWaveView::OnBnClickedCancel()
{
}


void CMotionWaveView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePaneChildView::OnSize(nType, cx, cy);

	CRect rcWnd;
	GetWindowRect(&rcWnd);
	HandleAnchors(&rcWnd);

	if (m_multiPlotWindows && m_multiPlotWindows->GetSafeHwnd())
	{
		CRect pwr;
		GetDlgItem(IDC_STATIC_GRAPH)->GetWindowRect(pwr);
		ScreenToClient(pwr);
		m_multiPlotWindows->MoveWindow(pwr);
	}
}


void CMotionWaveView::OnSelchangedTreeFile(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	*pResult = 0;

	const string fileName = m_FileTree.GetSelectFilePath(pNMTreeView->itemNew.hItem);
	if (common::GetFileExt(fileName).empty() || (fileName == "./media") || (fileName == "."))
		return;

	if (!m_mwaveFile.Read(fileName))
	{
		AfxMessageBox(L"MotionWave 파일이 아닙니다.");
	}
}


void CMotionWaveView::Update(const float deltaSeconds)
{
	RET(!m_isPlay);

	m_incTime += deltaSeconds;
	if (m_incTime > 0.03f)
	{
		sMotionData data;
		if (m_mwaveFile.Play(m_incTime, data))
		{
			m_multiPlotWindows->SetXY(0, data.yaw, 0);
			m_multiPlotWindows->SetXY(1, data.pitch, 0);
			m_multiPlotWindows->SetXY(2, data.roll, 0);
			m_multiPlotWindows->SetXY(3, data.heave, 0);
		}

		m_multiPlotWindows->DrawGraph(m_incTime);

		m_incTime = 0;
	}
}


void CMotionWaveView::OnBnClickedButtonPlay()
{
	if (m_isPlay)
	{
		m_isPlay = false;
		m_mwaveFile.StopPlay();
		m_PlayButton.SetWindowTextW(L"Play");
	}
	else
	{
		m_isPlay = true;
		m_mwaveFile.StartPlay();
		m_PlayButton.SetWindowTextW(L"Stop");
	}
}


void CMotionWaveView::OnBnClickedButtonClear()
{
	m_multiPlotWindows->ProcessPlotCommand(g_motionwavePlotCommand, 1);
}
