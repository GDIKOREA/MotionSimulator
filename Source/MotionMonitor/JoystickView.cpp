// JoystickView.cpp : implementation file
//

#include "stdafx.h"
#include "MotionMonitor.h"
#include "JoystickView.h"
#include "afxdialogex.h"
#include <stdio.h>
#include "MotionController.h"


#define _USE_MATH_DEFINES
#include <math.h>
#include <hidsdi.h>

#pragma comment(lib, "hid.lib")

#define ARRAY_SIZE(x)	(sizeof(x) / sizeof((x)[0]))
#define CHECK(exp)		{ if(!(exp)) goto Error; }
#define SAFE_FREE(p)	{ if(p) { HeapFree(hHeap, 0, p); (p) = NULL; } }


const static CString g_joystickPlotCommand = L"\r\n\
plot1 = 0, 256, 0, 0, 0\r\n\
string1 = %f;\r\n\
name1 = Yaw\r\n\
linewidth1 = 2\r\n\
\r\n\
plot2 = 0, 512, 0, 0, 0\r\n\
string2 = %*f; %f;\r\n\
name2 = Pitch\r\n\
linewidth2 = 2\r\n\
\r\n\
plot3 = 0, 512, 0, 0, 0\r\n\
string3 = %*f; %*f; %f;\r\n\
name3 = Roll\r\n\
linewidth3 = 2\r\n\
\r\n\
plot4 = 0, 256, 0, 0, 0\r\n\
string4 = %*f; %*f; %f; \r\n\
linewidth4 = 2\r\n\
name4 = Heave\r\n\
\r\n\
";


// CJoystickView dialog

CJoystickView::CJoystickView(CWnd* pParent /*=NULL*/)
	: CDockablePaneChildView(CJoystickView::IDD, pParent)
	, m_multiPlotWindows(NULL)
	, m_isStart(false)
	, m_isMotionOutputStart(false)
	, m_isRecord(false)
	, m_incTime(0)
	, m_recordIncTime(0)
	, m_CheckFixedMode(TRUE)
	, m_AxisX(0)
	, m_AxisY(0)
	, m_AxisZ(0)
	, m_AxisRz(0)
	, m_AxisH(0)
	, m_Hat(0)
{
}

CJoystickView::~CJoystickView()
{
}

void CJoystickView::DoDataExchange(CDataExchange* pDX)
{
	CDockablePaneChildView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_START, m_StartButton);
	DDX_Control(pDX, IDC_BUTTON_RECORD, m_RecordButton);
	DDX_Control(pDX, IDC_RICHEDIT2_COMMAND, m_EditCommand);
	DDX_Check(pDX, IDC_CHECK_FIXEDMODE, m_CheckFixedMode);
}


BEGIN_ANCHOR_MAP(CJoystickView)
 	ANCHOR_MAP_ENTRY(IDC_STATIC_GRAPH, ANF_LEFT | ANF_TOP | ANF_RIGHT | ANF_BOTTOM)
	ANCHOR_MAP_ENTRY(IDC_RICHEDIT2_COMMAND, ANF_LEFT | ANF_TOP | ANF_RIGHT )

	ANCHOR_MAP_ENTRY(IDC_BUTTON_RECORD, ANF_TOP | ANF_RIGHT)
END_ANCHOR_MAP()


BEGIN_MESSAGE_MAP(CJoystickView, CDockablePaneChildView)
	ON_BN_CLICKED(IDOK, &CJoystickView::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CJoystickView::OnBnClickedCancel)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_START, &CJoystickView::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_RECORD, &CJoystickView::OnBnClickedButtonRecord)
	ON_BN_CLICKED(IDC_BUTTON_UPDATE, &CJoystickView::OnBnClickedButtonUpdate)
	ON_BN_CLICKED(IDC_CHECK_FIXEDMODE, &CJoystickView::OnBnClickedCheckFixedmode)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CJoystickView message handlers

