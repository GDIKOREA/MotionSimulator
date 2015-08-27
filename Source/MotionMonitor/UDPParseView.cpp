// UDPParseView.cpp : implementation file
//

#include "stdafx.h"
#include "MotionMonitor.h"
#include "UDPParseView.h"
#include "afxdialogex.h"
#include "UDPInputView.h"


// CUDPParseView dialog

CUDPParseView::CUDPParseView(CWnd* pParent /*=NULL*/)
	: CDockablePaneChildView(CUDPParseView::IDD, pParent)
	, m_IsUpdateSymbol(TRUE)
	, m_state(STOP)
	, m_incTime(0)
	, m_incSymbolUpdateTime(0)
	, m_symbolCount(0)
	, m_EditRollCommand(_T(""))
	, m_EditPitchCommand(_T(""))
	, m_EditYawCommand(_T(""))
	, m_EditHeaveCommand(_T(""))
{
}

CUDPParseView::~CUDPParseView()
{
}

void CUDPParseView::DoDataExchange(CDataExchange* pDX)
{
	CDockablePaneChildView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_PROTOCOL, m_EditProtocolCommand);
	DDX_Control(pDX, IDC_EDIT_MIXING, m_EditMixingCommand);
	DDX_Check(pDX, IDC_CHECK_UPDATE_SYMBOL, m_IsUpdateSymbol);
	DDX_Control(pDX, IDC_LIST_SYMBOL, m_SymbolList);
	DDX_Text(pDX, IDC_EDIT_ROLL, m_EditRollCommand);
	DDX_Text(pDX, IDC_EDIT_PITCH, m_EditPitchCommand);
	DDX_Text(pDX, IDC_EDIT_YAW, m_EditYawCommand);
	DDX_Text(pDX, IDC_EDIT_HEAVE, m_EditHeaveCommand);
	DDX_Control(pDX, IDC_EDIT_PORT, m_EditPort);
	DDX_Control(pDX, IDC_BUTTON_SERVER_START, m_ServerStartButton);
}


BEGIN_ANCHOR_MAP(CUDPParseView)
 	ANCHOR_MAP_ENTRY(IDC_EDIT_PROTOCOL, ANF_LEFT | ANF_TOP | ANF_RIGHT)
	ANCHOR_MAP_ENTRY(IDC_EDIT_MIXING, ANF_LEFT | ANF_TOP | ANF_RIGHT)
	ANCHOR_MAP_ENTRY(IDC_LIST_SYMBOL, ANF_LEFT | ANF_TOP | ANF_RIGHT)
	ANCHOR_MAP_ENTRY(IDC_STATIC_GROUP, ANF_LEFT | ANF_TOP | ANF_RIGHT)
	ANCHOR_MAP_ENTRY(IDC_BUTTON_UPDATE, ANF_TOP | ANF_RIGHT)
	ANCHOR_MAP_ENTRY(IDC_EDIT_ROLL, ANF_LEFT | ANF_TOP | ANF_RIGHT)
	ANCHOR_MAP_ENTRY(IDC_EDIT_PITCH, ANF_LEFT | ANF_TOP | ANF_RIGHT)
	ANCHOR_MAP_ENTRY(IDC_EDIT_YAW, ANF_LEFT | ANF_TOP | ANF_RIGHT)
	ANCHOR_MAP_ENTRY(IDC_EDIT_HEAVE, ANF_LEFT | ANF_TOP | ANF_RIGHT)
END_ANCHOR_MAP()

BEGIN_MESSAGE_MAP(CUDPParseView, CDockablePaneChildView)
	ON_BN_CLICKED(IDCANCEL, &CUDPParseView::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CUDPParseView::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_UPDATE, &CUDPParseView::OnBnClickedButtonUpdate)
	ON_BN_CLICKED(IDC_BUTTON_SERVER_START, &CUDPParseView::OnBnClickedButtonServerStart)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_CHECK_UPDATE_SYMBOL, &CUDPParseView::OnBnClickedCheckUpdateSymbol)
