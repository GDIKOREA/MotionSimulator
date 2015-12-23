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
	float m_lastUDPUpdateTime; // ���� �ֱٿ� UDP ��Ŷ�� ���� �ð��� �����Ѵ�.

	bool m_activeGunFire1; // 1 �÷��� �� Ȱ��ȭ

	bool m_changeInformationPulse; // game state�� �ٲ� ���� true �� �ȴ�.

	// UDP ���
	// MainBoard �� ���
	network::cUDPServer m_cameraUDPReceiver; // �ӽŰ� ī�޶�� ���� ������ ��´�.
	network::cUDPClient m_gameClientSender; // �ӽŰ� Ŭ���̾�Ʈ�� ������ ������.
};
