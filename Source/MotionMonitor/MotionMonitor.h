
// MotionMonitor.h : main header file for the MotionMonitor application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CMotionMonitorApp:
// See MotionMonitor.cpp for the implementation of this class
//

class CMotionMonitorApp : public CWinAppEx
{
public:
	CMotionMonitorApp();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;
	ULONG_PTR m_gdiplusToken;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnIdle(LONG lCount);
	afx_msg void OnFileOpen();
};

extern CMotionMonitorApp theApp;
