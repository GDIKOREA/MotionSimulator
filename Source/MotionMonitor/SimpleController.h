//
//
// Joystick, MWave ��� ��Ʈ�ѷ�
//
//
#pragma once


class cSimpleController : public common::cSingleton < cSimpleController >
{
public:
	cSimpleController();
	virtual ~cSimpleController();

	void StartMotionSim(const string &configFileName);
	void StopMotionSim();
	void Update(const float deltaSeconds);
	void UpdateUDP(const sMotionPacket &packet);


protected:
};
