
#pragma once


//--------------------------------------------------------------------------------------------------------------
// 머신건 게임 클라이언트에서 보내는 패킷 프로토콜

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



//--------------------------------------------------------------------------------------------------------------
// 머신건 매인보드에서 보내는 UDP 패킷 프로토콜
// Stand Version


struct sMGDevicePacket
{
	char header; // $
	char comma1; // ,
	char deviceNumber; // 9
	char zero1; // 0
	char comma2; // ,

	char player1Fire;			// 0/1
	char player1FireEvent;		// 0/1
	char player1Reload;			// 0/1
	char player1Start;			// 0/1
	char player1UpMotor;		// 0/1
	char player1DownMotor;		// 0/1
	char player1UpSensor;		// 0/1
	char player1DownSensor;		// 0/1
	char player1EmergencySwitch;// 0/1
	char coin;
	char space1;

	char player2Fire;			// 0/1
	char player2FireEvent;		// 0/1
	char player2Reload;			// 0/1
	char player2Start;			// 0/1
	char player2UpMotor;		// 0/1
	char player2DownMotor;		// 0/1
	char player2UpSensor;		// 0/1
	char player2DownSensor;		// 0/1
	char player2EmergencySwitch;// 0/1
	char space3;
	char space4;

	char comma3;
	char at;

	char cr[2];					// 0x0d, 0x0a
};



//--------------------------------------------------------------------------------------------------------------
// 머신건 매니저에서 게임 클라이언트에게 보내는 패킷 프로토콜

struct SMGCameraData
{
	float x1, y1, x2, y2; // 총이 가르키는 위치 0 ~ 1 사이 값. 화면의 왼쪽 아래가 {0,0}
	int fire1; // 플레어1 격발, 1:격발, 0:Nothing~
	int fire2; // 플레어2 격발, 1:격발, 0:Nothing~
	int reload1; // 플레어1 리로드, 1:리로드, 0:Nothing~
	int reload2; // 플레어2 리로드, 1:리로드, 0:Nothing~
	int start1; // 플레어1 스타트버튼 On/Off, 1:On, 0:Off
	int start2; // 플레어2 스타트버튼 On/Off, 1:On, 0:Off
	int credit; // 게임 플레이 할 수 있는 회수
	int coinCount; // 여분의 동전 개수
	int coinPerGame; // 한 게임당 동전 개수
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


namespace AXIS_TYPE
{
	enum TYPE
	{
		AXIS4=0, 
		AXIS3,
	};
}


const static COLORREF g_blueColor = RGB(51, 153, 255);
const static COLORREF g_grayColor = RGB(240, 240, 240);
const static COLORREF g_redColor = RGB(255, 127, 80);
void SendMotionSimMessage(const int state);
void SendMotionSimMessage2(const int state);

void SpaceCharToZeroChar(string &str);


bool WriteCameraSensitive(const string &fileName, const float sens);
bool ReadCameraSensitive(const string &fileName, OUT float &sens);


