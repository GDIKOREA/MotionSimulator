
#include "stdafx.h"
#include "Global.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/xml_parser.hpp>



GAME_TYPE::TYPE g_gameType = GAME_TYPE::MACHINEGUN;



// http://www.codeproject.com/Articles/12093/Using-RichEditCtrl-to-Display-Formatted-Logs
int AppendToLogAndScroll(CRichEditCtrl* pCtrl, CString str, COLORREF color)
{
	long nVisible = 0;
	long nInsertionPoint = 0;
	CHARFORMAT cf;

	// Initialize character format structure
	cf.cbSize = sizeof(CHARFORMAT);
	cf.dwMask = CFM_COLOR;
	cf.dwEffects = 0; // To disable CFE_AUTOCOLOR

	cf.crTextColor = color;

	// Set insertion point to end of text
	nInsertionPoint = pCtrl->GetWindowTextLength();
	pCtrl->SetSel(nInsertionPoint, -1);

	// Set the character format
	pCtrl->SetSelectionCharFormat(cf);

	// Replace selection. Because we have nothing
	// selected, this will simply insert
	// the string at the current caret position.
	pCtrl->ReplaceSel(str);

	// Get number of currently visible lines or maximum number of visible lines
	// (We must call GetNumVisibleLines() before the first call to LineScroll()!)
	nVisible = GetNumVisibleLines(pCtrl);

	// Now this is the fix of CRichEditCtrl's abnormal behaviour when used
	// in an application not based on dialogs. Checking the focus prevents
	// us from scrolling when the CRichEditCtrl does so automatically,
	// even though ES_AUTOxSCROLL style is NOT set.
	if (pCtrl != pCtrl->GetFocus())
	{
		pCtrl->LineScroll(INT_MAX);
		pCtrl->LineScroll(1 - nVisible);
	}

	// 내용이 너무 많으면 지운다.
	const int maximumLine = 100;
	if (pCtrl->GetLineCount() > maximumLine)
	{
		long nFirstChar = pCtrl->CharFromPos(CPoint(0, 0));
		pCtrl->SetSel(0, nFirstChar);
		pCtrl->ReplaceSel(L"");
	}

	return 0;
}


//http://www.codeproject.com/Articles/12093/Using-RichEditCtrl-to-Display-Formatted-Logs
int GetNumVisibleLines(CRichEditCtrl* pCtrl)
{
	CRect rect;
	long nFirstChar, nLastChar;
	long nFirstLine, nLastLine;

	// Get client rect of rich edit control
	pCtrl->GetClientRect(rect);

	// Get character index close to upper left corner
	nFirstChar = pCtrl->CharFromPos(CPoint(0, 0));

	// Get character index close to lower right corner
	nLastChar = pCtrl->CharFromPos(CPoint(rect.right, rect.bottom));
	if (nLastChar < 0)
	{
		nLastChar = pCtrl->GetTextLength();
	}

	// Convert to lines
	nFirstLine = pCtrl->LineFromChar(nFirstChar);
	nLastLine = pCtrl->LineFromChar(nLastChar);

	return (nLastLine - nFirstLine);
}



// 모션 컨트롤러 상태 메세지를 보낸다.
void SendMotionSimMessage(const int state)
{
	const int out_pitch = 256;
	const int out_roll = 256;
	const int out_yaw = 256;
	const int out_sway = 256;
	const int out_surge = 256;
	const int out_heave = 512;
	const int out_switch = state;

	string out = common::format(
		"A%3d%3d%3d%3d%3d%3d%dZ", out_roll, out_pitch, out_yaw,
		out_sway, out_surge, out_heave, out_switch);
	SpaceCharToZeroChar(out);

	for (int i = 0; i < 5; ++i)
	{
		Sleep(50);
		cController::Get()->GetSerialComm().GetSerial().SendData(out.c_str(), out.size());
	}
	Sleep(50);
}


// Dirt3 motion simulator 용 프로토콜
void SendMotionSimMessage2(const int state)
{
	const int out_pitch = 10000;
	const int out_roll = 10000;
	const int out_yaw = 10000;
	const int out_sway = 10000;
	const int out_surge = 10000;
	const int out_heave = 10000;
	const int spareSpeed = 100;
	const int out_switch = state;

// 	const string out = common::format(
// 		"A%3d%3d%3d%3d%3d%3d%dZ", out_roll, out_pitch, out_yaw,
// 		out_sway, out_surge, out_heave, out_switch);
	string out = common::format(
		"A%5d%5d%5d%5d%5d%5d%3d%3d%3d%3d%3d%3d%1dZ", out_roll, out_pitch, out_yaw,
		out_sway, out_surge, out_heave, 
		spareSpeed, spareSpeed, spareSpeed, spareSpeed, spareSpeed, spareSpeed,
		out_switch);
	SpaceCharToZeroChar(out);

	for (int i = 0; i < 5; ++i)
	{
		Sleep(50);
		cController::Get()->GetSerialComm().GetSerial().SendData(out.c_str(), out.size());
	}
	Sleep(50);
}


// 카메라 감도 값 조정
bool WriteCameraSensitive(const string &fileName, const float sens)
{
	try
	{
		// boost property tree
		using boost::property_tree::ptree;
		using std::string;
		ptree props;
		boost::property_tree::read_json(fileName, props);

		if (props.get<string>("format", "") != "camera contour")
		{
			::AfxMessageBox(L"Fail!!\n");
			return false;
		}

		props.put<float>("scale", sens);

		boost::property_tree::write_json(fileName, props);
	}
	catch (std::exception&e)
	{
		::AfxMessageBox(CString(L"Error!!\n") + str2wstr(e.what()).c_str());
		return false;
	}

	return true;
}


// 카메라 감도 값을 읽어서 리턴한다.
bool ReadCameraSensitive(const string &fileName, OUT float &sens)
{
	try
	{
		// boost property tree
		using boost::property_tree::ptree;
		using std::string;
		ptree props;
		boost::property_tree::read_json(fileName, props);

		if (props.get<string>("format", "") != "camera contour")
		{
			::AfxMessageBox(L"Fail!!\n");
			return false;
		}

		sens = props.get<float>("scale", 1);
	}
	catch (std::exception&e)
	{
		::AfxMessageBox(CString(L"Error!!\n") + str2wstr(e.what()).c_str());
		return false;
	}

	return true;
}


// 문자열 중 space bar 를 숫자 0으로 바꾼다.
void SpaceCharToZeroChar(string &str)
{
	common::replaceAll(str, " ", "0");
}
