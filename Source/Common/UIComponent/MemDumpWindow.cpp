
#include "stdafx.h"
#include "MemDumpWindow.h"



cMemDumpWindow::cMemDumpWindow()
	: m_bmpSize(0, 0)
	, m_isDisplayASCII(false)
	, m_isDisplaySymbol(false)
	, m_protocol(NULL)
{
	m_brushes[0].CreateSolidBrush(RGB(255, 255, 255));
	m_brushes[1].CreateSolidBrush(RGB(255, 0, 0));
	m_brushes[2].CreateSolidBrush(RGB(0, 0, 0));
}

cMemDumpWindow::~cMemDumpWindow()
{

}


BEGIN_MESSAGE_MAP(cMemDumpWindow, CScrollView)
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
END_MESSAGE_MAP()


void cMemDumpWindow::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	sizeTotal.cx = sizeTotal.cy = 10;
	SetScrollSizes(MM_TEXT, sizeTotal);
}


void cMemDumpWindow::OnDraw(CDC* pDC)
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


	auto oldBrush = memDC.SelectObject(m_brushes[0]);
	const int RW = 30;
	const int RH = 30;
	const int cellPerLine = cr.Width() / RW;
	if (cellPerLine > 0)
	{
		if (m_isDisplaySymbol && m_protocol)
		{
			auto oldObject = memDC.SelectObject(m_brushes[2]);
			memDC.Rectangle(cr);
			memDC.SelectObject(oldObject);

			// 심볼 출력
			int xIdx = 0;
			int yIdx = 0;
			int symIdx = 0;
			for each (auto &field in m_protocol->m_fields)
			{
				int dispBytes = field.bytes;
				while(dispBytes > 0)
				{
					if (cellPerLine <= xIdx)
					{
						xIdx = 0;
						yIdx++;
					}

					const int dispSize = (cellPerLine > xIdx + dispBytes) ? dispBytes : (cellPerLine - xIdx);

					const int x = xIdx * RW;
					const int y = yIdx * RH;
					CRect r(x, y, x + (dispSize*RW), y + RH);
					memDC.Rectangle(r);
					memDC.SetBkColor(RGB(255, 255, 255));
					CString str;
					str.Format(L"$%d", symIdx+1);
					r.OffsetRect(CPoint(0, 10));
					memDC.DrawText(str, r, DT_CENTER);

					xIdx += dispSize;
					dispBytes -= dispSize;
				}

				++symIdx;
			}

		}
		else
		{
			// 메모리 덤프 출력
			for (u_int i = 0; i < m_dump.size(); ++i)
			{
				int x = (i % cellPerLine) * RW;
				int y = (i / cellPerLine) * RH;

				memDC.SelectObject(m_dump[i].change ? m_brushes[1] : m_brushes[0]);
				memDC.Rectangle(CRect(x, y, x + RW, y + RH));

				memDC.SetBkColor(m_dump[i].change? RGB(255,0,0):RGB(255,255,255));
				CString str;
				if (m_isDisplayASCII)
				{
					str.Format(L"%c", (char)m_dump[i].c);
				}
				else
				{
					str.Format(L"%x", m_dump[i].c);
				}
				memDC.TextOutW(x+5, y+10, str);
			}
		}
	}
	memDC.SelectObject(oldBrush);

	pDC->BitBlt(0, 0, cr.Width(), cr.Height(), &memDC, 0, 0, SRCCOPY);
	memDC.SelectObject(pOldBitmap);
	memDC.DeleteDC();
}


BOOL cMemDumpWindow::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}


void cMemDumpWindow::OnSize(UINT nType, int cx, int cy)
{
	CScrollView::OnSize(nType, cx, cy);

}


void cMemDumpWindow::UpdateDump(const char *buff, const int size)
{
	if ((int)m_dump.size() != size)
	{
		if (size > 0)
		{
			m_dump.clear();
			m_dump.resize(size);
		}
	}

	RET(size <= 0);

	for (int i = 0; i < size; ++i)
	{
		m_dump[i].change = (m_dump[i].c != *(unsigned char*)&buff[i]);
		m_dump[i].c = buff[i];
	}

	InvalidateRect(NULL);
}
