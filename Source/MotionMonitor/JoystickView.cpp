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


const static CString g_joystickPlotCommand = L"plot1 = 0, 0, 0, 0, 0\r\n\
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


// CJoystickView dialog

CJoystickView::CJoystickView(CWnd* pParent /*=NULL*/)
	: CDockablePaneChildView(CJoystickView::IDD, pParent)
	, m_multiPlotWindows(NULL)
	, m_isStart(false)
	, m_isMotionOutputStart(false)
	, m_isRecord(false)
	, m_incTime(0)
	, m_EditYawOffset(L"0")
	, m_EditPitchOffset(L"0")
	, m_EditRollOffset(L"0")
	, m_EditYawScale(L"0")
	, m_EditPitchScale(L"0")
	, m_EditRollScale(L"0")
	, m_EditHeaveOffset(L"0")
	, m_EditHeaveScale(L"0")
	, m_YawOffset(0)
	, m_PitchOffset(0)
	, m_RollOffset(0)
	, m_HeaveOffset(0)
	, m_YawScale(1)
	, m_PitchScale(1)
	, m_RollScale(1)
	, m_HeaveScale(1)
{
}

CJoystickView::~CJoystickView()
{
}

void CJoystickView::DoDataExchange(CDataExchange* pDX)
{
	CDockablePaneChildView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_START, m_StartButton);
	DDX_Control(pDX, IDC_BUTTON_MOTION_OUTPUT_START, m_MotionOutputStartButton);

	DDX_Control(pDX, IDC_SLIDER_YAW_OFFSET, m_SliderYawOffset);
	DDX_Control(pDX, IDC_SLIDER_PITCH_OFFSET, m_SliderPitchOffset);
	DDX_Control(pDX, IDC_SLIDER_ROLL_OFFSET, m_SliderRollOffset);
	DDX_Control(pDX, IDC_SLIDER_YAW, m_SliderYawScale);
	DDX_Control(pDX, IDC_SLIDER_PITCH, m_SliderPitchScale);
	DDX_Control(pDX, IDC_SLIDER_ROLL, m_SliderRollScale);
	DDX_Text(pDX, IDC_EDIT_YAW_OFFSET, m_EditYawOffset);
	DDX_Text(pDX, IDC_EDIT_PITCH_OFFSET, m_EditPitchOffset);
	DDX_Text(pDX, IDC_EDIT_ROLL_OFFSET, m_EditRollOffset);
	DDX_Text(pDX, IDC_EDIT_YAW, m_EditYawScale);
	DDX_Text(pDX, IDC_EDIT_PITCH, m_EditPitchScale);
	DDX_Text(pDX, IDC_EDIT_ROLL, m_EditRollScale);
	DDX_Control(pDX, IDC_SLIDER_HEAVE_OFFSET, m_SliderHeaveOffset);
	DDX_Text(pDX, IDC_EDIT_HEAVE_OFFSET, m_EditHeaveOffset);
	DDX_Control(pDX, IDC_SLIDER_HEAVE, m_SliderHeaveScale);
	DDX_Text(pDX, IDC_EDIT_HEAVE, m_EditHeaveScale);
	DDX_Control(pDX, IDC_BUTTON_RECORD, m_RecordButton);
}


