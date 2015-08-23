// MotionWaveView.cpp : implementation file
//

#include "stdafx.h"
#include "MotionMonitor.h"
#include "MotionWaveView.h"
#include "afxdialogex.h"


const static CString g_motionwavePlotCommand = L"\r\n\
plot1 = 0, 512, 0, 0, 0\r\n\
string1 = %f;\r\n\
name1 = Yaw\r\n\
linewidth1 = 2\r\n\
plot2 = 0, 512, 0, 0, 0\r\n\
string2 = %*f; %f;\r\n\
name2 = Pitch\r\n\
linewidth2 = 2\r\n\
plot3 = 0, 512, 0, 0, 0\r\n\
string3 = %*f; %*f; %f;\r\n\
name3 = Roll\r\n\
linewidth3 = 2\r\n\
plot4 = 0, 256, 0, 0, 0\r\n\
string4 = %*f; %*f; %f; \r\n\
name4 = Heave\r\n\
linewidth4 = 2\r\n\
";


// CMotionWaveView dialog
CMotionWaveView::CMotionWaveView(CWnd* pParent /*=NULL*/)
	: CDockablePaneChildView(CMotionWaveView::IDD, pParent)
	, m_multiPlotWindows(NULL)
	, m_incTime(0)
	, m_state(STOP)
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
	DDX_Control(pDX, IDC_RICHEDIT2_COMMAND, m_EditCommand);
	DDX_Control(pDX, IDC_TREE_FILEINFO, m_FileInfoTree);
	DDX_Control(pDX, IDC_SLIDER_PLAYPOS, m_PlayPosSlider);
	DDX_Control(pDX, IDC_STATIC_PLAYPOS, m_PlayerPos);
	DDX_Control(pDX, IDC_BUTTON_REFRESH, m_RefreshButton);
}


BEGIN_ANCHOR_MAP(CMotionWaveView)
	ANCHOR_MAP_ENTRY(IDC_TREE_FILE, ANF_LEFT | ANF_TOP | ANF_RIGHT )
	ANCHOR_MAP_ENTRY(IDC_STATIC_GROUP, ANF_LEFT | ANF_TOP | ANF_RIGHT)
	ANCHOR_MAP_ENTRY(IDC_TREE_FILEINFO, ANF_LEFT | ANF_TOP | ANF_RIGHT)
	ANCHOR_MAP_ENTRY(IDC_RICHEDIT2_COMMAND, ANF_LEFT | ANF_TOP | ANF_RIGHT )
	ANCHOR_MAP_ENTRY(IDC_STATIC_GRAPH, ANF_LEFT | ANF_TOP | ANF_RIGHT | ANF_BOTTOM)
	ANCHOR_MAP_ENTRY(IDC_STATIC_GROUP2, ANF_LEFT | ANF_TOP | ANF_RIGHT )
	ANCHOR_MAP_ENTRY(IDC_SLIDER_PLAYPOS, ANF_LEFT | ANF_TOP | ANF_RIGHT )
	ANCHOR_MAP_ENTRY(IDC_BUTTON_CLEAR, ANF_TOP | ANF_RIGHT)
END_ANCHOR_MAP()



BEGIN_MESSAGE_MAP(CMotionWaveView, CDockablePaneChildView)
	ON_BN_CLICKED(IDOK, &CMotionWaveView::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CMotionWaveView::OnBnClickedCancel)
	ON_WM_SIZE()
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_FILE, &CMotionWaveView::OnSelchangedTreeFile)
	ON_BN_CLICKED(IDC_BUTTON_PLAY, &CMotionWaveView::OnBnClickedButtonPlay)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CMotionWaveView::OnBnClickedButtonClear)
	ON_BN_CLICKED(IDC_BUTTON_REFRESH, &CMotionWaveView::OnBnClickedButtonRefresh)
	ON_BN_CLICKED(IDC_BUTTON_UPDATE, &CMotionWaveView::OnBnClickedButtonUpdate)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CMotionWaveView message handlers

BOOL CMotionWaveView::OnInitDialog()
{
	CDockablePaneChildView::OnInitDialog();

	InitAnchors();

	list<string> extList;
	extList.push_back("mwav");
	m_FileTree.Update("../media/", extList);
	m_FileTree.ExpandAll();

	// Plotâ ����.
	CRect rect;
	GetClientRect(rect);

	m_multiPlotWindows = new CMultiPlotWindow();
	BOOL result = m_multiPlotWindows->Create(NULL, NULL, WS_VISIBLE | WS_CHILD,
		CRect(0, 0, rect.Width(), 400), this, AFX_IDW_PANE_FIRST);

	m_multiPlotWindows->SetScrollSizes(MM_TEXT, CSize(rect.Width() - 30, 100));
	m_multiPlotWindows->ShowWindow(SW_SHOW);
	m_multiPlotWindows->SetFixedWidthMode(true);

	m_multiPlotWindows->ProcessPlotCommand(g_motionwavePlotCommand, 2);
	m_multiPlotWindows->SetFixedWidthMode(true);

	UpdateConfig();

	return TRUE;
}