BOOL CJoystickView::OnInitDialog()
{
	CDockablePaneChildView::OnInitDialog();

	InitAnchors();

	// Plot창 생성.
	CRect rect;
	GetClientRect(rect);

	m_multiPlotWindows = new CMultiPlotWindow();
	BOOL result = m_multiPlotWindows->Create(NULL, NULL, WS_VISIBLE | WS_CHILD,
		CRect(0, 0, rect.Width(), 400), this, AFX_IDW_PANE_FIRST);

	m_multiPlotWindows->SetScrollSizes(MM_TEXT, CSize(rect.Width() - 30, 100));
	m_multiPlotWindows->ShowWindow(SW_SHOW);
	m_multiPlotWindows->SetFixedWidthMode(m_CheckFixedMode? true:false);


	const CString command =
L"# joystick\n\
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
spline_plot_sampling_rate = 10\n\
spline_interpolation_rate = 10\n\
\n\
";

	CString cmdStr;
	std::ifstream cfgfile("joystick_config.cfg");
	if (cfgfile.is_open())
	{
		std::string str((std::istreambuf_iterator<char>(cfgfile)), std::istreambuf_iterator<char>());
		cmdStr = str2wstr(str).c_str();
	}
	else
	{
		cmdStr = command;
	}
	m_EditCommand.SetWindowTextW(cmdStr);


	UpdateData(FALSE);

	return TRUE;
}


void CJoystickView::OnBnClickedOk()
{
//	CDockablePaneChildView::OnOK();
}


void CJoystickView::OnBnClickedCancel()
{
//	CDockablePaneChildView::OnCancel();
}


void CJoystickView::OnSize(UINT nType, int cx, int cy)
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


// 조이스틱 인식을 시작한다.
void CJoystickView::OnBnClickedButtonStart()
{
	RET(!m_multiPlotWindows);

	UpdateData();

	if (m_isStart)
	{
		StopJoyStickUpdate();
	}
	else
	{
		m_isStart = true;
		m_multiPlotWindows->ProcessPlotCommand(g_joystickPlotCommand, 3);
		m_multiPlotWindows->SetFixedWidthMode(m_CheckFixedMode ? true : false);

		InitJoyStick();

		m_StartButton.SetWindowTextW(L"Stop");

		CString command;
		m_EditCommand.GetWindowTextW(command);
		cMotionController::Get()->m_joystickMod.ParseStr(common::wstr2str((LPCTSTR)command).c_str());
	}

}


void CJoystickView::StopJoyStickUpdate()
{
	m_isStart = false;
	m_StartButton.SetWindowTextW(L"JoyStick Monitor Start");
}


// 매프레임마다 호출된다.
void CJoystickView::Update(const float deltaSeconds)
{
	if (m_isStart)
	{
		m_incTime += deltaSeconds;

		const float t = cMotionController::Get()->m_joystickMod.m_totalIncTime;

		// 조이스틱 축 계산
		cMotionController::Get()->m_joystickMod.Update(deltaSeconds, (float)m_AxisRz, (float)m_AxisY, (float)m_AxisX, (float)m_AxisH);

		// 계산 된 값을 가져온다.
		float yaw, pitch, roll, heave;
		cMotionController::Get()->m_joystickMod.GetFinal(yaw, pitch, roll, heave);

		float origYaw, origPitch, origRoll, origHeave;
		cMotionController::Get()->m_joystickMod.GetOriginal(origYaw, origPitch, origRoll, origHeave);


		const float elapsT = 0.033f;
		if (m_incTime > elapsT)
		{
			//----------------------------------------------------------------------------------------------
			// original
			m_multiPlotWindows->SetXY(0, t, origYaw, 0);
			m_multiPlotWindows->SetXY(1, t, origPitch, 0);
			m_multiPlotWindows->SetXY(2, t, origRoll, 0);
			m_multiPlotWindows->SetXY(3, t, origHeave, 0);


			//----------------------------------------------------------------------------------------------
			// modulation Spline
			if (cMotionController::Get()->m_joystickMod.m_isSplineEnable)
			{
				vector<Vector2> yawSpline, pitchSpline, rollSpline, heaveSpline;
				if (cMotionController::Get()->m_joystickMod.GetSplineInterpolations(0, 1.f, yawSpline, 
					pitchSpline, rollSpline, heaveSpline))
				{
					for (u_int i = 0; i < yawSpline.size(); ++i)
					{
						m_multiPlotWindows->SetXY(0, yawSpline[i].x, yawSpline[i].y, 1);
						m_multiPlotWindows->SetXY(1, pitchSpline[i].x, pitchSpline[i].y, 1);
						m_multiPlotWindows->SetXY(2, rollSpline[i].x, rollSpline[i].y, 1);
						m_multiPlotWindows->SetXY(3, heaveSpline[i].x, heaveSpline[i].y, 1);
					}
				}
			}
			else
			{
				m_multiPlotWindows->SetXY(0, t, yaw, 1);
				m_multiPlotWindows->SetXY(1, t, pitch, 1);
				m_multiPlotWindows->SetXY(2, t, roll, 1);
				m_multiPlotWindows->SetXY(3, t, heave, 1);
			}
			//----------------------------------------------------------------------------------------------


			//----------------------------------------------------------------------------------------------
			// modulation record
			if (m_isRecord)
			{
				sMotionData out;
				sMotionData data;
				data.yaw = yaw;
				data.pitch = pitch;
				data.roll = roll;
				data.heave = heave;

				if (m_recordMWave.Record(m_incTime, data, &out))
				{
					m_recordData = out;
					m_multiPlotWindows->SetXY(0, t, out.yaw, 2);
					m_multiPlotWindows->SetXY(1, t, out.pitch, 2);
					m_multiPlotWindows->SetXY(2, t, out.roll, 2);
					m_multiPlotWindows->SetXY(3, t, out.heave, 2);
				}
			}


			m_multiPlotWindows->DrawGraph(elapsT);

			//m_incTime -= elapsT;
			m_incTime = 0;
		}
	}
}