END_MESSAGE_MAP()


// CUDPParseView message handlers
BOOL CUDPParseView::OnInitDialog()
{
	CDockablePaneChildView::OnInitDialog();

	InitAnchors();

	m_SymbolList.InsertColumn(0, L"Name");
	m_SymbolList.InsertColumn(1, L"Value");
	m_SymbolList.SetColumnWidth(0, 50);
	m_SymbolList.SetColumnWidth(1, 200);

	UpdateConfig();

	UpdateData(FALSE);
	return TRUE;
}


void CUDPParseView::UpdateConfig(bool IsSaveAndValidate)//IsSaveAndValidate=true
{
	cMotionMonitorConfig &config = cMotionController::Get()->m_config;

	if (IsSaveAndValidate)
	{
		const CString protocolCmd =
			L"2, uint #test\r\n"
			L"4, float\r\n"
			L"4, float\r\n"
			L"4, float\r\n"
			L"4, float\r\n";
		if (config.m_udpProtocolCommand.empty())
		{
			m_EditProtocolCommand.SetWindowTextW(protocolCmd);
		}
		else
		{
			m_EditProtocolCommand.SetWindowTextW(str2wstr(config.m_udpProtocolCommand).c_str());
		}		

		CString mixingCmd =
			L"$var1 = $1 + $2 + $3\r\n"
			L"$var2 = $1 + $2 + $3\r\n"
			L"$var3 = $1 + $2 + $3\r\n";
		if (config.m_udpProtocolCommand.empty())
		{
			m_EditMixingCommand.SetWindowTextW(mixingCmd);
		}
		else
		{
			m_EditMixingCommand.SetWindowTextW(str2wstr(config.m_mixingScript).c_str());
		}

		CString portStr;
		portStr.Format(L"%d", config.m_udpParsePort);
		m_EditPort.SetWindowTextW(portStr);

		if (config.m_rollCommand.empty())
			m_EditRollCommand = L"$1";
		else
			m_EditRollCommand = str2wstr(config.m_rollCommand).c_str();

		if (config.m_pitchCommand.empty())
			m_EditPitchCommand = L"$2";
		else
			m_EditPitchCommand = str2wstr(config.m_pitchCommand).c_str();

		if (config.m_yawCommand.empty())
			m_EditYawCommand = L"$3";
		else
			m_EditYawCommand = str2wstr(config.m_yawCommand).c_str();

		if (config.m_heaveCommand.empty())
			m_EditHeaveCommand = L"$4";
		else
			m_EditHeaveCommand = str2wstr(config.m_heaveCommand).c_str();

		UpdateData(FALSE);

		ProcessUIData();
	}
	else
	{
		UpdateData();

		CString strPort;
		m_EditPort.GetWindowTextW(strPort);
		config.m_udpParsePort = _wtoi((LPCTSTR)strPort);

		CString protocolCmdText;
		m_EditProtocolCommand.GetWindowTextW(protocolCmdText);
		config.m_udpProtocolCommand = wstr2str((LPCTSTR)protocolCmdText);

		CString mixingCmdText;
		m_EditMixingCommand.GetWindowTextW(mixingCmdText);
		config.m_mixingScript = wstr2str((LPCTSTR)mixingCmdText);

		config.m_rollCommand = wstr2str((LPCTSTR)m_EditRollCommand);
		config.m_pitchCommand = wstr2str((LPCTSTR)m_EditPitchCommand);
		config.m_yawCommand = wstr2str((LPCTSTR)m_EditYawCommand);
		config.m_heaveCommand = wstr2str((LPCTSTR)m_EditHeaveCommand);
	}
}


