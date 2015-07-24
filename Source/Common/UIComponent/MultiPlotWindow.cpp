
#include "stdafx.h"
#include "MultiPlotWindow.h"



// CSerialGraphForm dialog
CMultiPlotWindow::CMultiPlotWindow()	
{
}

CMultiPlotWindow::~CMultiPlotWindow()
{
}

BEGIN_MESSAGE_MAP(CMultiPlotWindow, CScrollView)
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
END_MESSAGE_MAP()


void CMultiPlotWindow::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
}


void CMultiPlotWindow::OnDraw(CDC* pDC)
{
	
}


// str 정보로 그래프를 업데이트 한다.
// str: 스트링 배열
void CMultiPlotWindow::SetString(const char *str, const int plotIndex)
{
	if (m_plotWindows.empty())
		return;

	for each (auto &plot in m_plotWindows)
	{
		float y;
		const int ret = sscanf_s(str, plot.scanString.c_str(), &y);
		if (ret >= 1)
			plot.wnd->SetPlotY(y, plotIndex);
	}
}


void CMultiPlotWindow::SetXY(const int plotIndex, const float x, const float y, const int graphIndex)
{
	if ((int)m_plotWindows.size() <= plotIndex)
		return;

	m_plotWindows[plotIndex].wnd->SetPlotXY(x, y, graphIndex);
}


void CMultiPlotWindow::SetY(const int plotIndex, const float y, const int graphIndex)
{
	if ((int)m_plotWindows.size() <= plotIndex)
		return;

	m_plotWindows[plotIndex].wnd->SetPlotY(y, graphIndex);
}


// 스트링 str에서 plot 정보를 분석해서 SPlotInfo 구조체에 정보를 저장하고 리턴한다.
// 스트링 분석에 문제가 생기면 false를 리턴하고 종료된다.
bool CMultiPlotWindow::ParsePlotInfo(const int plotIndex, const wstring &str, SPlotInfo &out)
{
	CString plotStr, scanStr, nameStr, modeStr, lineWidthStr;
	plotStr.Format(L"plot%d =", plotIndex + 1);
	scanStr.Format(L"string%d =", plotIndex + 1);
	nameStr.Format(L"name%d =", plotIndex + 1);
	modeStr.Format(L"mode%d =", plotIndex + 1);
	lineWidthStr.Format(L"linewidth%d =", plotIndex + 1);

	// plot graph parameter
	wstring plotParameters = ParseKeyValue(str, plotStr.GetBuffer());
	if (plotParameters.empty())
		return false;

	// scanning string
	wstring scanParameters = ParseKeyValue(str, scanStr.GetBuffer());
	if (scanParameters.empty())
		return false;

	// plot name
	wstring nameParameters = ParseKeyValue(str, nameStr.GetBuffer());
	common::trimw(nameParameters);

	// plot mode
	wstring modeParameters = ParseKeyValue(str, modeStr.GetBuffer());
	common::trimw(modeParameters);

	// line width
	wstring lineWidthParameters = ParseKeyValue(str, lineWidthStr.GetBuffer());
	common::trimw(lineWidthParameters);


	vector<wstring> plotParams;
	common::wtokenizer(plotParameters, L",", L"", plotParams);
	if (plotParams.size() < 5)
		return false;

	out.xRange = (float)_wtof(plotParams[0].c_str());
	out.yRange = (float)_wtof(plotParams[1].c_str());
	out.xVisibleRange = (float)_wtof(plotParams[2].c_str());
	out.yVisibleRange = (float)_wtof(plotParams[3].c_str());
	out.flags = _wtoi(plotParams[4].c_str());

	out.scanString = common::wstr2str(scanParameters);
	out.name = common::wstr2str(nameParameters);
	out.mode = (modeParameters == L"spline") ? CPlotWindow::SPLINE : CPlotWindow::NORMAL;
	out.lineWidth = _wtoi(lineWidthParameters.c_str());

	return true;
}


// key = value1, value2, value3 형태의 스트링을 입력받아
// value1, value2 ~~  문자열을 리턴한다.
// 실패하면 빈 문자열을 리턴한다.
wstring CMultiPlotWindow::ParseKeyValue(const wstring &src, const wstring &key)
{
	const int idx = src.find(key);
	if (idx == string::npos)
		return L"";

	const int beginIdx = src.find(L"=", idx);
	if (beginIdx == string::npos)
		return L"";

	const int endIdx = src.find(L"\n", idx);

	wstring ret = src.substr(beginIdx + 1, endIdx - beginIdx);
	return ret;
}