void CMotionWaveView::UpdateConfig(bool IsSaveAndValidate) //IsSaveAndValidate=true
{
	if (IsSaveAndValidate)
	{
		const CString command =
L"# motion wave\n\
\n\
yaw_proportion = 1\n\
pitch_proportion = 1\n\
roll_proportion = 1\n\
heave_proportion = 1\n\
\n\
spline_enable = 1\n\
\n\
yaw_c1 = 0\n\
yaw_c2 = 1\n\
yaw_c3 = 0\n\
\n\
pitch_c1 = 0\n\
pitch_c2 = 1\n\
pitch_c3 = 0\n\
\n\
roll_c1 = 0\n\
roll_c2 = 1\n\
roll_c3 = 0\n\
\n\
heave_c1 = 0\n\
heave_c2 = 1\n\
heave_c3 = 0\n\
\n\
#spline_plot_sampling_rate = 10\n\
#spline_interpolation_rate = 10\n\
\n\
motionview_spline_sampling_rate = 2\n\
motionview_spline_interpolation_rate = 2\n\
motionview_time_scaling = 1\n\
motionview_start_delay = 20\n\
\n\
";

		CString cmdStr;
		if (!cMotionController::Get()->m_config.m_mwaveModCommand.empty())
		{
			cmdStr = str2wstr(cMotionController::Get()->m_config.m_mwaveModCommand).c_str();
		}
		else
		{
			cmdStr = command;
		}

		m_EditCommand.SetWindowTextW(cmdStr);

		UpdateData(FALSE);
	}
	else
	{
		UpdateData();

		// ȯ������ ����
		CString command;
		m_EditCommand.GetWindowTextW(command);
		cMotionController::Get()->m_config.m_mwaveModCommand = wstr2str((LPCTSTR)command);
	}

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
	if (common::GetFileExt(fileName).empty() || (fileName == "../media") || (fileName == ".."))
		return;

	cMotionWave mwave;
	if (mwave.Read(fileName))
	{
		UpdateMotionWaveFile();
		UpdateMotionWaveFileInfo(fileName, mwave);
	}
	else
	{
		AfxMessageBox(L"MotionWave ������ �ƴմϴ�.");
	}
}


void CMotionWaveView::Update(const float deltaSeconds)
{
	RET(STOP==m_state);

	const float elapseT = 0.02f;

	m_incTime += (deltaSeconds * cMotionController::Get()->m_mwavMod.m_motionviewTimeScaling);

	if (m_incTime > elapseT)
	{
		float t = cMotionController::Get()->m_mwavMod.m_totalIncTime;

		sMotionData data;
		if (m_mwaveSpline.Play(m_incTime, data))
		{
			cMotionController::Get()->m_mwavMod.Update(m_incTime, data.yaw, data.pitch, data.roll, data.heave);

			t = cMotionController::Get()->m_mwavMod.m_totalIncTime;

			// ��� �� ���� �����´�.
			float yaw, pitch, roll, heave;
			cMotionController::Get()->m_mwavMod.GetFinal(yaw, pitch, roll, heave);

			//----------------------------------------------------------------------------------------------
			// modulation ��� �� �� ���
			m_multiPlotWindows->SetXY(0, t, yaw, 1);
			m_multiPlotWindows->SetXY(1, t, pitch, 1);
			m_multiPlotWindows->SetXY(2, t, roll, 1);
			m_multiPlotWindows->SetXY(3, t, heave, 1);
		}

		if (m_mwave.Play(m_incTime, data))
		{
			m_mod.Update(m_incTime, data.yaw, data.pitch, data.roll, data.heave);

			// ��� �� ���� �����´�.
			float yaw, pitch, roll, heave;
			m_mod.GetFinal(yaw, pitch, roll, heave);

			//----------------------------------------------------------------------------------------------
			// modulation ��� �� �� ���
			m_multiPlotWindows->SetXY(0, t, yaw, 0);
			m_multiPlotWindows->SetXY(1, t, pitch, 0);
			m_multiPlotWindows->SetXY(2, t, roll, 0);
			m_multiPlotWindows->SetXY(3, t, heave, 0);

			// sliderbar move
			m_PlayPosSlider.SetPos(m_mwave.GetPlayIndex());

			CString playPosStr;
			playPosStr.Format(L"%d / %d", m_mwave.GetPlayIndex(), m_mwave.GetSamplingCount());
			m_PlayerPos.SetWindowTextW(playPosStr);
		}

  		m_incTime -= elapseT;
  		if (m_incTime > elapseT)
			m_incTime = 0;
	}

	m_multiPlotWindows->DrawGraph(deltaSeconds, false);
}


void CMotionWaveView::OnBnClickedButtonPlay()
{
	if (START==m_state)
	{
		Stop();
	}
	else
	{
		const string fileName = m_FileTree.GetSelectFilePath(m_FileTree.GetSelectedItem());
		m_mwave.Read(fileName);

		PlayMWave();
	}
}


