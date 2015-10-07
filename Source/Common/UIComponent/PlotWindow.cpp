// PlotWindow.cpp : implementation file
//

#include "stdafx.h"
#include "PlotWindow.h"

COLORREF g_penColors[] = {
	RGB(255, 255, 0),
	RGB(0, 255, 0),
	RGB(0, 255, 255),
	RGB(255, 255, 255),
};


// CPlotWindow
CPlotWindow::CPlotWindow() :
	m_mode(NORMAL)
	, m_splineIncTime(0)
	, m_oldTime(0)
	, m_bmpSize(0,0)
{
	m_blackBrush.CreateSolidBrush(RGB(0, 0, 0));
	m_gridPen1.CreatePen(0, 1, RGB(100, 100, 100));
	m_gridPen2.CreatePen(0, 2, RGB(100,100,100));

}

CPlotWindow::~CPlotWindow()
{
}


BEGIN_MESSAGE_MAP(CPlotWindow, CScrollView)
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CPlotWindow drawing

void CPlotWindow::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	sizeTotal.cx = sizeTotal.cy = 10;
	SetScrollSizes(MM_TEXT, sizeTotal);
}


// �׷��� ���.
void CPlotWindow::OnDraw(CDC* pDC)
{
	CRect cr;
	GetClientRect(cr);


	CDC memDC;
	CBitmap *pOldBitmap;
	memDC.CreateCompatibleDC(pDC);
	if ((m_bmpSize.cx != cr.Width()) || (m_bmpSize.cy != cr.Height()))
	{
		m_Bitmap.DeleteObject();
		m_Bitmap.CreateCompatibleBitmap(pDC, cr.Width(), cr.Height());
		m_bmpSize = CSize(cr.Width(), cr.Height());
	}

	pOldBitmap = memDC.SelectObject(&m_Bitmap);
	//memDC.PatBlt(0, 0, rect.Width(), rect.Height(), WHITENESS); // ������� �ʱ�ȭ
	// �޸� DC�� �׸���
	//DrawImage(&memDC);
	// �޸� DC�� ȭ�� DC�� ��� ����
	//pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);

// 	memDC.SelectObject(pOldBitmap);
// 	memDC.DeleteDC();
// 	bitmap.DeleteObject(






	// background black
	memDC.SelectObject(m_blackBrush);
	memDC.Rectangle(cr);

	// draw x,y axis grid
	memDC.SelectObject(m_gridPen2); // center line width 2
	memDC.MoveTo(CPoint(0, cr.Height() / 2));
	memDC.LineTo(CPoint(cr.Width(), cr.Height() / 2));
	memDC.SelectObject(m_gridPen1); 
	memDC.MoveTo(CPoint(0, cr.Height() / 4 ));
	memDC.LineTo(CPoint(cr.Width(), cr.Height() / 4));
	memDC.MoveTo(CPoint(0, cr.Height()* 3 / 4));
	memDC.LineTo(CPoint(cr.Width(), cr.Height()*3 / 4));


	// draw time line grid
	float TOTAL_DRAW_TIMELINE = 0;
	int oneSecondsWidth = 0; // 1�ʴ� �ȼ� ����

	// 1�ʴ� �ȼ����� ������ ����� ���.
	if (m_isFixedPlot)
	{
		oneSecondsWidth = 100; // 1�ʴ� �ȼ� ������ 100���� ������Ų��.
		TOTAL_DRAW_TIMELINE = (float)cr.Width() / (float)oneSecondsWidth;
		m_plots[0].renderStartIndex = GetDrawStartIndex(0, m_plots[0].renderStartIndex, TOTAL_DRAW_TIMELINE-1); // Ÿ�Ӷ��κ��� 1�� ���� ����ؼ�, �׷��� ���� ������ �д�.
	}
	else
	{ // ȭ�鿡 ����� �׷����� �ð��� ������ų ���.
		TOTAL_DRAW_TIMELINE = 5.f; // �� ȭ�鿡 5���� �׷����� ����ϰ� �Ѵ�.
		oneSecondsWidth = (int)((float)cr.Width() / TOTAL_DRAW_TIMELINE); // 1�ʴ� �ȼ� ����
		m_plots[0].renderStartIndex = GetDrawStartIndex(0, m_plots[0].renderStartIndex, TOTAL_DRAW_TIMELINE-1);// Ÿ�Ӷ��κ��� 1�� ���� ����ؼ�, �׷��� ���� ������ �д�.
	}

	const float timeElapse = m_plots[0].xy[m_plots[0].renderStartIndex].x - m_startTime; // ������ �ð���ŭ timeline�� �̵���Ų��.
	const float renderStartX = m_plots[0].xy[m_plots[0].renderStartIndex].x;

	if (oneSecondsWidth == 0)
		return;

	// ���̰� ���� �ð� ���ݸ�ŭ �ȼ��� ����Ѵ�. 1�� ������ ���ݸ� �˸�ȴ�.
	const int offsetX = ((int)(timeElapse * (float)oneSecondsWidth)) % oneSecondsWidth;
	const int timeLineCount = cr.Width() / oneSecondsWidth;
	for (int i = 1; i <= timeLineCount + 1; ++i)
	{
		memDC.MoveTo(CPoint((i * oneSecondsWidth) - offsetX, 0));
		memDC.LineTo(CPoint((i * oneSecondsWidth) - offsetX, cr.Height()));
	}

	m_scaleY = (float)cr.Height() / (m_maxY - m_minY);
	m_scaleX = oneSecondsWidth / 1.f; // 1�� �� �ȼ��� //(float)(cr.Width() - 50) / (float)m_plot.size();


	for (u_int i = 0; i < m_plots.size(); ++i)
	{
		sPlotData &plot = m_plots[i];

		// 1�ʴ� �ȼ����� ������ ����� ���.
		if (m_isFixedPlot)
		{
			oneSecondsWidth = 100; // 1�ʴ� �ȼ� ������ 100���� ������Ų��.
			TOTAL_DRAW_TIMELINE = (float)cr.Width() / (float)oneSecondsWidth;
			plot.renderStartIndex = GetDrawStartIndex(i, plot.renderStartIndex, TOTAL_DRAW_TIMELINE);
		}
		else
		{ // ȭ�鿡 ����� �׷����� �ð��� ������ų ���.
			TOTAL_DRAW_TIMELINE = 5.f; // �� ȭ�鿡 5���� �׷����� ����ϰ� �Ѵ�.
			oneSecondsWidth = (int)((float)cr.Width() / TOTAL_DRAW_TIMELINE); // 1�ʴ� �ȼ� ����
			plot.renderStartIndex = GetDrawStartIndex(i, plot.renderStartIndex, TOTAL_DRAW_TIMELINE);
		}

		// draw graph
		memDC.SelectObject(m_plotPens[ i]);
		const int h = cr.Height();
		int lastX = 0;
		for (int i = plot.renderStartIndex; i != plot.tailIdx; i = ++i % plot.xy.size())
		{
			//const int x = (int)((plot.xy[i].first - plot.xy[plot.renderStartIndex].first) * m_scaleX) - 50;
			const int x = (int)((plot.xy[i].x - renderStartX) * m_scaleX);// -50;
			const int y = h - (int)((plot.xy[i].y - m_minY) * m_scaleY);
			lastX = x;

			if (i == plot.renderStartIndex)
				memDC.MoveTo(CPoint(x, y));
			else
				memDC.LineTo(CPoint(x, y));
		}


		// ���� ���. �ִ�, �ּ�, �߰� ��, ���� ��.
		memDC.SetBkMode(TRANSPARENT);
		memDC.SetTextColor(RGB(220, 220, 220));

		// ���� ������ ��ġ�� ���� ���� ����Ѵ�.
		if (plot.headIdx != plot.tailIdx)
		{
			int y = h - (int)((plot.xy[(plot.tailIdx - 1) % plot.xy.size()].y - m_minY) * m_scaleY);
			if (cr.Height() < (y + 20)) {
				y -= 20;
			}
			else {
				y += 10;
			}

			CString strLastVal;
			strLastVal.Format(L"%f", plot.xy[(plot.tailIdx - 1) % plot.xy.size()].y);
			memDC.TextOutW(lastX - 10, y, strLastVal);
		}
	}


	CString strMaxY, strMinY, strCenterY;
	strMaxY.Format(L"%f", m_maxY);
	strMinY.Format(L"%f", m_minY);
	strCenterY.Format(L"%f", (m_maxY + m_minY) / 2.f);
	memDC.TextOutW(5, 0, strMaxY); // maximum y
	memDC.TextOutW(5, cr.Height() - 20, strMinY); // minimum y
	memDC.TextOutW(5, cr.Height() / 2 - 20, strCenterY); // center y
	memDC.TextOutW(cr.Width()/2, 0, m_name); // plot name

	pDC->BitBlt(0, 0, cr.Width(), cr.Height(), &memDC, 0, 0, SRCCOPY);
	memDC.SelectObject(pOldBitmap);
	memDC.DeleteDC();
}


