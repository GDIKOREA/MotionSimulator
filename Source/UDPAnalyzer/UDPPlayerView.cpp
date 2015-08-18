// UDPPlayerView.cpp : implementation file
//

#include "stdafx.h"
#include "UDPAnalyzer.h"
#include "UDPPlayerView.h"
#include "afxdialogex.h"


// CUDPPlayerView dialog
CUDPPlayerView::CUDPPlayerView(CWnd* pParent /*=NULL*/)
	: CDockablePaneChildView(CUDPPlayerView::IDD, pParent)
	, m_state(STOP)
{

}

CUDPPlayerView::~CUDPPlayerView()
{
}

void CUDPPlayerView::DoDataExchange(CDataExchange* pDX)
{
	CDockablePaneChildView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_FILE, m_FileTree);
	DDX_Control(pDX, IDC_BUTTON_PLAY, m_PlayButton);
	DDX_Control(pDX, IDC_BUTTON_STOP, m_StopButton);
	DDX_Control(pDX, IDC_BUTTON_RECORD, m_RecordButton);
	DDX_Control(pDX, IDC_SLIDER_PLAY, m_PlaySlider);
	DDX_Control(pDX, IDC_TREE_FILEINFO, m_FileInfoTree);
	DDX_Control(pDX, IDC_IPADDRESS_UDP_IP, m_UDPIP);
	DDX_Control(pDX, IDC_EDIT_UDP_PORT, m_UDPPort);
	DDX_Control(pDX, IDC_BUTTON_FILE_REFRESH, m_RefreshButton);
}


BEGIN_ANCHOR_MAP(CUDPPlayerView)
 	ANCHOR_MAP_ENTRY(IDC_TREE_FILE, ANF_LEFT | ANF_RIGHT | ANF_TOP)
	ANCHOR_MAP_ENTRY(IDC_TREE_FILEINFO, ANF_LEFT | ANF_RIGHT | ANF_TOP)
	ANCHOR_MAP_ENTRY(IDC_BUTTON_FILE_REFRESH, ANF_RIGHT | ANF_TOP)
	ANCHOR_MAP_ENTRY(IDC_STATIC_GROUP, ANF_LEFT | ANF_RIGHT | ANF_TOP)
	ANCHOR_MAP_ENTRY(IDC_SLIDER_PLAY, ANF_LEFT | ANF_RIGHT | ANF_TOP)
END_ANCHOR_MAP()


BEGIN_MESSAGE_MAP(CUDPPlayerView, CDockablePaneChildView)
	ON_BN_CLICKED(IDOK, &CUDPPlayerView::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CUDPPlayerView::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_FILE_REFRESH, &CUDPPlayerView::OnBnClickedButtonFileRefresh)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_PLAY, &CUDPPlayerView::OnBnClickedButtonPlay)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CUDPPlayerView::OnBnClickedButtonStop)
	ON_BN_CLICKED(IDC_BUTTON_RECORD, &CUDPPlayerView::OnBnClickedButtonRecord)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_FILE, &CUDPPlayerView::OnSelchangedTreeFile)
END_MESSAGE_MAP()


// CUDPPlayerView message handlers
BOOL CUDPPlayerView::OnInitDialog()
{
	CDockablePaneChildView::OnInitDialog();

	InitAnchors();

	list<string> extList;
	//extList.push_back("udp");
	extList.push_back("mwav");
	m_FileTree.Update("../media/", extList);
	m_FileTree.ExpandAll();

	m_UDPIP.SetAddress(127, 0, 0, 1);
	m_UDPPort.SetWindowTextW(L"8888");

	return TRUE;
}


void CUDPPlayerView::OnBnClickedOk()
{
}
void CUDPPlayerView::OnBnClickedCancel()
{
}


void CUDPPlayerView::OnBnClickedButtonFileRefresh()
{

}


void CUDPPlayerView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePaneChildView::OnSize(nType, cx, cy);

	CRect rcWnd;
	GetWindowRect(&rcWnd);
	HandleAnchors(&rcWnd);
}


void CUDPPlayerView::OnBnClickedButtonPlay()
{
	ChangeState((m_state == PLAY)? PAUSE : PLAY);
}


void CUDPPlayerView::OnBnClickedButtonStop()
{
	ChangeState(STOP);
}


void CUDPPlayerView::OnBnClickedButtonRecord()
{
	ChangeState((m_state == RECORD) ? STOP : RECORD);
}


void CUDPPlayerView::OnSelchangedTreeFile(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	*pResult = 0;

	const string fileName = m_FileTree.GetSelectFilePath(pNMTreeView->itemNew.hItem);
	if (common::GetFileExt(fileName).empty() || (fileName == "../media") || (fileName == ".."))
		return;

// 	if (m_mwave.Read(fileName))
// 	{
// 		UpdateMotionWaveFile();
// 		UpdateUDPFileInfo(fileName, m_mwave);
// 	}
// 	else
// 	{
// 		AfxMessageBox(L"MotionWave 파일이 아닙니다.");
// 	}
}


// 
void CUDPPlayerView::UpdateUDP(const char *buffer, const int bufferLen)
{

}


