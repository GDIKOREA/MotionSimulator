//
//
// Dirt3 모션 컨트롤러
//
//
#pragma once


class cDirt3Controller : public common::cSingleton<cDirt3Controller>
{
public:
	cDirt3Controller();
	virtual ~cDirt3Controller();

	void StartMotionSim(const string &configFileName);
	void StopMotionSim();
	void Update(const float deltaSeconds);
	void UpdateUDP(const sMotionPacket &packet);


protected:
};
