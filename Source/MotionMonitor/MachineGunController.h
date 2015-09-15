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
	network::cUDPServer m_hardwareInput; // �ӽŰ� ���κ���� ���� ������ ��´�.
	network::cUDPClient m_hardwareSender; // �ӽŰ� ���κ��忡�� ������ ������ ��ü

	network::cUDPServer m_cameraUDPReceiver; // �ӽŰ� ī�޶�� ���� ������ ��´�.
	network::cUDPClient m_gameClientSender; // �ӽŰ� Ŭ���̾�Ʈ�� ������ ������.

	cVitconMotionSim m_vitconMotionSim;

	string m_configFileName;
	bool m_isPlayGame;
	int m_gameState;
	int m_gameMission;
	float m_playTime; // seconds
	float m_lastUDPUpdateTime; // ���� �ֱٿ� UDP ��Ŷ�� ���� �ð��� �����Ѵ�.

 	bool m_changeInformationPulse; // game state�� �ٲ� ���� true �� �ȴ�.
	cVitconMotionSim::STATE m_oldState;
};

