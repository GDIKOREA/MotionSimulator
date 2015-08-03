
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
	sizeTotal.cx = sizeTotal.cy = 10;
	SetScrollSizes(MM_TEXT, sizeTotal);
}


void CMultiPlotWindow::OnDraw(CDC* pDC)
{
	
}


// str ������ �׷����� ������Ʈ �Ѵ�.
// str: ��Ʈ�� �迭
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


void CMultiPlotWindow::SetString(const float t, const char *str, const int plotIndex) // plotIndex=0
{
	if (m_plotWindows.empty())
		return;

	for each (auto &plot in m_plotWindows)
	{
		float y;
		const int ret = sscanf_s(str, plot.scanString.c_str(), &y);
		if (ret >= 1)
			plot.wnd->SetPlotXY(t, y, plotIndex);
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


// ��Ʈ�� str���� plot ������ �м��ؼ� SPlotInfo ����ü�� ������ �����ϰ� �����Ѵ�.
// ��Ʈ�� �м��� ������ ����� false�� �����ϰ� ����ȴ�.
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


// key = value1, value2, value3 ������ ��Ʈ���� �Է¹޾�
// value1, value2 ~~  ���ڿ��� �����Ѵ�.
// �����ϸ� �� ���ڿ��� �����Ѵ�.
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


// �׷���â ũ�⿡ ���� plot window ũ�⸦ �����Ѵ�.
void CMultiPlotWindow::CalcGraphWindowSize()
{
	if (GetSafeHwnd() == NULL)
		return;

	CRect cr;
	GetClientRect(cr);

	// ȭ�鿡 �������� plot ������ ���Ѵ�.
	int plotWindowCount = 0;
	for each (auto &plot in m_plotWindows)
	{
		if (plot.wnd->IsWindowVisible())
			++plotWindowCount;
	}
	if (plotWindowCount <= 0)
		plotWindowCount = m_plotWindows.size(); // ó�� ���α׷��� ���۵� ��, 0 �� �ȴ�.

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

		// ���â �� Update��ư ��ġ�� ���� �ٲ��.
		//m_CommandEditor.MoveWindow(CRect(0, cr.bottom - editCr.Height() - 5, cr.right, cr.bottom - 5));

		RedrawWindow();
	}
}


// Plot Command Editor �� ��ɾ� ���ڿ��� ������ �����Ѵ�.
void CMultiPlotWindow::ProcessPlotCommand(const CString &str, const int plotCount)
{
	// ��ɾ� �ؽ�Ʈ ������.
// 	CString str;
// 	m_CommandEditor.GetWindowText(str);
	wstring wstr = (LPCTSTR)str;

	// plot1, string1, plot2, string2 ~~ ������� �о�´�. ���ٸ� ����.
	vector<SPlotInfo> plotInfos;
	plotInfos.reserve(10);
	for (int i = 0; i < 10; ++i)
	{
		SPlotInfo info;
		if (!ParsePlotInfo(i, wstr, info))
			break;
		plotInfos.push_back(info);
	}

	// ��ɹ��� ���� plot window�� �����Ѵ�.
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

		// �׷����� ȭ�鿡 ���.
		wnd->ShowWindow(SW_SHOW);

		// �׷��� ����.
		wnd->SetPlot(
			plotInfos[i].xRange, plotInfos[i].yRange,
			plotInfos[i].xVisibleRange, plotInfos[i].yVisibleRange, plotInfos[i].flags,
			plotCount, plotInfos[i].name, plotInfos[i].mode, plotInfos[i].lineWidth);

		// �׷��� �Ľ� ��Ʈ�� ����.
		m_plotWindows[i].scanString = plotInfos[i].scanString;
	}


	// ������ plot window�� hide ��Ų��.
	for (unsigned int i = plotInfos.size(); i < m_plotWindows.size(); ++i)
	{
		m_plotWindows[i].wnd->ShowWindow(SW_HIDE);
	}

	m_plotWindowCount = plotInfos.size();

	// ��� ����� ���� ��, âũ�⸦ ������ �Ѵ�.
	CalcGraphWindowSize();
}


// �������Ӹ��� ȣ��Ǿ�� �Ѵ�.
// �׷����� �����Ѵ�.
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

	plot->SetScrollSizes(MM_TEXT, CSize(10, 10));
	plot->ShowWindow(SW_SHOW);

	plot->SetPlot(0, 0, 0, 0, 0);

	return plot;
}


BOOL CMultiPlotWindow::OnEraseBkgnd(CDC* pDC)
{
	// ��׶���� �������� �ʴ´�. (�ӵ��� �������ϱ�����)
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