// UI 정보를 가져와 분석하고, 내부 심볼들을 업데이트 한다.
void CUDPParseView::ProcessUIData()
{
	UpdateData();

	//------------------------------------------------------------------
	// Parse Protocol Command
	CString protocolCmdText;
	m_EditProtocolCommand.GetWindowTextW(protocolCmdText);
	m_protocolParser.ParseStr(wstr2str((LPCTSTR)protocolCmdText));

	// 심볼 업데이트
	script::ClearSymbols();
	int i = 0;
	for each (auto &field in m_protocolParser.m_fields)
	{
		script::sFieldData data;
		ZeroMemory(data.buff, sizeof(data));
		data.type = field.type;

		const string id = format("$%d", i + 1); // $1 ,$2, $3 ~
		const string oldId = format("@%d", i + 1); // @1, @2, @3 ~

		script::g_symbols[oldId] = data;
		script::g_symbols[id] = data;

		++i;
	}


	//------------------------------------------------------------------
	// Parse Mixing Command
	CString mixingCmdText;
	m_EditMixingCommand.GetWindowTextW(mixingCmdText);
	m_mixingParser.ParseStr(wstr2str((LPCTSTR)mixingCmdText));


	//------------------------------------------------------------------
	// Parse Parameter
	m_rollParser.ParseStr(wstr2str((LPCTSTR)m_EditRollCommand));
	m_pitchParser.ParseStr(wstr2str((LPCTSTR)m_EditPitchCommand));
	m_yawParser.ParseStr(wstr2str((LPCTSTR)m_EditYawCommand));
	m_heaveParser.ParseStr(wstr2str((LPCTSTR)m_EditHeaveCommand));
}


void CUDPParseView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePaneChildView::OnSize(nType, cx, cy);

	CRect rcWnd;
	GetWindowRect(&rcWnd);
	HandleAnchors(&rcWnd);
}


void CUDPParseView::OnDestroy()
{
	CDockablePaneChildView::OnDestroy();
}


void CUDPParseView::OnBnClickedCancel()
{
}
void CUDPParseView::OnBnClickedOk()
{
}
void CUDPParseView::OnBnClickedCheckUpdateSymbol()
{
	UpdateData();
}


// UDP Parse를 시작한다.
void CUDPParseView::Start()
{
	UpdateData();

	m_state = START;

	cController::Get()->GetUDPComm().Close();
	
	CString strPort;
	m_EditPort.GetWindowTextW(strPort);
	const int serverPort = _wtoi((LPCTSTR)strPort);

	if (cController::Get()->GetUDPComm().InitServer(serverPort))
	{
		m_ServerStartButton.SetWindowTextW(L"Server Stop");
		g_udpInputView->Start();
	}
	else
	{

	}

	SetBackgroundColor(g_blueColor);
}


// UDP Parse를 종료한다.
void CUDPParseView::Stop()
{
	m_state = STOP;

	cController::Get()->GetUDPComm().Close();
	m_ServerStartButton.SetWindowTextW(L"Server Start");

	g_udpInputView->Stop();

	SetBackgroundColor(g_grayColor);
}


// 매프레임 마다 호출된다.
void CUDPParseView::Update(const float deltaSeconds)
{
	RET(STOP == m_state);

	m_incTime += deltaSeconds;

	if (m_incTime > 0.033f)
	{
		m_interpreter.Excute(m_mixingParser.m_stmt);

		const float roll = m_interpreter.Excute(m_rollParser.m_stmt);
		const float pitch = m_interpreter.Excute(m_pitchParser.m_stmt);
		const float yaw = m_interpreter.Excute(m_yawParser.m_stmt);
		const float heave = m_interpreter.Excute(m_heaveParser.m_stmt);

		// radian normalize
		Quaternion rotr;
		rotr.Euler2(Vector3(roll, 0, 0));
		Quaternion rotp;
		rotp.Euler2(Vector3(0, 0, pitch));
		Quaternion roty;
		roty.Euler2(Vector3(0, yaw, 0));


		Quaternion rot = roty * rotr * rotp;
		Vector3 euler = rot.Euler();
 		const float chRoll = euler.x;
 		const float chYaw = euler.y;
 		const float chPitch = euler.z;
		//

		// update roll-pitch-yaw symbol
		script::sFieldData data;
		data.fVal = chRoll;
		data.type = script::FEILD_TYPE::T_FLOAT;
		script::g_symbols["@roll0"] = data;
		data.fVal = chPitch;
		script::g_symbols["@pitch0"] = data;
		data.fVal = chYaw;
		script::g_symbols["@yaw0"] = data;

		// -pi ~ +pi 내의 각도로 정규화한 각도값(radian) chRoll, chPitch,chYaw 값으로 정보를 업데이트한다.
		cController::Get()->GetCubeFlight().SetEulerAngle(chRoll, chPitch, chYaw);
		cMotionController::Get()->m_udpMod.Update(m_incTime, yaw, chPitch, chRoll, heave);


		if (m_IsUpdateSymbol)
		{
			if (script::g_symbols.size() != m_symbolCount)
			{
				InitSymbolList();
				m_symbolCount = script::g_symbols.size();
			}
			else
			{
				UpdateSymbolList(m_incTime);
			}
		}

		m_incTime = 0;
	}
	
}