// Plot�� Frame per Seconds�� ���� ����ϰ� �Ѵ�.
void CPlotWindow::DrawPlot(const float deltaSeconds, const bool autoSet) //autoSet=true
{
	const float elapseTime = 0.03f; // 1�ʿ� 30�������� ��, �ð� ����

	// ���� �ֱٿ� ������Ʈ �� �ð��� ����ð��� ���ؼ�, 
	// �����ð� �̻� �׷����� ������Ʈ�� ���� �ʾҴٸ�,
	// ���� �ֱ� ������, �׷��� ���� �߰��Ѵ�.
	const float curT = timeGetTime() * 0.001f;
	if (autoSet && (curT - m_updateTime) > elapseTime) // 30�������ֱ�� �׷����� ������Ʈ �Ѵ�.
	{
		// ���� �ֱ� ���� �����Ѵ�.
		for (u_int i = 0; i < m_plots.size(); ++i)
			SetPlotXY(curT, GetTailValue(i).y, i);
	}

	m_incSeconds += deltaSeconds;
	if (m_incSeconds > elapseTime) // 1�ʿ� 30�������� �����Ѵ�.
	{
		if (IsWindowVisible())
			InvalidateRect(NULL);
		m_incSeconds = 0;
	}
}


// CPlotWindow diagnostics

