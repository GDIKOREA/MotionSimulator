#pragma once


// 프로토콜 V.1
struct sMotionPacketV1
{
	float directX, directY, directZ; // 자동차 방향 벡터
	float pitch, yaw, roll; // 자동차 상태 radian
	float speed; // 자동차 속도
};

// 프로토콜 V.2
struct sMotionPacketV2
{
	float directX, directY, directZ; // 자동차 방향 벡터
	float pitch, yaw, roll; // 자동차 상태 radian
	float speed; // 자동차 속도
	int gamestate;
	int mission;
};

// 프로토콜 V.4
struct sMotionPacketV4
{
	float directX, directY, directZ; // 자동차 방향 벡터
	float roll, pitch, yaw; // 자동차 상태 radian
	float speed; // 자동차 속도
	int gamestate; // 0:대기 중, 1:게임 중
	int mission; // 스테이지 정보
	int score; // 스코어 정보, 실시간 스코어 정보
};

// 프로토콜 v.8
struct sMotionPacket
{
	float directX, directY, directZ; // 자동차 방향 벡터
	float roll, pitch, yaw; // 자동차 상태 radian
	float speed; // 자동차 속도
	int bulletcount1; // 플레어1의 총알 개수
	int bulletcount2; // 플레어2의 총알 개수
	int gamestate; // 0:대기 중, 1:게임 중
	int mission; // 스테이지 정보
	int score1; // 플레어1 스코어 정보, 실시간 스코어 정보
	int score2; // 플레어2 스코어 정보, 실시간 스코어 정보
	int creditDown1; // 플레어1 게임에 입장, 1:크레딧 차감, 0:Nothing~(1초 동안 1상태가 되어있다가, 0으로 초기화된다)
	int creditDown2; // 플레어2 게임에 입장, 1:크레딧 차감, 0:Nothing~(1초 동안 1상태가 되어있다가, 0으로 초기화된다)
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


bool WriteCameraSensitive(const string &fileName, const float sens);
bool ReadCameraSensitive(const string &fileName, OUT float &sens);
