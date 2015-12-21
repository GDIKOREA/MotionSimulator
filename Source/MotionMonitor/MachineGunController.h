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
	float m_lastUDPUpdateTime; // ���� �ֱٿ� UDP ��Ŷ�� ���� �ð��� �����Ѵ�.

	int m_credit;
	int m_coin;
	int m_coinPerGame;

	int m_totalGameCount;	// �� ����� ���� Ƚ��
	int m_totalCredit;		// �� ũ������ (������ ������ ũ����)
	int m_totalInputCredit; // �� ���� ũ������

	bool m_checkCoin; // ���ν�ȣ�� ��� �� ������ true �� �ǰ�, 0 �� �Ǹ� false�� �ȴ�.
	int m_coinDownEdgeTime;
	int m_coinUpEdgeTime;

	bool m_checkCreditDown1;
	int m_creditUpEdgeTime1;
	bool m_checkCreditDown2;
	int m_creditUpEdgeTime2;

	bool m_activeGunFire1; // 1 �÷��� �� Ȱ��ȭ
	bool m_activeGunFire2; // 2 �÷��� �� Ȱ��ȭ

 	bool m_changeInformationPulse; // game state�� �ٲ� ���� true �� �ȴ�.
	cVitconMotionSim::STATE m_oldState;

	// UDP ���
	// MainBoard �� ���
	network::cUDPServer m_mainBoardInput; // �ӽŰ� ���κ���� ���� ������ ��´�.
	network::cUDPClient m_mainBoardSender; // �ӽŰ� ���κ��忡�� ������ ������ ��ü
	network::cUDPClient m_mainBoardProxy; // �ӽŰ� ���κ���� ���� ���� ������ �״�� ��������.

	network::cUDPServer m_cameraUDPReceiver; // �ӽŰ� ī�޶�� ���� ������ ��´�.
	network::cUDPClient m_gameClientSender; // �ӽŰ� Ŭ���̾�Ʈ�� ������ ������.

	sMGDevicePacket m_devicePacket;
};

