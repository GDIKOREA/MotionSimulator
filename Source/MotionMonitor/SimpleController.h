//
//
// Joystick, MWave 모션 컨트롤러
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