#ifdef _DEBUG
void CPlotWindow::AssertValid() const
{
	CScrollView::AssertValid();
}

#ifndef _WIN32_WCE
void CPlotWindow::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif
#endif //_DEBUG


// CPlotWindow message handlers
bool CPlotWindow::SetPlot(const float x_range, const float y_range,
	const float x_visble_range, const float y_visible_range, const DWORD flags,
	const int plotCount, const string &name, const MODE &mode, const int lineWidth)  // plotCount=1, name="", mode=NORMAL, lineWidth=1
{
	m_xRange = x_range;
	m_yRange = y_range;
	m_xVisibleRange = x_visble_range;
	m_yVisibleRange = y_visible_range;
	m_flags = flags;
	m_mode = mode;
	m_name = common::str2wstr(name).c_str();

	m_plots.resize(plotCount);
	for (u_int i = 0; i < m_plots.size(); ++i)
	{
		m_plots[i].xy.resize(2048, Vector2(0.f, 0.f));
		m_plots[i].headIdx = 0;
		m_plots[i].tailIdx = 0;
		m_plots[i].renderStartIndex = 0;

		m_plots[i].splineTemp.resize(2048, Vector2(0.f, 0.f));
		m_plots[i].spHeadIdx = 0;
		m_plots[i].spTailIdx = 0;
	}

	m_maxX = -FLT_MAX;
	m_minX = FLT_MAX;

	if (0 == m_yRange)
	{
		m_maxY = -FLT_MAX;
		m_minY = FLT_MAX;
	}
	else
	{
		m_maxY = m_yRange/2;
		m_minY = -m_yRange/2;
	}

	m_scaleY = 1.f;
	m_startTime = 0;
	m_updateTime = 0;

	for (int i = 0; i < 4; ++i)
	{
		m_plotPens[i].DeleteObject();
		m_plotPens[i].CreatePen(0, lineWidth, g_penColors[i]);
	}


	return true;
}