void CMotionWaveView::OnBnClickedButtonClear()
{
	m_multiPlotWindows->ProcessPlotCommand(g_motionwavePlotCommand, 2);
}


void CMotionWaveView::OnBnClickedButtonRefresh()
{
	list<string> extList;
	extList.push_back("mwav");
	m_FileTree.Update("../media/", extList);
	m_FileTree.ExpandAll();
}


void CMotionWaveView::OnBnClickedButtonUpdate()
{
	UpdateData();

	CString command;
	m_EditCommand.GetWindowTextW(command);
	cMotionController::Get()->m_mwavMod.ParseStr(common::wstr2str((LPCTSTR)command).c_str());
	m_mod.ParseStr(common::wstr2str((LPCTSTR)command).c_str());

	UpdateMotionWaveFile();
}


// ���õ� ��� ���̺� ������ ������Ʈ �Ѵ�.
void CMotionWaveView::UpdateMotionWaveFile()
{
	const int samplingRate = cMotionController::Get()->m_mwavMod.m_motionviewSplineSamplingRate;
	const int interpolationRate = cMotionController::Get()->m_mwavMod.m_motionviewSplineInterpolationRate;
	const int startDelayTime = cMotionController::Get()->m_mwavMod.m_motionviewStartDelay;
	m_mwaveSpline = m_mwave;

	sMotionData data = { 255, 255, 255, 512 };
	cMotionWave mwave;
	mwave.Make(20, startDelayTime, data);
	m_mwaveSpline.Insert(mwave, 0);
	m_mwaveSpline.MakeSpline(samplingRate, interpolationRate);
}


void CMotionWaveView::UpdateMotionWaveFileInfo(const string &fileName, const cMotionWave &mwav)
{
	// ���� ���� ������Ʈ
	m_FileInfoTree.DeleteAllItems();

	{
		CString name;
		name.Format(L"Name = %s", str2wstr(fileName).c_str());
		m_FileInfoTree.InsertItem(name);
	}

	{
		CString samplingRate;
		samplingRate.Format(L"Sampling Rate = %d Hz", mwav.m_samplingRate);
		m_FileInfoTree.InsertItem(samplingRate);
	}

	{
		CString samplingCount;
		samplingCount.Format(L"Sampling Count = %d", mwav.m_wave.size());
		m_FileInfoTree.InsertItem(samplingCount);
	}

	{
		m_FileInfoTree.InsertItem(
			common::formatw("PlayTime = %.1f seconds", ((float)mwav.m_wave.size() / (float)mwav.m_samplingRate)).c_str());
	}

	m_PlayPosSlider.SetRange(0, mwav.GetSamplingCount());
	m_PlayPosSlider.SetPos(0);

	CString playPosStr;
	playPosStr.Format(L"%d / %d", 0, mwav.GetSamplingCount());
	m_PlayerPos.SetWindowTextW(playPosStr);

}


// ��ǿ��̺� ���� �ε�.
bool CMotionWaveView::LoadandPlayMotionWave(const string &fileName)
{
	m_mwave.StopPlay();
	m_mwaveSpline.StopPlay();

	if (m_mwave.Read(fileName))
	{
		UpdateMotionWaveFile();
		UpdateMotionWaveFileInfo(fileName, m_mwave);
	}

	PlayMWave();

	return true;
}


// ��ǿ��̺긦 �÷����Ѵ�.
void CMotionWaveView::PlayMWave()
{
	// update config
	CString command;
	m_EditCommand.GetWindowTextW(command);
	cMotionController::Get()->m_mwavMod.ParseStr(common::wstr2str((LPCTSTR)command).c_str());
	m_mod.ParseStr(common::wstr2str((LPCTSTR)command).c_str());
	UpdateMotionWaveFile();

	m_state = START;
	m_mwave.StartPlay();
	m_mwaveSpline.StartPlay();
	m_PlayButton.SetWindowTextW(L"Stop");

	m_FileTree.EnableWindow(FALSE);
	m_FileInfoTree.EnableWindow(FALSE);
	m_RefreshButton.EnableWindow(FALSE);
	SetBackgroundColor(g_blueColor);
}


void CMotionWaveView::OnDestroy()
{
	UpdateConfig(false);

	CDockablePaneChildView::OnDestroy();
}


void CMotionWaveView::Start()
{

}


// �÷��� ���� ��ǿ��̺긦 �����.
void CMotionWaveView::Stop()
{
	m_state = STOP;
	m_mwave.StopPlay();
	m_mwaveSpline.StopPlay();
	m_PlayButton.SetWindowTextW(L"Play");

	m_FileTree.EnableWindow(TRUE);
	m_FileInfoTree.EnableWindow(TRUE);
	m_RefreshButton.EnableWindow(TRUE);

	SetBackgroundColor(g_grayColor);
}
