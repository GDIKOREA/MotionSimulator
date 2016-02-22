
#pragma once


//--------------------------------------------------------------------------------------------------------------
// �ӽŰ� ���� Ŭ���̾�Ʈ���� ������ ��Ŷ ��������

// �������� V.1
struct sMotionPacketV1
{
	float directX, directY, directZ; // �ڵ��� ���� ����
	float pitch, yaw, roll; // �ڵ��� ���� radian
	float speed; // �ڵ��� �ӵ�
};

// �������� V.2
struct sMotionPacketV2
{
	float directX, directY, directZ; // �ڵ��� ���� ����
	float pitch, yaw, roll; // �ڵ��� ���� radian
	float speed; // �ڵ��� �ӵ�
	int gamestate;
	int mission;
};

// �������� V.4
struct sMotionPacketV4
{
	float directX, directY, directZ; // �ڵ��� ���� ����
	float roll, pitch, yaw; // �ڵ��� ���� radian
	float speed; // �ڵ��� �ӵ�
	int gamestate; // 0:��� ��, 1:���� ��
	int mission; // �������� ����
	int score; // ���ھ� ����, �ǽð� ���ھ� ����
};

// �������� v.8
struct sMotionPacket
{
	float directX, directY, directZ; // �ڵ��� ���� ����
	float roll, pitch, yaw; // �ڵ��� ���� radian
	float speed; // �ڵ��� �ӵ�
	int bulletcount1; // �÷���1�� �Ѿ� ����
	int bulletcount2; // �÷���2�� �Ѿ� ����
	int gamestate; // 0:��� ��, 1:���� ��
	int mission; // �������� ����
	int score1; // �÷���1 ���ھ� ����, �ǽð� ���ھ� ����
	int score2; // �÷���2 ���ھ� ����, �ǽð� ���ھ� ����
	int creditDown1; // �÷���1 ���ӿ� ����, 1:ũ���� ����, 0:Nothing~(1�� ���� 1���°� �Ǿ��ִٰ�, 0���� �ʱ�ȭ�ȴ�)
	int creditDown2; // �÷���2 ���ӿ� ����, 1:ũ���� ����, 0:Nothing~(1�� ���� 1���°� �Ǿ��ִٰ�, 0���� �ʱ�ȭ�ȴ�)
};



//--------------------------------------------------------------------------------------------------------------
// �ӽŰ� ���κ��忡�� ������ UDP ��Ŷ ��������
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
// �ӽŰ� �Ŵ������� ���� Ŭ���̾�Ʈ���� ������ ��Ŷ ��������

struct SMGCameraData
{
	float x1, y1, x2, y2; // ���� ����Ű�� ��ġ 0 ~ 1 ���� ��. ȭ���� ���� �Ʒ��� {0,0}
	int fire1; // �÷���1 �ݹ�, 1:�ݹ�, 0:Nothing~
	int fire2; // �÷���2 �ݹ�, 1:�ݹ�, 0:Nothing~
	int reload1; // �÷���1 ���ε�, 1:���ε�, 0:Nothing~
	int reload2; // �÷���2 ���ε�, 1:���ε�, 0:Nothing~
	int start1; // �÷���1 ��ŸƮ��ư On/Off, 1:On, 0:Off
	int start2; // �÷���2 ��ŸƮ��ư On/Off, 1:On, 0:Off
	int credit; // ���� �÷��� �� �� �ִ� ȸ��
	int coinCount; // ������ ���� ����
	int coinPerGame; // �� ���Ӵ� ���� ����
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


