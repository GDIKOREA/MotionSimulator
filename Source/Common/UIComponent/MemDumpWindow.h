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
		BYTE c; // 1 바이트 정보를 저장한다.
		bool change; // 정보가 바꼈으면 true가 된다. UpdateDump()함수에서 판별한다.
	};
	vector<sByteData> m_dump; // 바이트 단위로 메모리를 덤프해서 저장한다.

	bool m_isDisplayASCII; // true 이면 ASCII 코드 출력, 아니면 Hexa 코드 출력, default = false
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
