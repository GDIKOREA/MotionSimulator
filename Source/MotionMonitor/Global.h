#pragma once


// 프로토콜 V.1
struct sMotionPacketV1
{
	float directX, directY, directZ; // 자동차 방향 벡터
	float pitch, yaw, roll; // 자동차 상태 radian
	float speed; // 자동차 속도
};

// 프로토콜 V.2
struct sMotionPacket
{
	float directX, directY, directZ; // 자동차 방향 벡터
	float pitch, yaw, roll; // 자동차 상태 radian
	float speed; // 자동차 속도
	int gamestate;
	int mission;
};



int AppendToLogAndScroll(CRichEditCtrl* pCtrl, CString str, COLORREF color);
int GetNumVisibleLines(CRichEditCtrl* pCtrl);



// BITCON Serial Communication Protocol
namespace BITCON_SER
{
	enum TYPE
	{
		START = 1,
		STOP = 2,
		ORIGIN = 5,
		EMERGENCY = 6,
		SERVOON = 7,
		SERVOOFF = 8,
	};
}


const static COLORREF g_blueColor = RGB(51, 153, 255);
const static COLORREF g_grayColor = RGB(240, 240, 240);
const static COLORREF g_redColor = RGB(255, 127, 80);



void SendMotionSimMessage(const int state);
