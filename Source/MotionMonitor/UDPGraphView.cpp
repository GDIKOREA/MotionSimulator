// UDPGraphView.cpp : implementation file
//

#include "stdafx.h"
#include "MotionMonitor.h"
#include "UDPGraphView.h"
#include "afxdialogex.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>
#include "MotionController.h"

#define M_PPI 3.14159265358979323846f

struct sMotionPacket
{
	float directX, directY, directZ; // 자동차 방향 벡터
	float pitch, yaw, roll; // 자동차 상태 radian
	float speed; // 자동차 속도
};


// CUDPGraphView dialog
CUDPGraphView::CUDPGraphView(CWnd* pParent /*=NULL*/)
	: CDockablePaneChildView(CUDPGraphView::IDD, pParent)
	, m_ServerPort(20777)
	, m_multiPlotWindows(NULL)
//	, m_isServerBind(false)
	, m_isPause(false)
{
}

CUDPGraphView::~CUDPGraphView()
{
}

void CUDPGraphView::OnDestroy()
{
	WriteConfigFile("udp_conf.json");

	CDockablePaneChildView::OnDestroy();
}


void CUDPGraphView::DoDataExchange(CDataExchange* pDX)
{
	CDockablePaneChildView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_COMMAND, m_CommandEditor);
	DDX_Text(pDX, IDC_EDIT1, m_ServerPort);
	DDX_Control(pDX, IDC_BUTTON_UPDATE, m_UpdateButton);
	DDX_Control(pDX, IDC_BUTTON_HELP, m_HelpButton);
	DDX_Control(pDX, IDC_CHECK_FIXEDMODE, m_FixedModeButton);
	DDX_Control(pDX, IDC_BUTTON_UDP_SERVER_BIND, m_ServerBindButton);
	DDX_Text(pDX, IDC_STATIC_PACKET, m_PacketString);
	DDX_Control(pDX, IDC_MFCEDITBROWSE_XMLPATH, m_fileBrowser);
}


BEGIN_ANCHOR_MAP(CUDPGraphView)
	ANCHOR_MAP_ENTRY(IDC_EDIT_COMMAND, ANF_LEFT | ANF_RIGHT | ANF_BOTTOM)
	ANCHOR_MAP_ENTRY(IDC_BUTTON_UPDATE, ANF_RIGHT | ANF_BOTTOM)
	ANCHOR_MAP_ENTRY(IDC_BUTTON_HELP, ANF_RIGHT | ANF_BOTTOM)
	ANCHOR_MAP_ENTRY(IDC_BUTTON_PAUSE, ANF_RIGHT | ANF_BOTTOM)
	ANCHOR_MAP_ENTRY(IDC_CHECK_FIXEDMODE, ANF_LEFT | ANF_BOTTOM)
	ANCHOR_MAP_ENTRY(IDC_STATIC1, ANF_LEFT | ANF_BOTTOM)
END_ANCHOR_MAP()


BEGIN_MESSAGE_MAP(CUDPGraphView, CDockablePaneChildView)
	ON_BN_CLICKED(IDOK, &CUDPGraphView::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CUDPGraphView::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_HELP, &CUDPGraphView::OnBnClickedButtonHelp)
	ON_BN_CLICKED(IDC_BUTTON_UPDATE, &CUDPGraphView::OnBnClickedButtonUpdate)
	ON_BN_CLICKED(IDC_BUTTON_UDP_SERVER_BIND, &CUDPGraphView::OnBnClickedButtonUdpServerBind)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_CHECK_FIXEDMODE, &CUDPGraphView::OnBnClickedCheckFixedmode)
	ON_BN_CLICKED(IDC_BUTTON_PAUSE, &CUDPGraphView::OnBnClickedButtonPause)
	ON_EN_CHANGE(IDC_MFCEDITBROWSE_XMLPATH, &CUDPGraphView::OnEnChangeMfceditbrowseXmlpath)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CUDPGraphView message handlers

