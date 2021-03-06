
// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Exclude rarely-used stuff from Windows headers
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit

// turns off MFC's hiding of some common and often safely ignored warning messages
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions


#include <afxdisp.h>        // MFC Automation classes



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC support for Internet Explorer 4 Common Controls
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // MFC support for ribbons and control bars









#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif


#include <string>
#include <sstream>
#include <vector>
#include <mmsystem.h>


using std::string;
using std::wstring;
using std::vector;
using std::pair;

#include "../../../Common/Common/common.h"
#include "../../../Common/Network/network.h"




// 전역 윈도우 메세지
enum {

	// UDP2UDP 접속이 성공하면 ProxyWindow에세 메세지를 부모에게 보낸다.
	WM_UDP2UDP_START_SUCCESS = (WM_USER+10000),
		// lParam = ProxyWindow Handle (HWND)


	// UDP2UDP 접속이 실패하면, ProxyWindow에세 메세지를 부모에게 보낸다.
	WM_UDP2UDP_START_FAIL,
		// lParam = ProxyWindow Handle (HWND)


	// UDP2UDP 접속이 종료되면, ProxyWindow에세 메세지를 부모에게 보낸다.
	WM_UDP2UDP_STOP,
		// lParam = ProxyWindow Handle (HWND)


};


#include "udp2udpconfig.h"
extern cUDP2UDPConfig g_config;

