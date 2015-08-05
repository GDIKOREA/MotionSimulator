// MixingView.cpp : implementation file
//

#include "stdafx.h"
#include "UDPAnalyzer.h"
#include "MixingView.h"
#include "afxdialogex.h"


// CMixingView dialog
CMixingView::CMixingView(CWnd* pParent /*=NULL*/)
	: CDockablePaneChildView(CMixingView::IDD, pParent)
	, m_incTime(0)
	, m_IsUpdateSymbolList(TRUE)
	, m_symbolCount(0)
{

}

CMixingView::~CMixingView()
{
}

void CMixingView::DoDataExchange(CDataExchange* pDX)
{
	CDockablePaneChildView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_COMMAND, m_CommandEditor);
	DDX_Control(pDX, IDC_LIST_SYMBOL, m_SymbolList);
	DDX_Check(pDX, IDC_CHECK_SYMBOL, m_IsUpdateSymbolList);
}

BEGIN_ANCHOR_MAP(CMixingView)
	ANCHOR_MAP_ENTRY(IDC_EDIT_COMMAND, ANF_LEFT | ANF_RIGHT | ANF_TOP)
	ANCHOR_MAP_ENTRY(IDC_LIST_SYMBOL, ANF_LEFT | ANF_RIGHT | ANF_TOP | ANF_BOTTOM)
END_ANCHOR_MAP()

BEGIN_MESSAGE_MAP(CMixingView, CDockablePaneChildView)
	ON_BN_CLICKED(IDOK, &CMixingView::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CMixingView::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_UPDATE, &CMixingView::OnBnClickedButtonUpdate)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_CHECK_SYMBOL, &CMixingView::OnBnClickedCheckSymbol)
END_MESSAGE_MAP()


// CMixingView message handlers

BOOL CMixingView::OnInitDialog()
{
	CDockablePaneChildView::OnInitDialog();

	InitAnchors();

	CString command =
		L"$var1 = $1 + $2 + $3\r\n"
		L"$var2 = $1 + $2 + $3\r\n"
		L"$var3 = $1 + $2 + $3\r\n";

	CString cmdStr;
	std::ifstream cfgfile("udpanalyzer_mixing.cfg");
	if (cfgfile.is_open())
	{
		std::string str((std::istreambuf_iterator<char>(cfgfile)), std::istreambuf_iterator<char>());
		cmdStr = str2wstr(str).c_str();
	}
	else
	{
		cmdStr = command;
	}
	m_CommandEditor.SetWindowTextW(cmdStr);


	m_SymbolList.InsertColumn(0, L"Name");
	m_SymbolList.InsertColumn(1, L"Value");
	m_SymbolList.SetColumnWidth(0, 50);
	m_SymbolList.SetColumnWidth(1, 200);

	return TRUE;
}


void CMixingView::OnBnClickedOk()
{
}


void CMixingView::OnBnClickedCancel()
{
}


void CMixingView::OnBnClickedButtonUpdate()
{
	UpdateData();

	CString text;
	m_CommandEditor.GetWindowTextW(text);
	m_parser.ParseStr(wstr2str((LPCTSTR)text));

	script::ClearSymbols();
}


void CMixingView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePaneChildView::OnSize(nType, cx, cy);

	CRect rcWnd;
	GetWindowRect(&rcWnd);
	HandleAnchors(&rcWnd);
}


void CMixingView::Update(const float deltaSeconds)
{
	m_incTime += deltaSeconds;

	if (m_incTime > 0.033f)
	{
		m_interpreter.Excute(m_parser.m_stmt);


		// 심볼 정보 리스트를 업데이트 한다.
		if (m_IsUpdateSymbolList)
		{
			if (script::g_symbols.size() != m_symbolCount)
			{
				InitSymbolList();
				m_symbolCount = script::g_symbols.size();
			}
			else
			{
				UpdateSymbolList();
			}
		}

		m_incTime = 0;
	}

}


void CMixingView::OnDestroy()
{
	UpdateData();

	// 환경파일 저장
	std::ofstream cfgfile("udpanalyzer_mixing.cfg");
	if (cfgfile.is_open())
	{
		CString command;
		m_CommandEditor.GetWindowTextW(command);
		string str = wstr2str((LPCTSTR)command);
		cfgfile << str;
	}

	CDockablePaneChildView::OnDestroy();
}


void CMixingView::OnBnClickedCheckSymbol()
{
	UpdateData();	
}


// 심볼리스트를 생성한다.
// 복사본도 같이 생성한다.
void CMixingView::InitSymbolList()
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


void CMixingView::UpdateSymbolList()
{
	using namespace script;
	
	for each (auto sym in script::g_symbols)
	{
		// 그 전 데이타와 다를 때만 업데이트 한다.
		if (m_cloneSymbols[sym.first] == sym.second)
			continue;

		CString str;
		switch (sym.second.type)
		{
		case FEILD_TYPE::T_BOOL: str = (sym.second.bVal ? L"true" : L"false"); break;
		case FEILD_TYPE::T_UINT:
		case FEILD_TYPE::T_INT: str.Format(L"%d", sym.second.iVal); break;
		case FEILD_TYPE::T_FLOAT: str.Format(L"%f", sym.second.fVal); break;
		default:
			break;
		}

		const int item = m_symLookup[sym.first];
		m_SymbolList.SetItemText(item, 1, str);
	}

}
