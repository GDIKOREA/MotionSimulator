#pragma once



struct sMotionPacket
{
	float directX, directY, directZ; // �ڵ��� ���� ����
	float pitch, yaw, roll; // �ڵ��� ���� radian
	float speed; // �ڵ��� �ӵ�
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