// 조이스틱 디바이스 초기화.
bool CJoystickView::InitJoyStick()
{
	//
	// Register for joystick devices
	//
	RAWINPUTDEVICE rid;
	rid.usUsagePage = 1;
	rid.usUsage = 4;	// Joystick
	rid.dwFlags = 0;
	rid.hwndTarget = GetSafeHwnd();
	if (!RegisterRawInputDevices(&rid, 1, sizeof(RAWINPUTDEVICE)))
		return false;

	return true;
}


// 윈도우 메세지 처리
BOOL CJoystickView::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	switch (message)
	{
	case WM_INPUT:
	{
		RETV(!m_isStart, TRUE);

		//
		// Get the pointer to the raw device data, process it and update the window
		//
		RAWINPUT RawInput;
		ZeroMemory(&RawInput, sizeof(RAWINPUT));
		UINT bufferSize = sizeof(RawInput);

		GetRawInputData((HRAWINPUT)lParam, RID_INPUT, &RawInput, &bufferSize, sizeof(RAWINPUTHEADER));
		ParseRawInput(&RawInput);
	}
	return 0;

	default:
		return CDockablePaneChildView::OnWndMsg(message, wParam, lParam, pResult);
	}
	
	return TRUE;
}


void CJoystickView::ParseRawInput(PRAWINPUT pRawInput)
{
	PHIDP_PREPARSED_DATA pPreparsedData = NULL;
	HIDP_CAPS            Caps;
	PHIDP_BUTTON_CAPS    pButtonCaps = NULL;
	PHIDP_BUTTON_CAPS    pButtonCaps2 = NULL;
	PHIDP_VALUE_CAPS     pValueCaps = NULL;
	USHORT               capsLength;
	UINT                 bufferSize;
	HANDLE               hHeap = GetProcessHeap();
	USAGE                usage[MAX_BUTTONS];
	ULONG                i, usageLength, value;

	//
	// Get the preparsed data block
	//

	CHECK(GetRawInputDeviceInfo(pRawInput->header.hDevice, RIDI_PREPARSEDDATA, NULL, &bufferSize) == 0);
	CHECK(pPreparsedData = (PHIDP_PREPARSED_DATA)HeapAlloc(hHeap, 0, bufferSize));
	CHECK((int)GetRawInputDeviceInfo(pRawInput->header.hDevice, RIDI_PREPARSEDDATA, pPreparsedData, &bufferSize) >= 0);

	//
	// Get the joystick's capabilities
	//

	// Button caps
	CHECK(HidP_GetCaps(pPreparsedData, &Caps) == HIDP_STATUS_SUCCESS)
		CHECK(pButtonCaps = (PHIDP_BUTTON_CAPS)HeapAlloc(hHeap, 0, sizeof(HIDP_BUTTON_CAPS) * Caps.NumberInputButtonCaps));

	capsLength = Caps.NumberInputButtonCaps;
	CHECK(HidP_GetButtonCaps(HidP_Input, pButtonCaps, &capsLength, pPreparsedData) == HIDP_STATUS_SUCCESS);

	m_NumberOfButtons = 0;
	for (int i = 0; i < capsLength; ++i)
	{
		PHIDP_BUTTON_CAPS bcaps = &pButtonCaps[i];
		m_NumberOfButtons += bcaps->Range.UsageMax - bcaps->Range.UsageMin + 1;
	}


	// Value caps
	CHECK(pValueCaps = (PHIDP_VALUE_CAPS)HeapAlloc(hHeap, 0, sizeof(HIDP_VALUE_CAPS) * Caps.NumberInputValueCaps));
	capsLength = Caps.NumberInputValueCaps;
	CHECK(HidP_GetValueCaps(HidP_Input, pValueCaps, &capsLength, pPreparsedData) == HIDP_STATUS_SUCCESS)

		//
		// Get the pressed buttons
		//
		usageLength = m_NumberOfButtons;
	CHECK(
		HidP_GetUsages(
		HidP_Input, pButtonCaps->UsagePage, 0, usage, &usageLength, pPreparsedData,
		(PCHAR)pRawInput->data.hid.bRawData, pRawInput->data.hid.dwSizeHid
		) == HIDP_STATUS_SUCCESS);

	ZeroMemory(m_bButtonStates, sizeof(m_bButtonStates));
	for (i = 0; i < usageLength; i++)
		m_bButtonStates[usage[i] - pButtonCaps->Range.UsageMin] = TRUE;

	//
	// Get the state of discrete-valued-controls
	//

	for (i = 0; i < Caps.NumberInputValueCaps; i++)
	{
		CHECK(
			HidP_GetUsageValue(
			HidP_Input, pValueCaps[i].UsagePage, 0, pValueCaps[i].Range.UsageMin, &value, pPreparsedData,
			(PCHAR)pRawInput->data.hid.bRawData, pRawInput->data.hid.dwSizeHid
			) == HIDP_STATUS_SUCCESS);

		switch (pValueCaps[i].Range.UsageMin)
		{
		case 0x30:	// X-axis
			m_AxisX = (LONG)value - 512;
			break;

		case 0x31:	// Y-axis
			m_AxisY = (LONG)value - 512;
			break;

		case 0x32: // Z-axis
			m_AxisZ = (LONG)value - 512;
			break;

		case 0x35: // Rotate-Z
			m_AxisRz = (LONG)value - 128;
			break;

		case 0x36: // Heave
			m_AxisH = (LONG)value - 128;
			break;

		case 0x39:	// Hat Switch
			m_Hat = value;
			break;
		}
	}

	//
	// Clean up
	//

Error:
	SAFE_FREE(pPreparsedData);
	SAFE_FREE(pButtonCaps);
	SAFE_FREE(pValueCaps);
}


