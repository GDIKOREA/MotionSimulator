//
//
// Joystick, MWave 모션 컨트롤러
//
//
#pragma once

#include "vitconmotionsim.h"


class cSimpleController : public common::cSingleton < cSimpleController >
{
public:
	cSimpleController();
	virtual ~cSimpleController();

	void StartMotionSim(const string &configFileName, const bool isStartMotionSimOut);
	void StopMotionSim();
	void Update(const float deltaSeconds);
	void UpdateUDP(const sMotionPacket &packet);


protected:
	cVitconMotionSim m_vitconMotionSim;
	cVitconMotionSim::STATE m_oldState;
};
