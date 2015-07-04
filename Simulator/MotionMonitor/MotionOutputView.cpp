// MotionOutputView.cpp : implementation file
//

#include "stdafx.h"
#include "MotionMonitor.h"
#include "MotionOutputView.h"
#include "afxdialogex.h"
#include "MotionController.h"


// CMotionOutputView dialog

CMotionOutputView::CMotionOutputView(CWnd* pParent /*=NULL*/)
: CDockablePaneChildView(CMotionOutputView::IDD, pParent)
	, m_isStartSendMotion(false)
	, m_incTime(0)
{

}

CMotionOutputView::~CMotionOutputView()
{
}

void CMotionOutputView::DoDataExchange(CDataExchange* pDX)
{
	CDockablePaneChildView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_COM_PORTS, m_COMPortComboBox);
	DDX_Control(pDX, IDC_COMBO_COM_BAUDRATE, m_BaudRateComboBox);
	DDX_Control(pDX, IDC_BUTTON_CONNECT, m_ConnectButton);
	DDX_Control(pDX, IDC_BUTTON_START_SEND_MOTION, m_StartSendMotionButton);
	DDX_Control(pDX, IDC_RICHEDIT2_LOG, m_Log);
	DDX_Control(pDX, IDC_RICHEDIT2_OUTPUT_LOG, m_OutputLog);
}


BEGIN_MESSAGE_MAP(CMotionOutputView, CDockablePaneChildView)
	ON_BN_CLICKED(IDOK, &CMotionOutputView::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CMotionOutputView::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_CONNECT, &CMotionOutputView::OnBnClickedButtonConnect)
	ON_BN_CLICKED(IDC_BUTTON_START_SEND_MOTION, &CMotionOutputView::OnBnClickedButtonStartSendMotion)
END_MESSAGE_MAP()


// CMotionOutputView message handlers


void CMotionOutputView::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//CDockablePaneChildView::OnOK();
}


void CMotionOutputView::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	//CDockablePaneChildView::OnCancel();
}


BOOL CMotionOutputView::OnInitDialog()
{
	CDockablePaneChildView::OnInitDialog();

	m_COMPortComboBox.InitList();

	const int baudRate[] = { 9600, 14400, 19200, 38400, 56000, 57600, 115200 };
	for (int i = 0; i < ARRAYSIZE(baudRate); ++i)
	{
		TCHAR brateName[32];
		wsprintf(brateName, L"%d", baudRate[i]);
		m_BaudRateComboBox.InsertString(m_BaudRateComboBox.GetCount(), brateName);
	}
	m_BaudRateComboBox.SetCurSel(0);

	m_Log.SetBackgroundColor(FALSE, RGB(0, 0, 0));
	m_OutputLog.SetBackgroundColor(FALSE, RGB(0, 0, 0));

	return TRUE;
}


void CMotionOutputView::OnBnClickedButtonConnect()
{
	const bool isConnect = cController::Get()->GetSerialComm().IsConnect();

	if (isConnect)
	{
		cController::Get()->CloseSerial();
		m_ConnectButton.SetWindowTextW(L"Start");
	}
	else
	{
		const int portNumber = m_COMPortComboBox.GetPortNum();
		CString baudRate;
		m_BaudRateComboBox.GetWindowTextW(baudRate);

		if (cController::Get()->ConnectSerial(portNumber, _wtoi(baudRate)))
		{
			m_ConnectButton.SetWindowTextW(L"Stop");
		}
		else
		{

		}
	}
}


// 매 프레임마다 호출된다.
void CMotionOutputView::Update(const float deltaSeconds)
{
	const bool isSerialConnect = cController::Get()->GetSerialComm().IsConnect();
	if (m_isStartSendMotion && isSerialConnect)
	{
		//Quaternion quat = cController::Get()->GetCubeFlight().GetRotation();
		m_incTime += deltaSeconds;

		if (m_incTime > 0.01f)
		{
			float roll, pitch, yaw;
			cMotionController::Get()->GetMotion(yaw, pitch, roll);

			CString str;
			str.Format(L"roll=%f, pitch=%f, yaw=%f\n", roll, pitch, yaw);
			AppendToLogAndScroll(&m_Log, str, RGB(200, 200, 200));

			m_incTime = 0;
		}
	}
}


void CMotionOutputView::OnBnClickedButtonStartSendMotion()
{
	if (m_isStartSendMotion)
	{
		m_isStartSendMotion = false;
		m_StartSendMotionButton.SetWindowTextW(L"Start Send Motion");
	}
	else
	{
		m_isStartSendMotion = true;
		m_StartSendMotionButton.SetWindowTextW(L"Stop!! ");
	}
}