BOOL CUDPGraphView::OnInitDialog()
{
	CDockablePaneChildView::OnInitDialog();

	InitAnchors();


	//-----------------------------------------------------------------------------
	// read config file
	string serverPort;
	string plotCommand;
	string xmlFilePath;
	try
	{
		// boost property tree
		using boost::property_tree::ptree;
		using std::string;
		ptree props;
		boost::property_tree::read_json("udp_conf.json", props);
		serverPort = props.get<string>("server port");
		plotCommand = props.get<string>("plot command");
		xmlFilePath = props.get<string>("xml file path");
	}
	catch (std::exception&e)
	{
		::AfxMessageBox( CString(L"Error!!\n") + str2wstr(e.what()).c_str() );
	}
	//-----------------------------------------------------------------------------


	CString strComandEditor;
	if (plotCommand.empty())
	{
		strComandEditor = L"plot1 = 0, 0, 0, 0, 0\r\n\
string1 = %f;\r\n\
name1 = Yaw\r\n\
plot2 = 0, 0, 0, 0, 0\r\n\
string2 = %*f; %f;\r\n\
name2 = Pitch\r\n\
plot3 = 0, 0, 0, 0, 0\r\n\
string3 = %*f; %*f; %f;\r\n\
name3 = Roll\r\n\
plot4 = 0, 0, 0, 0, 0\r\n\
string4 = %*f; %*f; %*f; %f;\r\n\
name4 = Heave\r\n";
	}
	else
	{
		strComandEditor = str2wstr(plotCommand).c_str();
	}

	m_CommandEditor.SetWindowTextW(strComandEditor);		


	if (!xmlFilePath.empty())
	{
		m_fileBrowser.SetWindowTextW(str2wstr(xmlFilePath).c_str());
	}

	m_ServerPort = atoi(serverPort.c_str());


	// Plot창 생성.
	CRect rect;
	GetClientRect(rect);

	m_multiPlotWindows = new CMultiPlotWindow();
	BOOL result = m_multiPlotWindows->Create(NULL, NULL, WS_VISIBLE | WS_CHILD,
		CRect(0, 0, rect.Width(), 400), this, AFX_IDW_PANE_FIRST);

	m_multiPlotWindows->SetScrollSizes(MM_TEXT, CSize(rect.Width() - 30, 100));
	m_multiPlotWindows->ShowWindow(SW_SHOW);


	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CUDPGraphView::OnBnClickedOk()
{
	//CDockablePaneChildView::OnOK();
}


void CUDPGraphView::OnBnClickedCancel()
{
	//CDockablePaneChildView::OnCancel();
}


void CUDPGraphView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePaneChildView::OnSize(nType, cx, cy);

	CRect rcWnd;
	GetWindowRect(&rcWnd);
	HandleAnchors(&rcWnd);

	if (m_multiPlotWindows && m_multiPlotWindows->GetSafeHwnd())
	{
		CRect hwr;
		m_HelpButton.GetWindowRect(hwr);
		ScreenToClient(hwr);
		m_multiPlotWindows->MoveWindow(CRect(0, 85, cx, hwr.top));
	}
}


void CUDPGraphView::OnBnClickedButtonHelp()
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
\n\
예제)\n\
	- Serial Data\n\
		- Start, 1234, 1.11, 2.34, 3.33, 4.44\n\
\n\
	- Plot Command Script\n\
		plot1 = 0, 0, 0, 0, 0\n\
		string1 = %*s %f, %f, %*f, %*f, %*f\n\
\n\
		plot2 = 0, 0, 0, 0, 0\n\
		string2 = %*s %f, %*f, %f, %*f, %*f\n\
");
}


void CUDPGraphView::OnBnClickedButtonUpdate()
{
	CString str;
	m_CommandEditor.GetWindowText(str);
	m_multiPlotWindows->ProcessPlotCommand(str);
}


void CUDPGraphView::Update(const float deltaSeconds)
{
 	if (!m_isPause && m_multiPlotWindows)
 		m_multiPlotWindows->DrawGraph(deltaSeconds);
}


void CUDPGraphView::UpdateUDP(const char *buffer, const int bufferLen)
{
	if (bufferLen < 10)
	{
		m_PacketString = common::str2wstr(buffer).c_str();
		UpdateData(FALSE);
		return;
	}

	std::stringstream ss;
	vector<float> values(4);

	if (0) // Dirt3 Motion
	{
		// 패킷의 특정 값만 float으로 변환해서 가져온다.
		// 프로토콜에 대한 문서는 https://github.com/GDIKOREA/MotionSimulator/wiki/Dirt3-Motion-Data-Capture 를 참조하자.
		// packet[16~19] : yaw, radian, float
		// packet[20~23] : pitch, radian, float
		// packet[24~27] : roll, radian, float
		// packet[28~31] : heave, float(확실치 않음)

		const int indices[] = { 16, 20, 24, 28 };
		const int size = sizeof(indices) / sizeof(int);
		for (int i = 0; i < size; ++i)
		{
			float v = *(float*)&buffer[indices[i]];
			values[i] = v;
			ss << v << ";";
		}

		values[2] -= 1.5f;

		const float roll = values[2];
		const float pitch = values[1];
		const float yaw = 0;// -values[0];

		cController::Get()->GetCubeFlight().SetEulerAngle(roll, yaw, pitch);

		m_PacketString = common::str2wstr(ss.str()).c_str();

		// 그래프 출력
		if (m_multiPlotWindows)
			m_multiPlotWindows->SetString(ss.str().c_str());

		cMotionController::Get()->SetMotion(0, pitch, roll);
	}
	else
	{
		if (bufferLen < sizeof(sMotionPacket))
			return;

		sMotionPacket *packet = (sMotionPacket*)buffer;
		values[0] = packet->yaw;
		values[1] = packet->pitch;
		values[2] = packet->roll;
		values[3] = 0;

		// 3D Model에 적용.
		float roll = -values[2];
		float pitch = -values[1];
		const float yaw = 0;
		cController::Get()->GetCubeFlight().SetEulerAngle(roll, yaw, pitch);
		Vector3 euler = cController::Get()->GetCubeFlight().GetRotation().Euler();
		roll = euler.x;
		pitch = euler.z;

		ss << 0 << ";";
		ss << pitch << ";";
		ss << roll << ";";
		ss << 0 << ";";

		m_PacketString = common::str2wstr(ss.str()).c_str();

		// 그래프 출력
		if (m_multiPlotWindows)
			m_multiPlotWindows->SetString(ss.str().c_str());

		cMotionController::Get()->SetMotion(0, pitch, roll);
	}

	
	UpdateData(FALSE);
}


