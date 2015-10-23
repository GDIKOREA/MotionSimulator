//
//
// Dirt3 모션 컨트롤러
//
//
#pragma once

#include "vitconmotionsim.h"


class cDirt3Controller : public common::cSingleton<cDirt3Controller>
{
public:
	cDirt3Controller();
	virtual ~cDirt3Controller();

	void StartMotionSim(const string &configFileName, const bool isStartMotionSimOut=false);
	void StopMotionSim();
	void Update(const float deltaSeconds);
	void UpdateUDP(const char *buffer, const int bufferLen);
	cVitconMotionSim& GetMotionSim();

protected:
	cVitconMotionSim m_vitconMotionSim;
	cVitconMotionSim::STATE m_oldState;
};


inline cVitconMotionSim& cDirt3Controller::GetMotionSim() { return m_vitconMotionSim; }
