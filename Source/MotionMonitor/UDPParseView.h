#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "protocolparser.h"
#include "mathinterpreter.h"
#include "mathparser.h"


// CUDPParseView dialog

class CUDPParseView : public CDockablePaneChildView
{
public:
	CUDPParseView(CWnd* pParent = NULL);   // standard constructor
	virtual ~CUDPParseView();

// Dialog Data
	enum { IDD = IDD_DIALOG_UDPPARSE };

	virtual void UpdateConfig(bool IsSaveAndValidate = true) override;
	virtual void Start() override;
	virtual void Stop() override;

	virtual void Update(const float deltaSeconds) override;
	virtual void UpdateUDP(const char *buffer, const int bufferLen) override;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void InitSymbolList();
	void UpdateSymbolList(const float deltaSeconds);
	void ProcessUIData();


	enum STATE {
		STOP,
		START,
	};

	STATE m_state;
	float m_incTime;
	cProtocolParser m_protocolParser;
	cMathParser m_mixingParser;
	cMathParser m_rollParser;
	cMathParser m_pitchParser;
	cMathParser m_yawParser;
	cMathParser m_heaveParser;
	mathscript::cMathInterpreter m_interpreter;

	int m_symbolCount; // 이 값과 현재 심볼 개수와 다르다면, SymbolList를 업데이트 한다.
	map<string, int> m_symLookup;
	map<string, script::sFieldData> m_cloneSymbols;
	float m_incSymbolUpdateTime;


	DECLARE_ANCHOR_MAP();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	CEdit m_EditProtocolCommand;
	CEdit m_EditMixingCommand;
	BOOL m_IsUpdateSymbol;
	CListCtrl m_SymbolList;
	afx_msg void OnBnClickedButtonUpdate();
	CString m_EditRollCommand;
	CString m_EditPitchCommand;
	CString m_EditYawCommand;
	CString m_EditHeaveCommand;
	CEdit m_EditPort;
	afx_msg void OnBnClickedButtonServerStart();
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	CButton m_ServerStartButton;
	afx_msg void OnBnClickedCheckUpdateSymbol();
};