void CUDPPlayerView::ChangeState(const STATE nextState)
{
	switch (m_state)
	{
	// Stop State
	case CUDPPlayerView::STOP:
	{
		switch (nextState)
		{
		case CUDPPlayerView::STOP:
			break;
		case CUDPPlayerView::PLAY:
			m_PlayButton.EnableWindow(TRUE);
			m_StopButton.EnableWindow(TRUE);
			m_RecordButton.EnableWindow(FALSE);
			m_UDPIP.EnableWindow(FALSE);
			m_UDPPort.EnableWindow(FALSE);
			m_FileTree.EnableWindow(FALSE);
			m_FileInfoTree.EnableWindow(FALSE);
			m_RefreshButton.EnableWindow(FALSE);
			m_PlayButton.SetWindowTextW(L"Pause");
			SetBackgroundColor(RGB(51, 153, 255));
			break;
		case CUDPPlayerView::PAUSE:
			m_PlayButton.SetWindowTextW(L"Play");
			break;
		case CUDPPlayerView::RECORD:
			m_PlayButton.EnableWindow(FALSE);
			m_StopButton.EnableWindow(FALSE);
			m_RecordButton.EnableWindow(TRUE);
			m_UDPIP.EnableWindow(FALSE);
			m_UDPPort.EnableWindow(FALSE);
			m_FileTree.EnableWindow(FALSE);
			m_RefreshButton.EnableWindow(FALSE);
			m_FileInfoTree.EnableWindow(FALSE);
			m_RecordButton.SetWindowTextW(L"Stop");
			SetBackgroundColor(RGB(255, 127, 80));
			break;
		default:
			break;
		}
		m_state = nextState;
	}
	break;

	// Play State
	case CUDPPlayerView::PLAY:
	{
		switch (nextState)
			{
			case CUDPPlayerView::STOP:
				m_PlayButton.EnableWindow(TRUE);
				m_StopButton.EnableWindow(TRUE);
				m_RecordButton.EnableWindow(TRUE);
				m_UDPIP.EnableWindow(TRUE);
				m_UDPPort.EnableWindow(TRUE);
				m_FileTree.EnableWindow(TRUE);
				m_FileInfoTree.EnableWindow(TRUE);
				m_RefreshButton.EnableWindow(TRUE);
				m_PlayButton.SetWindowTextW(L"Play");
				m_RecordButton.SetWindowTextW(L"Record");
				SetBackgroundColor(RGB(240, 240, 240));
				break;

			case CUDPPlayerView::PLAY:
				break;

			case CUDPPlayerView::PAUSE:
				m_PlayButton.EnableWindow(TRUE);
				m_StopButton.EnableWindow(TRUE);
				m_RecordButton.EnableWindow(FALSE);
				m_PlayButton.SetWindowTextW(L"Play");
				SetBackgroundColor(RGB(119, 136, 153));
				break;

			case CUDPPlayerView::RECORD:
				break;

			default:
				break;
			}
			m_state = nextState;
		}
		break;

	// Pause State
	case CUDPPlayerView::PAUSE:
	{
		switch (nextState)
		{
		case CUDPPlayerView::STOP:
			m_PlayButton.EnableWindow(TRUE);
			m_StopButton.EnableWindow(TRUE);
			m_RecordButton.EnableWindow(TRUE);
			m_UDPIP.EnableWindow(TRUE);
			m_UDPPort.EnableWindow(TRUE);
			m_FileTree.EnableWindow(TRUE);
			m_FileInfoTree.EnableWindow(TRUE);
			m_RefreshButton.EnableWindow(TRUE);
			m_PlayButton.SetWindowTextW(L"Play");
			m_RecordButton.SetWindowTextW(L"Record");
			SetBackgroundColor(RGB(240, 240, 240));
			break;

		case CUDPPlayerView::PLAY:
			m_PlayButton.EnableWindow(TRUE);
			m_StopButton.EnableWindow(TRUE);
			m_RecordButton.EnableWindow(FALSE);
			m_PlayButton.SetWindowTextW(L"Pause");
			SetBackgroundColor(RGB(51, 153, 255));
			break;

		case CUDPPlayerView::PAUSE:
			break;

		case CUDPPlayerView::RECORD:
			break;

		default:
			break;
		}
		m_state = nextState;
	}
	break;

	case CUDPPlayerView::RECORD:
	{
		switch (nextState)
		{
		case CUDPPlayerView::STOP:
			m_PlayButton.EnableWindow(TRUE);
			m_StopButton.EnableWindow(TRUE);
			m_RecordButton.EnableWindow(TRUE);
			m_UDPIP.EnableWindow(TRUE);
			m_UDPPort.EnableWindow(TRUE);
			m_FileTree.EnableWindow(TRUE);
			m_FileInfoTree.EnableWindow(TRUE);
			m_RefreshButton.EnableWindow(TRUE);
			m_PlayButton.SetWindowTextW(L"Play");
			m_RecordButton.SetWindowTextW(L"Record");
			SetBackgroundColor(RGB(240, 240, 240));
			break;

		case CUDPPlayerView::PLAY:
		case CUDPPlayerView::PAUSE:
		case CUDPPlayerView::RECORD:
			break;

		default:
			break;
		}
		m_state = nextState;
	}
	break;

	default:
		break;
	}

}

