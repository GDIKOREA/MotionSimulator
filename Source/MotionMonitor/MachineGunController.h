//
// 머신건 모션시뮬레이터 관리 한다.
//
// 게임 상태, 시간, 스테이지 관리를 한다.
// 
// 스테이트 머신을 이용해 모션시뮬레이터의 행동을 구현하고 있다.
//
#pragma once

#include "vitconmotionsim.h"


class cMachineGunController : public common::cSingleton<cMachineGunController>
{
public:
	cMachineGunController();
	virtual ~cMachineGunController();

	void StartMotionSim(const string &configFileName, const bool isStartMotionSimOut);
	void StopMotionSim();
	void Update(const float deltaSeconds);
	void UpdateUDP(const sMotionPacket &packet);
	bool CheckChangeState();
	//CString GetInnerStateName();


protected:
	void GameStart(const int stage);
	void GameStop();


public:
	cVitconMotionSim m_vitconMotionSim;

	string m_configFileName;
	bool m_isPlayGame;
	int m_gameState;
	int m_gameMission;
	float m_playTime; // seconds
	float m_lastUDPUpdateTime; // 가장 최근에 UDP 패킷을 받은 시간을 저장한다.

 	bool m_changeInformationPulse; // game state가 바뀔 때만 true 가 된다.
	cVitconMotionSim::STATE m_oldState;
};

