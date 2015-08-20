// UDPPlayerView.cpp : implementation file
//

#include "stdafx.h"
#include "UDPAnalyzer.h"
#include "UDPPlayerView.h"
#include "afxdialogex.h"


const static COLORREF g_playColor = RGB(51, 153, 255);
const static string g_temporaryUDPStreamFileName = "../media/udpstream_temp.udp";


// CUDPPlayerView dialog
CUDPPlayerView::CUDPPlayerView(CWnd* pParent /*=NULL*/)
	: CDockablePaneChildView(CUDPPlayerView::IDD, pParent)
	, m_state(STOP)
	, m_IsRepeat(FALSE)
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
	DDX_Control(pDX, IDC_STATIC_PLAY_POS, m_PlayPos);
	DDX_Check(pDX, IDC_CHECK_REPEAT, m_IsRepeat);
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
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_PLAY, &CUDPPlayerView::OnNMCustomdrawSliderPlay)
	ON_BN_CLICKED(IDC_CHECK_REPEAT, &CUDPPlayerView::OnBnClickedCheckRepeat)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CUDPPlayerView message handlers
BOOL CUDPPlayerView::OnInitDialog()
{
	CDockablePaneChildView::OnInitDialog();

	InitAnchors();

	list<string> extList;
	extList.push_back("udp");
	m_FileTree.Update("../media/", extList);
	m_FileTree.ExpandAll();

	m_UDPIP.SetAddress(127, 0, 0, 1);
	m_UDPPort.SetWindowTextW(L"8888");

	return TRUE;
}


// 환경설정에 저장된 정보를 UI에 표시한다.
void CUDPPlayerView::UpdateConfig()
{
	vector<string> ipnums;
	tokenizer(g_option.m_udpPlayerIP, ".", "", ipnums);
	if (ipnums.size() >= 4)
	{
		m_UDPIP.SetAddress(atoi(ipnums[0].c_str()),
			atoi(ipnums[1].c_str()),
			atoi(ipnums[2].c_str()),
			atoi(ipnums[3].c_str()));
	}
	else
	{
		m_UDPIP.SetAddress(127, 0, 0, 1);
	}

	m_UDPPort.SetWindowTextW(formatw("%d", g_option.m_udpPlayerPort).c_str());
}


// UI에 표시된 정보를 환경설정에 저장한다.
void CUDPPlayerView::SaveConfig()
{
	UpdateData();

	g_option.m_udpPlayerIP = GetIP(m_UDPIP);

	CString udpPort;
	m_UDPPort.GetWindowTextW(udpPort);
	g_option.m_udpPlayerPort = _wtoi(udpPort);
}


void CUDPPlayerView::OnBnClickedOk()
{
}
void CUDPPlayerView::OnBnClickedCancel()
{
}


void CUDPPlayerView::OnBnClickedButtonFileRefresh()
{
	list<string> extList;
	extList.push_back("udp");
	m_FileTree.Update("../media/", extList);
	m_FileTree.ExpandAll();
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
	UpdateData();

	if (PLAY == ChangeState((m_state == PLAY) ? PAUSE : PLAY))
	{
		// UDP Stream 이 Play 되면, UDP 네트워크를 연결해서 정보를 전송한다.
		const string ip = GetIP(m_UDPIP);
		CString udpPortStr;
		m_UDPPort.GetWindowTextW(udpPortStr);
		const int port = _wtoi((LPCTSTR)udpPortStr);

		if (m_udpClient.Init(ip, port))
		{
			m_udpClient.m_sleepMillis = 10;
		}
		else
		{
			// UDP 접속실패
			AfxMessageBox(L"UDP 접속 실패");
			ChangeState(STOP);
		}
	}
}


void CUDPPlayerView::OnBnClickedButtonStop()
{
	m_udpClient.Close();
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
	if (common::GetFileExt(fileName).empty() 
		|| (fileName == "../media") 
		|| (fileName == "..")
		|| (fileName == "../media/udpanalyzer"))
	{
		m_FileInfoTree.DeleteAllItems();
		return;
	}

	cUDPStream udpStream;
	if (udpStream.Open(false, fileName))
	{
		UpdateUDPFileInfo(fileName, udpStream);
	}
	else
	{
		AfxMessageBox(L"UDP Stream 파일이 아닙니다.");
	}
}


// udpStream 정보를 fileinformation UI에 출력한다.
void CUDPPlayerView::UpdateUDPFileInfo(const string &fileName, const cUDPStream &udpStream)
{
	// 파일 정보 업데이트
	m_FileInfoTree.DeleteAllItems();

	{
		CString name;
		name.Format(L"File Name = %s", str2wstr(fileName).c_str());
		m_FileInfoTree.InsertItem(name);
	}

	{
		CString versionStr;
		versionStr.Format(L"Version = %d", udpStream.m_version);
		m_FileInfoTree.InsertItem(versionStr);
	}

	{
		CString bufferLenStr;
		bufferLenStr.Format(L"Element Buffer Size = %d", udpStream.m_bufferLen);
		m_FileInfoTree.InsertItem(bufferLenStr);
	}

	{
		CString elemSizeStr;
		elemSizeStr.Format(L"Element Buffer Count = %d", udpStream.m_totalElementSize);
		m_FileInfoTree.InsertItem(elemSizeStr);
	}

	m_PlaySlider.SetRange(0, udpStream.m_totalElementSize);
	m_PlaySlider.SetPos(0);

	CString playPosStr;
	playPosStr.Format(L"1 / %d", udpStream.m_totalElementSize);
	m_PlayPos.SetWindowTextW(playPosStr);

}


