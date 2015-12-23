#pragma once


class cRealShotController : public common::cSingleton<cRealShotController>
{
public:
	cRealShotController();
	virtual ~cRealShotController();

	bool Init();
	void StartMotionSim(const string &configFileName, const bool isStartMotionSimOut);
	void StopMotionSim();
	void Update(const float deltaSeconds);
	bool CheckChangeState();


protected:
	void GameStart(const int stage);
	void GameStop();
	void MainBoardProcess(const char *buff, const int size);


public:
	string m_configFileName;
	bool m_isPlayGame;
	int m_gameState;
	int m_gameMission;
	float m_playTime; // seconds
	float m_lastUDPUpdateTime; // 가장 최근에 UDP 패킷을 받은 시간을 저장한다.

	bool m_activeGunFire1; // 1 플레어 총 활성화

	bool m_changeInformationPulse; // game state가 바뀔 때만 true 가 된다.

	// UDP 통신
	// MainBoard 간 통신
	network::cUDPServer m_cameraUDPReceiver; // 머신건 카메라로 부터 정보를 얻는다.
	network::cUDPClient m_gameClientSender; // 머신건 클라이언트에 정보를 보낸다.
};