BEGIN_ANCHOR_MAP(CJoystickView)
 	ANCHOR_MAP_ENTRY(IDC_STATIC_GRAPH, ANF_LEFT | ANF_TOP | ANF_RIGHT | ANF_BOTTOM)
	ANCHOR_MAP_ENTRY(IDC_SLIDER_YAW_OFFSET, ANF_LEFT | ANF_TOP | ANF_RIGHT )
	ANCHOR_MAP_ENTRY(IDC_SLIDER_PITCH_OFFSET, ANF_LEFT | ANF_TOP | ANF_RIGHT)
	ANCHOR_MAP_ENTRY(IDC_SLIDER_ROLL_OFFSET, ANF_LEFT | ANF_TOP | ANF_RIGHT)
	ANCHOR_MAP_ENTRY(IDC_SLIDER_HEAVE_OFFSET, ANF_LEFT | ANF_TOP | ANF_RIGHT)
	ANCHOR_MAP_ENTRY(IDC_SLIDER_YAW, ANF_LEFT | ANF_TOP | ANF_RIGHT)
	ANCHOR_MAP_ENTRY(IDC_SLIDER_PITCH, ANF_LEFT | ANF_TOP | ANF_RIGHT)
	ANCHOR_MAP_ENTRY(IDC_SLIDER_ROLL, ANF_LEFT | ANF_TOP | ANF_RIGHT)
	ANCHOR_MAP_ENTRY(IDC_SLIDER_HEAVE, ANF_LEFT | ANF_TOP | ANF_RIGHT)
	ANCHOR_MAP_ENTRY(IDC_EDIT_YAW_OFFSET, ANF_TOP | ANF_RIGHT )
	ANCHOR_MAP_ENTRY(IDC_EDIT_PITCH_OFFSET, ANF_TOP | ANF_RIGHT)
	ANCHOR_MAP_ENTRY(IDC_EDIT_ROLL_OFFSET, ANF_TOP | ANF_RIGHT)
	ANCHOR_MAP_ENTRY(IDC_EDIT_HEAVE_OFFSET, ANF_TOP | ANF_RIGHT)
	ANCHOR_MAP_ENTRY(IDC_EDIT_YAW, ANF_TOP | ANF_RIGHT)
	ANCHOR_MAP_ENTRY(IDC_EDIT_PITCH, ANF_TOP | ANF_RIGHT)
	ANCHOR_MAP_ENTRY(IDC_EDIT_ROLL, ANF_TOP | ANF_RIGHT)
	ANCHOR_MAP_ENTRY(IDC_EDIT_HEAVE, ANF_TOP | ANF_RIGHT)

	ANCHOR_MAP_ENTRY(IDC_BUTTON_RECORD, ANF_TOP | ANF_RIGHT)
END_ANCHOR_MAP()


BEGIN_MESSAGE_MAP(CJoystickView, CDockablePaneChildView)
	ON_BN_CLICKED(IDOK, &CJoystickView::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CJoystickView::OnBnClickedCancel)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_START, &CJoystickView::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_MOTION_OUTPUT_START, &CJoystickView::OnBnClickedButtonMotionOutputStart)
	ON_BN_CLICKED(IDC_BUTTON_RECORD, &CJoystickView::OnBnClickedButtonRecord)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_YAW_OFFSET, &CJoystickView::OnNMCustomdrawSliderYawOffset)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_PITCH_OFFSET, &CJoystickView::OnNMCustomdrawSliderPitchOffset)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_ROLL_OFFSET, &CJoystickView::OnNMCustomdrawSliderRollOffset)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_YAW, &CJoystickView::OnNMCustomdrawSliderYaw)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_PITCH, &CJoystickView::OnNMCustomdrawSliderPitch)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_ROLL, &CJoystickView::OnNMCustomdrawSliderRoll)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_HEAVE_OFFSET, &CJoystickView::OnNMCustomdrawSliderHeaveOffset)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_HEAVE, &CJoystickView::OnNMCustomdrawSliderHeave)
	ON_EN_CHANGE(IDC_EDIT_YAW_OFFSET, &CJoystickView::OnEnChangeEditYawOffset)
	ON_EN_CHANGE(IDC_EDIT_PITCH_OFFSET, &CJoystickView::OnEnChangeEditPitchOffset)
	ON_EN_CHANGE(IDC_EDIT_ROLL_OFFSET, &CJoystickView::OnEnChangeEditRollOffset)
	ON_EN_CHANGE(IDC_EDIT_HEAVE_OFFSET, &CJoystickView::OnEnChangeEditHeaveOffset)
	ON_EN_CHANGE(IDC_EDIT_YAW, &CJoystickView::OnEnChangeEditYaw)
	ON_EN_CHANGE(IDC_EDIT_PITCH, &CJoystickView::OnEnChangeEditPitch)
	ON_EN_CHANGE(IDC_EDIT_ROLL, &CJoystickView::OnEnChangeEditRoll)
	ON_EN_CHANGE(IDC_EDIT_HEAVE, &CJoystickView::OnEnChangeEditHeave)
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
	m_multiPlotWindows->SetFixedWidthMode(true);


	m_SliderYawOffset.SetRange(-512, 512, TRUE);
	m_SliderYawOffset.SetPos(0);
	m_SliderPitchOffset.SetRange(-512, 512, TRUE);
	m_SliderPitchOffset.SetPos(0);
	m_SliderRollOffset.SetRange(-512, 512, TRUE);
	m_SliderRollOffset.SetPos(0);
	m_SliderHeaveOffset.SetRange(-128, 128, TRUE);
	m_SliderHeaveOffset.SetPos(0);

	m_SliderYawScale.SetRange(-3000, 3000, TRUE);
	m_SliderYawScale.SetPos(1000);
	m_SliderPitchScale.SetRange(-3000, 3000, TRUE);
	m_SliderPitchScale.SetPos(1000);
	m_SliderRollScale.SetRange(-3000, 3000, TRUE);
	m_SliderRollScale.SetPos(1000);
	m_SliderHeaveScale.SetRange(-3000, 3000, TRUE);
	m_SliderHeaveScale.SetPos(1000);

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

	if (m_isStart)
	{
		m_isStart = false;
		m_StartButton.SetWindowTextW(L"JoyStick Monitor Start");
	}
	else
	{
		m_isStart = true;
		m_multiPlotWindows->ProcessPlotCommand(g_joystickPlotCommand, 2);
		m_multiPlotWindows->SetFixedWidthMode(true);

		InitJoyStick();

		m_StartButton.SetWindowTextW(L"Stop");
	}

}


