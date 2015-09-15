//
// �ӽŰ� ��ǽùķ����� ���� �Ѵ�.
//
// ���� ����, �ð�, �������� ������ �Ѵ�.
// 
// ������Ʈ �ӽ��� �̿��� ��ǽùķ������� �ൿ�� �����ϰ� �ִ�.
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


public:
	cVitconMotionSim m_vitconMotionSim;

	string m_configFileName;
	bool m_isPlayGame;
	int m_gameState;
	int m_gameMission;
	float m_playTime; // seconds
	float m_lastUDPUpdateTime; // ���� �ֱٿ� UDP ��Ŷ�� ���� �ð��� �����Ѵ�.
	int m_credit;
	int m_coin;
	int m_coinPerGame;

	bool m_checkCoin; // ���ν�ȣ�� ��� �� ������ true �� �ǰ�, 0 �� �Ǹ� false�� �ȴ�.
	int m_coinDownEdgeTime;
	int m_coinUpEdgeTime;

 	bool m_changeInformationPulse; // game state�� �ٲ� ���� true �� �ȴ�.
	cVitconMotionSim::STATE m_oldState;

	// UDP ���
	network::cUDPServer m_mainBoardInput; // �ӽŰ� ���κ���� ���� ������ ��´�.
	network::cUDPClient m_mainBoardSender; // �ӽŰ� ���κ��忡�� ������ ������ ��ü
	network::cUDPClient m_mainBoardProxy; // �ӽŰ� ���κ���� ���� ���� ������ �״�� ��������.
	network::cUDPServer m_cameraUDPReceiver; // �ӽŰ� ī�޶�� ���� ������ ��´�.
	network::cUDPClient m_gameClientSender; // �ӽŰ� Ŭ���̾�Ʈ�� ������ ������.

	sMGDevicePacket m_devicePacket;
};

