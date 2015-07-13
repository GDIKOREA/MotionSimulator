//
// CPlotWindow 를 여러개 출력하는 윈도우 클래스.
//
#pragma once


class CPlotWindow;
class CMultiPlotWindow : public CScrollView
{
public:
	CMultiPlotWindow();
	virtual ~CMultiPlotWindow();


	void SetString(const char *str, const int plotIndex=0);
	void SetXY(const int plotIndex, const float y, const int graphIndex);
	void DrawGraph(const float deltaSeconds);
	void ProcessPlotCommand(const CString &str, const int plotCount = 1);
	void SetFixedWidthMode(const bool isFixedWidth);


protected:
	struct SPlot
	{
		CPlotWindow *wnd;
		string scanString;
	};

	vector<SPlot> m_plotWindows;
	int m_plotWindowCount;

	struct SPlotInfo
	{
		float xRange;
		float yRange;
		float xVisibleRange;
		float yVisibleRange;
		DWORD flags;
		string scanString;
		string name;
	};
	vector<SPlotInfo> m_PlotInfos;



protected:
	CPlotWindow* CreatePlotWindow(const int plotIndex);
	bool ParsePlotInfo(const int plotIndex, const wstring &str, SPlotInfo &out);
	wstring ParseKeyValue(const wstring &str, const wstring &key);
	void CalcGraphWindowSize();


public:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnInitialUpdate();     // first time after construct

	DECLARE_MESSAGE_MAP()
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
