
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

#include <winspool.h>
#include <WBemCli.h>







#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif





#include "../Common/Common/common.h"
#include "../Common/Graphic/graphic.h"
#include "../Common/UIComponent/uicomponent.h"
#include "../Common/Network/network.h"
#include <mmsystem.h>
#include <fstream>
//#include "Serial.h"

using std::pair;

class C3DView;
class C3DPhysxView;
extern C3DView *g_3DView;
extern C3DPhysxView *g_3DPhysxView;


enum VIEW_TYPE {
	ID_VIEW_SENSOR = 10001,
	ID_VIEW_3D,
	ID_VIEW_MOTION3D,
	ID_VIEW_MOTION_OUTPUT,
	ID_VIEW_MOTION_INPUT,
	ID_VIEW_JOYSTICK,
	ID_VIEW_MOTIONWAVE,
	ID_VIEW_MIXING,
	ID_VIEW_CONTROLBOARD,
	ID_VIEW_UDPPARSE,
	ID_VIEW_PLOT,
	ID_VIEW_LAUNCHER,
	ID_VIEW_3DSIMUL,
};

#define CENUMERATESERIAL_USE_STL


namespace GAME_TYPE {
	enum TYPE {
		DIRT3,
		MACHINEGUN,
		JOYSTICK,
		MWAVE,
	};
}


extern GAME_TYPE::TYPE g_gameType;


class CMotionWaveView;
class CUDPInputView;
class CControlBoard;
class CLauncherView;
extern CMotionWaveView *g_mwaveView;
extern CUDPInputView *g_udpInputView;
extern CControlBoard *g_controlView;
extern CLauncherView *g_launcherView;
extern C3DView *g_3dGameView;
extern C3DView *g_3dMotionView;


#include "Global.h"
#include "motionwave.h"
#include "motionwavemodulator.h"
#include "spline.h"
#include "MotionController.h"
#include "plotinputparser.h"