void CJoystickView::OnBnClickedButtonRecord()
{
	RET(!m_isStart);

	UpdateData();

	if (m_isRecord)
	{
		m_isRecord = false;
		m_recordMWave.Stop();
		StopJoyStickUpdate(); // Stop Joystick Update

		CString Filter = L"MotionWave File(*.mwav)|*.mwav";
		CFileDialog dlg(
			FALSE,
			L"mwav",
			L"motionwave.mwav",
			OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
			Filter,
			this);
		if (IDOK == dlg.DoModal())
		{
			CString fileName = dlg.GetFileName();
			m_recordMWave.Write(wstr2str((LPCTSTR)fileName));
		}

		m_RecordButton.SetWindowTextW(L"Record");
	}
	else
	{
		m_isRecord = true;
		m_multiPlotWindows->ProcessPlotCommand(g_joystickPlotCommand, 3);

		m_recordMWave.StartRecord();
		m_RecordButton.SetWindowTextW(L"Record Stop");
	}
}


// Motion Wave Modulator 를 업데이트한다.
void CJoystickView::OnBnClickedButtonUpdate()
{
	UpdateData();

	CString command;
	m_EditCommand.GetWindowTextW(command);
	cMotionController::Get()->m_joystickMod.ParseStr(common::wstr2str((LPCTSTR)command).c_str());
}


void CJoystickView::OnBnClickedCheckFixedmode()
{
	UpdateData();
	m_multiPlotWindows->SetFixedWidthMode(m_CheckFixedMode ? true : false);
}


void CJoystickView::OnDestroy()
{
	UpdateData();

	// 환경파일 저장
	std::ofstream cfgfile("joystick_config.cfg");
	if (cfgfile.is_open())
	{
		CString command;
		m_EditCommand.GetWindowTextW(command);
		string str = wstr2str((LPCTSTR)command);
		cfgfile << str;
	}

	CDockablePaneChildView::OnDestroy();	
}