// �׷��� ���� �߰�.
void CPlotWindow::SetPlotY(const float y, const int plotIndex)
{
	// X���� �ð������� �Ѵ�. 
	const float t = timeGetTime() * 0.001f;
	const float deltaSeconds = (t - m_oldTime);
	m_oldTime = t;
	m_splineIncTime += deltaSeconds;

	sPlotData &plot = m_plots[plotIndex];

	if ((SPLINE == m_mode) && (plotIndex == 1))
	{
		if (m_splineIncTime < 0.1f)
			return;
		
		m_splineIncTime = 0;

		// ���� �� ����
		const int size = plot.splineTemp.size();

		// Ring�� �迭 ����, �߰�
		plot.splineTemp[plot.spTailIdx] = Vector2(t, y);
		plot.spTailIdx = ++plot.spTailIdx % size;
		if (plot.spHeadIdx == plot.spTailIdx)
			plot.spHeadIdx = ++plot.spHeadIdx % size;

		const int indexGap = 1;

		// ���� �ֱٿ� ����� 3���� ����Ʈ�� ã�´�.
		// Vector2(x,y)�� ������ �� 4���� ����Ʈ�� spline ��� �����.
		// ���� ����Ʈ�� �� �� ����Ʈ�� �߰��� spline � �˰����� ���� ����� ��, �����Ѵ�.
		const int idx4 = (plot.spTailIdx - 1 + size) % size;
		const int idx3 = (plot.spTailIdx - (indexGap*1 + 1) + size) % size;
		const int idx2 = (plot.spTailIdx - (indexGap*2 + 1) + size) % size;
		const int idx1 = (plot.spTailIdx - (indexGap*3 + 1) + size) % size;
		const int length = (plot.spTailIdx - plot.spHeadIdx + size) % size;
		if (length > (indexGap*3 + 1)) // 4�� �̻� ���� �Ǿ��� ������, spline������ �����Ѵ�.
		{
			const Vector2 p1 = plot.splineTemp[idx1];
			const Vector2 p2 = plot.splineTemp[idx2];
			const Vector2 p3 = plot.splineTemp[idx3];
			const Vector2 p4 = plot.splineTemp[idx4];

			float t = 0.0f;
			while (t < 1.f)
			{
				Vector2 p;
				D3DXVec2CatmullRom((D3DXVECTOR2*)&p,
					(D3DXVECTOR2*)&p1,
					(D3DXVECTOR2*)&p2,
					(D3DXVECTOR2*)&p3,
					(D3DXVECTOR2*)&p4, t);

				// insert spline curve b3
				SetPlotXY(p.x, p.y, plotIndex);

				t += .1f;
			}
		}
		else
		{
			SetPlotXY(t, y, plotIndex);
		}

	}
	else
	{
		SetPlotXY(t, y, plotIndex);
	}

}


// Ring�� �迭 ����, �߰�
void CPlotWindow::SetPlotXY(const float x, const float y, const int plotIndex) //plotIndex=0
{
	if ((int)m_plots.size() <= plotIndex)
		return;

	sPlotData &plot = m_plots[plotIndex];
	plot.xy[plot.tailIdx] = Vector2(x, y);
	plot.tailIdx = ++plot.tailIdx % plot.xy.size();
	if (plot.headIdx == plot.tailIdx)
		plot.headIdx = ++plot.headIdx % plot.xy.size();

	m_maxX = max(x, m_maxX);
	m_minX = min(x, m_minX);
	m_maxY = max(y, m_maxY);
	m_minY = min(y, m_minY);

	const float t = timeGetTime() * 0.001f;
	m_updateTime = t; // ���� �ֱ� ���� �ٲ� �ð��� �����Ѵ�.

	if (m_startTime == 0) // ó�� �׷����� �׷��� ��, �ʱ�ȭ �ȴ�.
		m_startTime = x;
}


void CPlotWindow::OnSize(UINT nType, int cx, int cy)
{
	CScrollView::OnSize(nType, cx, cy);
}


BOOL CPlotWindow::OnEraseBkgnd(CDC* pDC)
{
	//return CScrollView::OnEraseBkgnd(pDC); ��׶��� ȭ���� �������� �ʴ´�.
	return TRUE;
}


// ���� ���� ����� ���� ���� �Ѵ�. (���� ������ ��)
Vector2 CPlotWindow::GetHeadValue(const u_int plotIndex) const
{
	const sPlotData &plot = m_plots[plotIndex];

	if (plot.headIdx < plot.tailIdx)
	{
		return plot.xy[plot.headIdx];
	}
	else
	{
		const int idx = (plot.xy.size() + plot.headIdx - 1) % plot.xy.size();
		return plot.xy[idx];
	}
}


// ���� ���߿� ���� �� ���� �����Ѵ�. (���� �ֱ� ��)
Vector2 CPlotWindow::GetTailValue(const u_int plotIndex) const
{
	const sPlotData &plot = m_plots[plotIndex];
	const int idx = (plot.xy.size() + plot.tailIdx - 1) % plot.xy.size();
	return plot.xy[idx];
}


// ���� plot index ���� tail ���� �Ѿư��鼭 drawTimeRange ������ �ȿ� ��� 
// ù ��° plot index �� ã�� �����Ѵ�.
// �׷����� ��Ȯ�� drawTimeRange ��ŭ ����ϱ� ���� �Լ���.
int CPlotWindow::GetDrawStartIndex(const u_int plotIndex, const int currentIndex, const float drawTimeRange)
{
	const float tailSeconds = GetTailValue(plotIndex).x;

	for (int i = currentIndex; i != m_plots[plotIndex].tailIdx; i = ++i % m_plots[plotIndex].xy.size())
	{
		const float totalDrawTime = tailSeconds - m_plots[plotIndex].xy[i].x;
		if (totalDrawTime < drawTimeRange)
			return i;
	}

	// ã�� ���ߴٸ�, ���� �ε����� �����Ѵ�.
	return currentIndex;
}