void CUDPGraphView::OnBnClickedCheckFixedmode()
{
	if (m_multiPlotWindows)
		m_multiPlotWindows->SetFixedWidthMode(m_FixedModeButton.GetCheck() > 0);
}


// UDP 서버 실행.
void CUDPGraphView::OnBnClickedButtonUdpServerBind()
{
	UpdateData();

	if (cController::Get()->GetUDPComm().IsConnect())
	{
		cController::Get()->GetUDPComm().Close();
		m_ServerBindButton.SetWindowTextW(L"Server Start");
	}
	else
	{
		if (cController::Get()->GetUDPComm().InitServer(m_ServerPort))
		{
			m_ServerBindButton.SetWindowTextW(L"Server Stop");

			CString str;
			m_CommandEditor.GetWindowText(str);
			m_multiPlotWindows->ProcessPlotCommand(str);
		}
		else
		{
			//m_LogList.InsertString(m_LogList.GetCount(), L"접속 실패");
		}

	}
}


void CUDPGraphView::OnBnClickedButtonPause()
{
	m_isPause = !m_isPause;
}


// 설정파일 저장.
bool CUDPGraphView::WriteConfigFile(const string &fileName)
{
	UpdateData();

	CString strCommandText;
	m_CommandEditor.GetWindowTextW(strCommandText);
	string commandText = wstr2str((LPCTSTR)strCommandText);

	try
	{
		using namespace std;
		ofstream ofs(fileName);
		
		ofs << "{" << endl;
		ofs << "\t\"server port\"" << " : " << "\"" << m_ServerPort << "\"," << endl;
		ofs << "\t\"plot command\"" << " : " << "\"" << commandText << "\"," << endl;
		ofs << "\t\"xml file path\"" << " : " << "\"" << m_xmlFilePath << "\"" << endl;
		ofs << "}" << endl;

	}
	catch (std::exception &e)
	{
		::AfxMessageBox(CString(L"Error!!\n") + str2wstr(e.what()).c_str());
	}

	return true;
}


void CUDPGraphView::OnEnChangeMfceditbrowseXmlpath()
{
	CString xmlFilePath;
	m_fileBrowser.GetWindowTextW(xmlFilePath);
	m_xmlFilePath = wstr2str((LPCTSTR)xmlFilePath);
	
	//  json은 역슬래시 문자가 있을 경우, 두번 넣어줘야 된다. (앞뒤 역슬래시 체크 후)
	string dst;
	for (u_int i = 0; i < m_xmlFilePath.size(); ++i)
	{
		const char c = m_xmlFilePath[i];
		dst += c;
		if (c == '\\')
		{
			if ((i + 1 < m_xmlFilePath.size()) && 
				('\\' != m_xmlFilePath[i + 1]))
			{
				if ((i - 1 > 0) &&
					('\\' != m_xmlFilePath[i - 1]))
				{
					dst += '\\';
				}
			}
		}
	}
	//

	m_xmlFilePath = dst;

	//ReadHardwareSettingConfigFile(m_xmlFilePath);
}


void CUDPGraphView::ReadHardwareSettingConfigFile(const string &fileName)
{
	UpdateData();

	try
	{
		// populate tree structure pt
		using boost::property_tree::ptree;
		ptree props;
		boost::property_tree::read_xml(fileName, props);
		ptree &child = props.get_child("hardware_settings_config");
		BOOST_FOREACH(const ptree::value_type &f, child)
		{
			if (f.first == "motion")
			{
				string at = f.first + ".<xmlattr>";
				ptree &attributes = child.get_child(at);
// 				string enabled = attributes.get<string>("enabled");
// 				string ip = attributes.get<string>("ip");
// 				string port = attributes.get<string>("port");

				std::stringstream ss;
				ss << m_ServerPort;

				attributes.put("enabled", "true");
				attributes.put("ip", "127.0.0.1");
				attributes.put("port", ss.str());

				boost::property_tree::write_xml(fileName, props);
			}
		}
	}
	catch (std::exception&e)
	{
		::AfxMessageBox(CString(L"Error!!\n") + str2wstr(e.what()).c_str());
	}
}