// 그래프창 크기에 따라 plot window 크기를 조정한다.
void CMultiPlotWindow::CalcGraphWindowSize()
{
	if (GetSafeHwnd() == NULL)
		return;

	CRect cr;
	GetClientRect(cr);

	// 화면에 보여지는 plot 개수를 구한다.
	int plotWindowCount = 0;
	for each (auto &plot in m_plotWindows)
	{
		if (plot.wnd->IsWindowVisible())
			++plotWindowCount;
	}
	if (plotWindowCount <= 0)
		plotWindowCount = m_plotWindows.size(); // 처음 프로그램이 시작될 때, 0 이 된다.

	const int plotSize = plotWindowCount;
	if (plotSize > 0)
	{
		//const int totalHeight = cr.Height() - editCr.Height() - btnR.Height() - 10;
		const int totalHeight = cr.Height();// -10;
		const int plotH = totalHeight / plotSize;

		int plotT = 0;
		for each (auto &it in m_plotWindows)
		{
			if (it.wnd)
				it.wnd->MoveWindow(CRect(0, plotT, cr.Width(), plotT + plotH));
			plotT += plotH;
		}

		// 명령창 과 Update버튼 위치도 따라 바뀐다.
		//m_CommandEditor.MoveWindow(CRect(0, cr.bottom - editCr.Height() - 5, cr.right, cr.bottom - 5));

		RedrawWindow();
	}
}


// Plot Command Editor 의 명령어 문자열을 가져와 실행한다.
void CMultiPlotWindow::ProcessPlotCommand(const CString &str, const int plotCount)
{
	// 명령어 텍스트 가져옴.
// 	CString str;
// 	m_CommandEditor.GetWindowText(str);
	wstring wstr = (LPCTSTR)str;

	// plot1, string1, plot2, string2 ~~ 순서대로 읽어온다. 없다면 종료.
	vector<SPlotInfo> plotInfos;
	plotInfos.reserve(10);
	for (int i = 0; i < 10; ++i)
	{
		SPlotInfo info;
		if (!ParsePlotInfo(i, wstr, info))
			break;
		plotInfos.push_back(info);
	}

	// 명령문을 토대로 plot window를 생성한다.
	for (unsigned int i = 0; i < plotInfos.size(); ++i)
	{
		CPlotWindow *wnd;
		if (m_plotWindows.size() > i)
		{
			wnd = m_plotWindows[i].wnd;
		}
		else
		{
			wnd = CreatePlotWindow(i);
			m_plotWindows.push_back({ wnd, plotInfos[i].scanString });
		}

		// 그래프를 화면에 출력.
		wnd->ShowWindow(SW_SHOW);

		// 그래프 설정.
		wnd->SetPlot(
			plotInfos[i].xRange, plotInfos[i].yRange,
			plotInfos[i].xVisibleRange, plotInfos[i].yVisibleRange, plotInfos[i].flags,
			plotCount, plotInfos[i].name, plotInfos[i].mode, plotInfos[i].lineWidth);

		// 그래프 파싱 스트링 설정.
		m_plotWindows[i].scanString = plotInfos[i].scanString;
	}


	// 나머지 plot window는 hide 시킨다.
	for (unsigned int i = plotInfos.size(); i < m_plotWindows.size(); ++i)
	{
		m_plotWindows[i].wnd->ShowWindow(SW_HIDE);
	}

	m_plotWindowCount = plotInfos.size();

	// 모든 계산이 끝난 후, 창크기를 재조정 한다.
	CalcGraphWindowSize();
}


// 매프레임마다 호출되어야 한다.
// 그래프를 갱신한다.
void CMultiPlotWindow::DrawGraph(const float deltaSeconds, const bool autoSet) // autoSet = true
{
	for each(auto &plot in m_plotWindows)
	{
		plot.wnd->DrawPlot(deltaSeconds, autoSet);
	}
}


CPlotWindow* CMultiPlotWindow::CreatePlotWindow(const int plotIndex)
{
	CRect rect;
	GetClientRect(rect);

	CPlotWindow *plot = new CPlotWindow();
	BOOL result = plot->Create(NULL, NULL, AFX_WS_DEFAULT_VIEW | WS_CHILD,
		CRect(0, 0, rect.Width(), 100), this, AFX_IDW_PANE_FIRST);

	plot->SetScrollSizes(MM_TEXT, CSize(rect.Width() - 30, 100));
	plot->ShowWindow(SW_SHOW);

	plot->SetPlot(0, 0, 0, 0, 0);

	return plot;
}


BOOL CMultiPlotWindow::OnEraseBkgnd(CDC* pDC)
{
	// 백그라운드는 갱신하지 않는다. (속도를 빠르게하기위해)
	//return CScrollView::OnEraseBkgnd(pDC);
	return TRUE;
}


void CMultiPlotWindow::SetFixedWidthMode(const bool isFixedWidth)
{
 	for each(auto &plot in m_plotWindows)
 	{
		plot.wnd->SetFixedWidthMode(isFixedWidth);
 	}
}


void CMultiPlotWindow::OnSize(UINT nType, int cx, int cy)
{
	CScrollView::OnSize(nType, cx, cy);

	if (GetSafeHwnd())
	{
		CalcGraphWindowSize();
	}
}
