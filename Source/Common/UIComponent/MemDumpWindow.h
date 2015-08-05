#pragma once


class cMemDumpWindow : public CScrollView
{
public:
	cMemDumpWindow();
	virtual ~cMemDumpWindow();

	void UpdateDump(const char *buff, const int size);
	void SetDisplayASCII(const bool isASCII);


protected:
	struct sByteData
	{
		BYTE c; // 1 ����Ʈ ������ �����Ѵ�.
		bool change; // ������ �ٲ����� true�� �ȴ�. UpdateDump()�Լ����� �Ǻ��Ѵ�.
	};
	vector<sByteData> m_dump; // ����Ʈ ������ �޸𸮸� �����ؼ� �����Ѵ�.

	bool m_isDisplayASCII; // true �̸� ASCII �ڵ� ���, �ƴϸ� Hexa �ڵ� ���, default = false
	CBitmap m_Bitmap; // double buffering
	CSize m_bmpSize;
	CBrush m_brushes[2]; // white, red

public:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnInitialUpdate();     // first time after construct

	DECLARE_MESSAGE_MAP()
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};


inline void cMemDumpWindow::SetDisplayASCII(const bool isASCII) { m_isDisplayASCII = isASCII;  }
