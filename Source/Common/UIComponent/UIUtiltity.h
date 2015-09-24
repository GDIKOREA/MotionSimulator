// UI 에서 공통으로 사용하는 함수들은 모은 유틸리티 코드다.

#pragma once



void ExpandAll(CTreeCtrl &treeCtrl);

HTREEITEM FindTree( CTreeCtrl &treeCtrl, const wstring &text );

void MoveChildCtrlWindow(CWnd &parent, CWnd &wndCtrl, int cx, int cy);

std::string GetIP(CIPAddressCtrl &ipControl);
void SetIP(CIPAddressCtrl &ipControl, const string &ip);


namespace uiutil
{
	int GetProfileInt(const string &appName, const string &keyName, const int defaultValue, const string &fileName);
	float GetProfileFloat(const string &appName, const string &keyName, const float defaultValue, const string &fileName);
	string GetProfileString(const string &appName, const string &keyName, const string &defaultValue, const string &fileName);
}
