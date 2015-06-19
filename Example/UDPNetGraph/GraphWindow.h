#pragma once


// CGraphWindow dialog
class CPlotWindow;
class CGraphWindow : public CDialogEx
{
	DECLARE_DYNAMIC(CGraphWindow)

public:
	CGraphWindow(CWnd* pParent = NULL);   // standard constructor
	virtual ~CGraphWindow();

// Dialog Data
	enum { IDD = IDD_DIALOG1 };


	void Update(const float deltaSeconds);
	void SetGraphValue(const string &str);
	void SetGraphValue2(float v);

	CPlotWindow *m_plotP;
	float m_incSeconds;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
