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

	bool Init();
	void StartMotionSim(const string &configFileName, const bool isStartMotionSimOut);
	void StopMotionSim();
	void Update(const float deltaSeconds);
	void UpdateUDP(const sMotionPacket &packet);
	bool CheckChangeState();
	//CString GetInnerStateName();


protected:
	void GameStart(const int stage);
	void GameStop();
	void MainBoardProcess(const char *buff, const int size);
	void CheckCreditPulse(const sMotionPacket &packet);
	void CheckReload(const sMotionPacket &packet);
	void ActiveMainBoard();
	void ActiveGunFire(const bool active1, const bool active2);


public:
	cVitconMotionSim m_vitconMotionSim;

	string m_configFileName;
	bool m_isPlayGame;
	int m_gameState;
	int m_gameMission;
	float m_playTime; // seconds
	float m_lastUDPUpdateTime; // 가장 최근에 UDP 패킷을 받은 시간을 저장한다.

	int m_credit;
	int m_coin;
	int m_coinPerGame;

	int m_totalGameCount;	// 총 진행된 게임 횟수
	int m_totalCredit;		// 총 크래딧수 (게임을 시작한 크래딧)
	int m_totalInputCredit; // 총 들어온 크래딧수

	bool m_checkCoin; // 코인신호가 들어 올 때부터 true 가 되고, 0 이 되면 false가 된다.
	int m_coinDownEdgeTime;
	int m_coinUpEdgeTime;

	bool m_checkCreditDown1;
	int m_creditUpEdgeTime1;
	bool m_checkCreditDown2;
	int m_creditUpEdgeTime2;

	bool m_activeGunFire1; // 1 플레어 총 활성화
	bool m_activeGunFire2; // 2 플레어 총 활성화

 	bool m_changeInformationPulse; // game state가 바뀔 때만 true 가 된다.
	cVitconMotionSim::STATE m_oldState;

	// UDP 통신
	// MainBoard 간 통신
	network::cUDPServer m_mainBoardInput; // 머신건 매인보드로 부터 정보를 얻는다.
	network::cUDPClient m_mainBoardSender; // 머신건 매인보드에게 정보를 보내는 객체
	network::cUDPClient m_mainBoardProxy; // 머신건 매인보드로 부터 받은 정보를 그대로 내보낸다.

	network::cUDPServer m_cameraUDPReceiver; // 머신건 카메라로 부터 정보를 얻는다.
	network::cUDPClient m_gameClientSender; // 머신건 클라이언트에 정보를 보낸다.

	sMGDevicePacket m_devicePacket;
};

