// JoystickView.cpp : implementation file
//

#include "stdafx.h"
#include "MotionMonitor.h"
#include "JoystickView.h"
#include "afxdialogex.h"
#include <stdio.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <hidsdi.h>

#pragma comment(lib, "hid.lib")

#define ARRAY_SIZE(x)	(sizeof(x) / sizeof((x)[0]))
#define CHECK(exp)		{ if(!(exp)) goto Error; }
#define SAFE_FREE(p)	{ if(p) { HeapFree(hHeap, 0, p); (p) = NULL; } }



// CJoystickView dialog

CJoystickView::CJoystickView(CWnd* pParent /*=NULL*/)
	: CDockablePaneChildView(CJoystickView::IDD, pParent)
	, m_multiPlotWindows(NULL)
	, m_isStart(false)
	, m_incTime(0)
{
}

CJoystickView::~CJoystickView()
{
}

void CJoystickView::DoDataExchange(CDataExchange* pDX)
{
	CDockablePaneChildView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_START, m_StartButton);
}


BEGIN_ANCHOR_MAP(CJoystickView)
 	ANCHOR_MAP_ENTRY(IDC_STATIC_GRAPH, ANF_LEFT | ANF_TOP | ANF_RIGHT | ANF_BOTTOM)
// 	ANCHOR_MAP_ENTRY(IDC_RICHEDIT2_LOG, ANF_TOP | ANF_LEFT | ANF_RIGHT)
// 	ANCHOR_MAP_ENTRY(IDC_RICHEDIT2_OUTPUT_LOG, ANF_TOP | ANF_LEFT | ANF_RIGHT)
END_ANCHOR_MAP()


BEGIN_MESSAGE_MAP(CJoystickView, CDockablePaneChildView)
	ON_BN_CLICKED(IDOK, &CJoystickView::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CJoystickView::OnBnClickedCancel)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_START, &CJoystickView::OnBnClickedButtonStart)
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

	const CString plotCommand = L"plot1 = 0, 0, 0, 0, 0\r\n\
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

	if (m_isStart)
	{
		m_isStart = false;
		m_StartButton.SetWindowTextW(L"JoyStick Start");
	}
	else
	{
		m_isStart = true;
		m_multiPlotWindows->ProcessPlotCommand(plotCommand, 1);
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

		if (m_incTime > 0.033f)
		{
			m_multiPlotWindows->SetXY(0, (float)m_AxisRz, 0);
			m_multiPlotWindows->SetXY(1, (float)m_AxisY, 0);
			m_multiPlotWindows->SetXY(2, (float)m_AxisX, 0);
			m_multiPlotWindows->SetXY(3, (float)m_AxisH, 0);
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

// 		// 시리얼통신을 주기적으로 보낸다.
// 		static int oldT = 0;
// 		int curT = GetTickCount();
// 		if (curT - oldT > 300)
// 		{
// 			char buff[5] = { 'S', (char)lAxisX, (char)lAxisY, (char)lAxisZ, (char)lAxisRz };
// 			oldT = curT;
// 		}
// 		
// 		InvalidateRect(m_hWnd, NULL, TRUE);
// 		UpdateWindow(m_hWnd);
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
