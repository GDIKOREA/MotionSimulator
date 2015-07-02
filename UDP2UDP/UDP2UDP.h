
// UDP2UDP.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CUDP2UDPApp:
// See UDP2UDP.cpp for the implementation of this class
//

class CUDP2UDPApp : public CWinApp
{
public:
	CUDP2UDPApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CUDP2UDPApp theApp;