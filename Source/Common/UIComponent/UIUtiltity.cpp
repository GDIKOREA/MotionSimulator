
#include "stdafx.h"
#include "UIUtiltity.h"
#include <sstream>

using namespace uiutil;



// 모든 트리노드를 펼친다.
void ExpandAll(CTreeCtrl &treeCtrl)
{
	HTREEITEM hRoot = treeCtrl.GetRootItem();
	vector<HTREEITEM> items;
	items.reserve(treeCtrl.GetCount());

	items.push_back(hRoot);

	while (!items.empty())
	{
		HTREEITEM hItem = items.back();
		items.pop_back();
		treeCtrl.Expand(hItem, TVE_EXPAND);

		HTREEITEM hfirstChild = treeCtrl.GetChildItem(hItem);
		if (hfirstChild)
		{
			items.push_back(hfirstChild);

			while (HTREEITEM hNextItem = treeCtrl.GetNextSiblingItem(hfirstChild))
			{
				items.push_back(hNextItem);
				hfirstChild = hNextItem;
			}
		}
	}

	treeCtrl.SelectSetFirstVisible(hRoot);
}


HTREEITEM FindTree( CTreeCtrl &treeCtrl, const wstring &findText )
{
	HTREEITEM hRoot = treeCtrl.GetRootItem();
	vector<HTREEITEM> items;
	items.reserve(treeCtrl.GetCount());

	wstring lowerText = findText;
	lowerCasew(lowerText);

	items.push_back(hRoot);

	while (!items.empty())
	{
		HTREEITEM hItem = items.back();
		items.pop_back();
		CString itemText = treeCtrl.GetItemText(hItem);
		CString lowerItemText = lowerCasew((wstring)itemText).c_str();

		const int idx = lowerItemText.Find(lowerText.c_str());
		if (idx >= 0)
			return hItem;

		HTREEITEM hfirstChild = treeCtrl.GetChildItem(hItem);
		if (hfirstChild)
		{
			items.push_back(hfirstChild);

			while (HTREEITEM hNextItem = treeCtrl.GetNextSiblingItem(hfirstChild))
			{
				items.push_back(hNextItem);
				hfirstChild = hNextItem;
			}
		}
	}

	return NULL;
}


void MoveChildCtrlWindow(CWnd &parent, CWnd &wndCtrl, int cx, int cy)
{
	if (wndCtrl.GetSafeHwnd())
	{
		CRect wr;
		wndCtrl.GetWindowRect(wr);
		parent.ScreenToClient(wr);
		wndCtrl.MoveWindow(wr.left, wr.top, cx, wr.Height());
	}
}


// IPAddressCtrl의 IP정보를 문자열로 리턴한다.
string GetIP(CIPAddressCtrl &ipControl)
{
	DWORD address;
	ipControl.GetAddress(address);
	std::stringstream ss;
	ss << ((address & 0xff000000) >> 24) << "."
		<< ((address & 0x00ff0000) >> 16) << "."
		<< ((address & 0x0000ff00) >> 8) << "."
		<< (address & 0x000000ff);
	const string ip = ss.str();
	return ip;
}


// 문자열 IP정보를 CIPAddressCtrl 에 저장한다.
void SetIP(CIPAddressCtrl &ipControl, const string &ip)
{
	vector<string> ipnums;
	tokenizer(ip, ".", "", ipnums);
	if (ipnums.size() >= 4)
	{
		ipControl.SetAddress(atoi(ipnums[0].c_str()),
			atoi(ipnums[1].c_str()),
			atoi(ipnums[2].c_str()),
			atoi(ipnums[3].c_str()));
	}
	else
	{
		ipControl.SetAddress(127, 0, 0, 1);
	}
}


int uiutil::GetProfileInt(const string &appName, const string &keyName, const int defaultValue, const string &fileName)
{
	TCHAR buff[128];
	const int ret = GetPrivateProfileString(
		str2wstr(appName).c_str(),
		str2wstr(keyName).c_str(),
		formatw("%d", defaultValue).c_str(),
		buff,
		(DWORD)sizeof(buff),
		str2wstr(fileName).c_str());

	return _wtoi(buff);
}


float uiutil::GetProfileFloat(const string &appName, const string &keyName, const float defaultValue, const string &fileName)
{
	TCHAR buff[128];
	const int ret = GetPrivateProfileString(
		str2wstr(appName).c_str(),
		str2wstr(keyName).c_str(),
		formatw("%f",defaultValue).c_str(),
		buff,
		(DWORD)sizeof(buff),
		str2wstr(fileName).c_str());

	return (float)_wtof(buff);
}


string uiutil::GetProfileString(const string &appName, const string &keyName, const string &defaultValue, const string &fileName)
{
	TCHAR buff[128];
	const int ret = GetPrivateProfileString(
		str2wstr(appName).c_str(),
		str2wstr(keyName).c_str(),
		str2wstr(defaultValue).c_str(),
		buff,
		(DWORD)sizeof(buff),
		str2wstr(fileName).c_str());

	return wstr2str(buff);
}

