#pragma once



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
