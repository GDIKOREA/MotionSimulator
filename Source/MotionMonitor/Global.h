#pragma once


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
