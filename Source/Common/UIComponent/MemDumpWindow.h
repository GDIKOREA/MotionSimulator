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
		BYTE c; // 1 ����Ʈ ������ �����Ѵ�.
		bool change; // ������ �ٲ����� true�� �ȴ�. UpdateDump()�Լ����� �Ǻ��Ѵ�.
	};
	vector<sByteData> m_dump; // ����Ʈ ������ �޸𸮸� �����ؼ� �����Ѵ�.

	bool m_isDisplayASCII; // true �̸� ASCII �ڵ� ���, �ƴϸ� Hexa �ڵ� ���, default = false
	bool m_isDisplaySymbol; // true �̸�, �� �޸𸮴� �������� �ɺ��� ����Ѵ�.
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
