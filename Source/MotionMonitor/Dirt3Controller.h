//
//
// Dirt3 모션 컨트롤러
//
//
#pragma once

#include "vitconmotionsim2.h"


class cDirt3Controller : public common::cSingleton<cDirt3Controller>
{
public:
	cDirt3Controller();
	virtual ~cDirt3Controller();

	enum STATE {
		OFF,				// 모션장치 Off
		OFF_PROCESS,  // 모션장치 On, 모션 장치 종료 준비 중
		READY,		// 모션장치 On, UDP 정보를 받아 모션 장치를 움직일 준비가 된 상태
		PLAY,			// 모션장치 On, UDP 정보를 받아 모션 장치를 움직이는 상태
		TIMEUPSTOP,	// 모션장치 On, 특정 조건에 의해 모션장치를 Stop 중인 상태 (시간 초과, 임의 Stop)
	};

	void StartMotionSim(const string &configFileName, const bool isStartMotionSimOut=false);
	void StopMotionSim();
	void Update(const float deltaSeconds);
	void UpdateUDP(const char *buffer, const int bufferLen);
	cVitconMotionSim2& GetMotionSim();
	STATE GetState() const;
	string GetStateStr() const;

	
protected:
	STATE m_state;
	cVitconMotionSim2 m_vitconMotionSim;
	cVitconMotionSim2::STATE m_oldState;
	float m_lastUDPUpdateTime; // 가장 최근에 UDP 패킷을 받은 시간을 저장한다.
	bool m_isLapTimeProgress; // @laptime 이 증가되기 시작하면, true 가 된다.
	float m_lastLapTime; // 전 프레임의 lap time
};


inline cVitconMotionSim2& cDirt3Controller::GetMotionSim() { return m_vitconMotionSim; }
inline cDirt3Controller::STATE cDirt3Controller::GetState() const { return m_state;  }