// UDP 정보가 올때마다 호출된다.
void CUDPParseView::UpdateUDP(const char *buffer, const int bufferLen)
{
	RET(STOP == m_state);

	//--------------------------------------------------------------------------
	// UDP Packet을 실시간 분석한다.
	int i = 0;
	int index = 0;
	const char *pmem = buffer;
	for each (auto &field in m_protocolParser.m_fields)
	{
		if (index > bufferLen)
			break;

		script::sFieldData data;
		ZeroMemory(data.buff, sizeof(data));
		memcpy(data.buff, pmem, field.bytes);
		data.type = field.type;

		const string id = format("$%d", i + 1); // $1 ,$2, $3 ~
		const string oldId = format("@%d", i + 1); // @1, @2, @3 ~

		script::g_symbols[oldId] = script::g_symbols[id]; // 전 데이타를 저장한다. 
		script::g_symbols[id] = data;

		index += field.bytes;
		pmem += field.bytes;
		++i;
	}
}


// Upate Button Click
void CUDPParseView::OnBnClickedButtonUpdate()
{
	ProcessUIData();
}


void CUDPParseView::OnBnClickedButtonServerStart()
{
	UpdateData();

	if (cController::Get()->GetUDPComm().IsConnect())
	{
		Stop();
	}
	else
	{
		Start();
	}
}


// 심볼리스트를 생성한다.
// 복사본도 같이 생성한다.
void CUDPParseView::InitSymbolList()
{
	m_SymbolList.DeleteAllItems();
	m_symLookup.clear();

	int item = 0;
	for each (auto sym in script::g_symbols)
	{
		m_SymbolList.InsertItem(item, str2wstr(sym.first).c_str());
		m_symLookup[sym.first] = item;
		++item;
	}

	// 똑같은 값을 복사해 놓는다.
	m_cloneSymbols = script::g_symbols;
}


// 심볼리스트 정보를 업데이트 한다.
void CUDPParseView::UpdateSymbolList(const float deltaSeconds)
{
	using namespace script;

	m_incSymbolUpdateTime += deltaSeconds;
	const bool isAllUpdate = m_incSymbolUpdateTime > 3.f;
	if (isAllUpdate)
		m_incSymbolUpdateTime = 0;

	for each (auto sym in script::g_symbols)
	{
		// 그 전 데이타와 다를 때만 업데이트 한다.
		if (!isAllUpdate && (m_cloneSymbols[sym.first] == sym.second))
			continue;

		CString str;
		switch (sym.second.type)
		{
		case FEILD_TYPE::T_BOOL: str = (sym.second.bVal ? L"true" : L"false"); break;
		case FEILD_TYPE::T_UINT:
		case FEILD_TYPE::T_INT: str.Format(L"%d", sym.second.iVal); break;
		case FEILD_TYPE::T_FLOAT: str.Format(L"%f", sym.second.fVal); break;
		case FEILD_TYPE::T_DOUBLE: str.Format(L"%f", sym.second.dVal); break;
		default:
			break;
		}

		const int item = m_symLookup[sym.first];
		m_SymbolList.SetItemText(item, 1, str);
	}
}