// 
void CUDPPlayerView::Update(const float deltaSeconds)
{

	switch (m_state)
	{
	case CUDPPlayerView::STOP:
		break;
	case CUDPPlayerView::PLAY:
		{
			if (!m_udpPlayer.IsPlay())
				break;

			const int bufferLen = m_udpPlayer.Update(m_buffer, sizeof(m_buffer));
			if (bufferLen > 0)
			{
				m_udpClient.SendData(m_buffer, 128);
				
				const int curPos = m_udpPlayer.GetCurrentPlayElementIndex();
				m_PlaySlider.SetPos(curPos);

				CString playPosStr;
				playPosStr.Format(L"%d / %d", curPos, m_udpPlayer.GetTotalElementSize());
				m_PlayPos.SetWindowTextW(playPosStr);
			}
			else if (bufferLen < 0) // 종료
			{
				if (m_IsRepeat)
				{
					ChangeState(STOP);
					ChangeState(PLAY);
				}
				else
				{
					ChangeState(STOP);
				}
			}
		}
		break;

	case CUDPPlayerView::PAUSE:
		break;
	case CUDPPlayerView::RECORD:
		break;
	default:
		break;
	}

}


// 
void CUDPPlayerView::UpdateUDP(const char *buffer, const int bufferLen)
{
	switch (m_state)
	{
	case CUDPPlayerView::STOP:
		break;
	case CUDPPlayerView::PLAY:
		break;
	case CUDPPlayerView::PAUSE:
		break;
	case CUDPPlayerView::RECORD:
		m_udpStream.Write(buffer, bufferLen);
		break;
	default:
		break;
	}
}


void CUDPPlayerView::Play(const string &fileName)
{
	m_udpPlayer.Open(fileName);
	m_udpPlayer.Play();
}


void CUDPPlayerView::RecordEnd()
{
	m_udpStream.Close();

	TCHAR szFilter[] = L"UDPStreaming(*.udp)|*.udp| All Files(*.*)|*.*||";
	CFileDialog dlg(FALSE, NULL, NULL, OFN_HIDEREADONLY, szFilter);
	if (IDOK == dlg.DoModal())
	{
		// 저장이 완료된 임시파일의 이름을 변경한다.
		rename(g_temporaryUDPStreamFileName.c_str(), wstr2str((LPCTSTR)dlg.GetPathName()).c_str() );
	}

	OnBnClickedButtonFileRefresh(); // 파일리스트 업데이트
}


void CUDPPlayerView::OnNMCustomdrawSliderPlay(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	*pResult = 0;

	// 일시 정시 상태일 때만, 스크롤을 움직일 수 있다.
	if (PAUSE != m_state)
		return;

	const int curPos = m_PlaySlider.GetPos();
	if (m_udpPlayer.SetPlayPos(curPos) > 0)
	{
		CString playPosStr;
		playPosStr.Format(L"%d / %d", curPos, m_udpPlayer.GetTotalElementSize());
		m_PlayPos.SetWindowTextW(playPosStr);
	}

}


void CUDPPlayerView::OnBnClickedCheckRepeat()
{
	UpdateData();
}


void CUDPPlayerView::OnDestroy()
{
	SaveConfig();
	CDockablePaneChildView::OnDestroy();
}


CUDPPlayerView::STATE CUDPPlayerView::ChangeState(const STATE nextState)
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
			{
				m_PlayButton.EnableWindow(TRUE);
				m_StopButton.EnableWindow(TRUE);
				m_RecordButton.EnableWindow(FALSE);
				m_UDPIP.EnableWindow(FALSE);
				m_UDPPort.EnableWindow(FALSE);
				m_FileTree.EnableWindow(FALSE);
				m_FileInfoTree.EnableWindow(FALSE);
				m_RefreshButton.EnableWindow(FALSE);
				m_PlayButton.SetWindowTextW(L"Pause");
				SetBackgroundColor(g_playColor);

				const string fileName = m_FileTree.GetSelectFilePath(m_FileTree.GetSelectedItem());
				Play(fileName);
			}
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
			m_RecordButton.SetWindowTextW(L"Record End");
			SetBackgroundColor(RGB(255, 127, 80));

			m_udpStream.Open(true, g_temporaryUDPStreamFileName);
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

				m_udpPlayer.Stop();
				break;

			case CUDPPlayerView::PLAY:
				break;

			case CUDPPlayerView::PAUSE:
				m_PlayButton.EnableWindow(TRUE);
				m_StopButton.EnableWindow(TRUE);
				m_RecordButton.EnableWindow(FALSE);
				m_PlayButton.SetWindowTextW(L"Play");
				SetBackgroundColor(RGB(119, 136, 153));

				m_udpPlayer.Pause();
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

			m_udpPlayer.Stop();
			break;

		case CUDPPlayerView::PLAY:
			m_PlayButton.EnableWindow(TRUE);
			m_StopButton.EnableWindow(TRUE);
			m_RecordButton.EnableWindow(FALSE);
			m_PlayButton.SetWindowTextW(L"Pause");
			SetBackgroundColor(g_playColor);

			m_udpPlayer.Play();
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

			RecordEnd();
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

	return m_state;
}
