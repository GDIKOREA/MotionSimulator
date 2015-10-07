// UI 에서 공통으로 사용하는 함수들은 모은 유틸리티 코드다.

#pragma once


//--------------------------------------------------------------------------------------------------
// Macro

// CWnd* 객체 제거
#define DELETE_WINDOW(window) \
	if (window)\
	{\
		window->DestroyWindow();\
		delete window;\
		window = NULL;\
	}



//--------------------------------------------------------------------------------------------------
// Function

void ExpandAll(CTreeCtrl &treeCtrl);

HTREEITEM FindTree( CTreeCtrl &treeCtrl, const wstring &text );

void MoveChildCtrlWindow(CWnd &parent, CWnd &wndCtrl, int cx, int cy);

std::string GetIP(CIPAddressCtrl &ipControl);
void SetIP(CIPAddressCtrl &ipControl, const string &ip);

class cPlotInputParser;
void ParsePlotInputStringFormat(const string &str, OUT vector<cPlotInputParser> &out);



namespace uiutil
{
	int GetProfileInt(const string &appName, const string &keyName, const int defaultValue, const string &fileName);
	float GetProfileFloat(const string &appName, const string &keyName, const float defaultValue, const string &fileName);
	string GetProfileString(const string &appName, const string &keyName, const string &defaultValue, const string &fileName);
}
