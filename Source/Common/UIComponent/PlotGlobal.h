
// CPlotWindow, CMultPlotWindow ���� �������� ���̴� �͵��� �����Ѵ�.
#pragma once


namespace plot
{

	enum MODE { NORMAL, SPLINE, };

	struct SPlotInfo
	{
		float xRange;
		float yRange;
		float xVisibleRange;
		float yVisibleRange;
		DWORD flags;
		string scanString;
		string name;
		MODE mode;
		int lineWidth;
		int timeLine;
		int vectorSize;
		int plotCount;
	};


}
