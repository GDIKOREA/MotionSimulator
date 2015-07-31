
// UDPAnalyzer.h : main header file for the UDPAnalyzer application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CUDPAnalyzerApp:
// See UDPAnalyzer.cpp for the implementation of this class
//

class CUDPAnalyzerApp : public CWinAppEx
{
public:
	CUDPAnalyzerApp();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnIdle(LONG lCount);
};

extern CUDPAnalyzerApp theApp;
