#pragma once


class cProtocolParser;
class cMemDumpWindow : public CScrollView
{
public:
	cMemDumpWindow();
	virtual ~cMemDumpWindow();

	void UpdateDump(const char *buff, const int size);
	void SetDisplayASCII(const bool isASCII);
	void SetDisplaySymbol(const bool isSymbol, cProtocolParser *protocol=NULL);


protected:
	struct sByteData
	{
		BYTE c; // 1 바이트 정보를 저장한다.
		bool change; // 정보가 바꼈으면 true가 된다. UpdateDump()함수에서 판별한다.
	};
	vector<sByteData> m_dump; // 바이트 단위로 메모리를 덤프해서 저장한다.

	bool m_isDisplayASCII; // true 이면 ASCII 코드 출력, 아니면 Hexa 코드 출력, default = false
	bool m_isDisplaySymbol; // true 이면, 각 메모리당 프로토콜 심볼을 출력한다.
	cProtocolParser *m_protocol; // reference
	CBitmap m_Bitmap; // double buffering
	CSize m_bmpSize;
	CBrush m_brushes[3]; // white, red, black


public:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnInitialUpdate();     // first time after construct

	DECLARE_MESSAGE_MAP()
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};


inline void cMemDumpWindow::SetDisplayASCII(const bool isASCII) { m_isDisplayASCII = isASCII;  }
inline void cMemDumpWindow::SetDisplaySymbol(const bool isSymbol, cProtocolParser *protocol) { m_isDisplaySymbol = isSymbol; m_protocol = protocol; }