// 매프레임마다 호출된다.
void CJoystickView::Update(const float deltaSeconds)
{
	if (m_isStart)
	{
		m_incTime += deltaSeconds;

		// 조이스틱 축 계산
		const float yaw = (m_AxisRz + m_YawOffset) * m_YawScale;
		const float pitch = (m_AxisY + m_PitchOffset) * m_PitchScale;
		const float roll = (m_AxisX + m_RollOffset) * m_RollScale;
		const float heave = (m_AxisH + m_HeaveOffset) * m_HeaveScale;

		cMotionController::Get()->m_joystickYaw = yaw;
		cMotionController::Get()->m_joystickPitch = pitch;
		cMotionController::Get()->m_joystickRoll = roll;
		cMotionController::Get()->m_joystickHeave = heave;

		if (m_isRecord)
		{
			sMotionData data;
			data.yaw = yaw;
			data.pitch = pitch;
			data.roll = roll;
			data.heave = heave;

			sMotionData out;
			if (m_recordMWave.Record(deltaSeconds, data, &out))
			{
				m_multiPlotWindows->SetXY(0, out.yaw, 1);
				m_multiPlotWindows->SetXY(1, out.pitch, 1);
				m_multiPlotWindows->SetXY(2, out.roll, 1);
				m_multiPlotWindows->SetXY(3, out.heave, 1);
			}
		}

		if (m_incTime > 0.033f)
		{
			m_multiPlotWindows->SetXY(0, yaw, 0);
			m_multiPlotWindows->SetXY(1, pitch, 0);
			m_multiPlotWindows->SetXY(2, roll, 0);
			m_multiPlotWindows->SetXY(3, heave, 0);
			m_multiPlotWindows->DrawGraph(m_incTime);

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


void CJoystickView::OnBnClickedButtonMotionOutputStart()
{
	if (m_isMotionOutputStart)
	{
		m_isMotionOutputStart = false;
		m_MotionOutputStartButton.SetWindowTextW(L"Motion Output Start");
	}
	else
	{
		m_isMotionOutputStart = true;
		m_MotionOutputStartButton.SetWindowTextW(L"Stop");
	}	
}


void CJoystickView::OnBnClickedButtonRecord()
{
	if (m_isRecord)
	{
		m_isRecord = false;
		m_recordMWave.Stop();
		m_recordMWave.Write("motionwave.mwav");
		m_RecordButton.SetWindowTextW(L"Record");
	}
	else
	{
		m_isRecord = true;
		m_multiPlotWindows->ProcessPlotCommand(g_joystickPlotCommand, 2);

		m_recordMWave.StartRecord();
		m_RecordButton.SetWindowTextW(L"Record Stop");
	}
}


void CJoystickView::OnNMCustomdrawSliderYawOffset(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	
	if (CSliderCtrl *slider = dynamic_cast<CSliderCtrl*>(GetDlgItem(pNMHDR->idFrom)))
	{
		const int pos = slider->GetPos();
		m_EditYawOffset.Format(L"%d", pos);
		m_YawOffset = (float)pos;
		UpdateData(FALSE);
	}

	*pResult = 0;
}


void CJoystickView::OnNMCustomdrawSliderPitchOffset(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);

	if (CSliderCtrl *slider = dynamic_cast<CSliderCtrl*>(GetDlgItem(pNMHDR->idFrom)))
	{
		const int pos = slider->GetPos();
		m_EditPitchOffset.Format(L"%d", pos);
		m_PitchOffset = (float)pos;
		UpdateData(FALSE);
	}

	*pResult = 0;
}


void CJoystickView::OnNMCustomdrawSliderRollOffset(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	
	if (CSliderCtrl *slider = dynamic_cast<CSliderCtrl*>(GetDlgItem(pNMHDR->idFrom)))
	{
		const int pos = slider->GetPos();
		m_EditRollOffset.Format(L"%d", pos);
		m_RollOffset = (float)pos;
		UpdateData(FALSE);
	}

	*pResult = 0;
}


void CJoystickView::OnNMCustomdrawSliderHeaveOffset(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);

	if (CSliderCtrl *slider = dynamic_cast<CSliderCtrl*>(GetDlgItem(pNMHDR->idFrom)))
	{
		const int pos = slider->GetPos();
		m_EditHeaveOffset.Format(L"%d", pos);
		m_HeaveOffset = (float)pos;
		UpdateData(FALSE);
	}

	*pResult = 0;
}


void CJoystickView::OnNMCustomdrawSliderYaw(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	
	if (CSliderCtrl *slider = dynamic_cast<CSliderCtrl*>(GetDlgItem(pNMHDR->idFrom)))
	{
		const float pos = (float)slider->GetPos() * 0.001f;
		m_EditYawScale.Format(L"%f", pos);
		m_YawScale= pos;
		UpdateData(FALSE);
	}

	*pResult = 0;
}


void CJoystickView::OnNMCustomdrawSliderPitch(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);

	if (CSliderCtrl *slider = dynamic_cast<CSliderCtrl*>(GetDlgItem(pNMHDR->idFrom)))
	{
		const float pos = (float)slider->GetPos() * 0.001f;
		m_EditPitchScale.Format(L"%f", pos);
		m_PitchScale = pos;
		UpdateData(FALSE);
	}

	*pResult = 0;
}


void CJoystickView::OnNMCustomdrawSliderRoll(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);

	if (CSliderCtrl *slider = dynamic_cast<CSliderCtrl*>(GetDlgItem(pNMHDR->idFrom)))
	{
		const float pos = (float)slider->GetPos() * 0.001f;
		m_EditRollScale.Format(L"%f", pos);
		m_RollScale = pos;
		UpdateData(FALSE);
	}

	*pResult = 0;
}


void CJoystickView::OnNMCustomdrawSliderHeave(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	
	if (CSliderCtrl *slider = dynamic_cast<CSliderCtrl*>(GetDlgItem(pNMHDR->idFrom)))
	{
		const float pos = (float)slider->GetPos() * 0.001f;
		m_EditHeaveScale.Format(L"%f", pos);
		m_HeaveScale = pos;
		UpdateData(FALSE);
	}

	*pResult = 0;
}


void CJoystickView::OnEnChangeEditYawOffset()
{
	UpdateData();
	const float pos = (float)_wtof((LPCTSTR)m_EditYawOffset);
	m_SliderYawOffset.SetPos((int)pos);
	m_YawOffset = pos;
}


void CJoystickView::OnEnChangeEditPitchOffset()
{
	UpdateData();
	const float pos = (float)_wtof((LPCTSTR)m_EditPitchOffset);
	m_SliderPitchOffset.SetPos((int)pos);
	m_PitchOffset = pos;
}


void CJoystickView::OnEnChangeEditRollOffset()
{
	UpdateData();
	const float pos = (float)_wtof((LPCTSTR)m_EditRollOffset);
	m_SliderRollOffset.SetPos((int)pos);
	m_RollOffset = pos;
}


void CJoystickView::OnEnChangeEditHeaveOffset()
{
	UpdateData();
	const float pos = (float)_wtof((LPCTSTR)m_EditHeaveOffset);
	m_SliderHeaveOffset.SetPos((int)pos);
	m_HeaveOffset = pos;
}


void CJoystickView::OnEnChangeEditYaw()
{
	UpdateData();
	const float pos = (float)_wtof((LPCTSTR)m_EditYawScale);
	m_SliderYawScale.SetPos((int)(pos*1000.f));
	m_YawScale = pos;
}


void CJoystickView::OnEnChangeEditPitch()
{
	UpdateData();
	const float pos = (float)_wtof((LPCTSTR)m_EditPitchScale);
	m_SliderPitchScale.SetPos((int)(pos*1000.f));
	m_PitchScale = pos;
}


void CJoystickView::OnEnChangeEditRoll()
{
	UpdateData();
	const float pos = (float)_wtof((LPCTSTR)m_EditRollScale);
	m_SliderRollScale.SetPos((int)(pos*1000.f));
	m_RollScale = pos;
}


void CJoystickView::OnEnChangeEditHeave()
{
	UpdateData();
	const float pos = (float)_wtof((LPCTSTR)m_EditHeaveScale);
	m_SliderHeaveScale.SetPos((int)(pos*1000.f));
	m_HeaveScale = pos;
}
