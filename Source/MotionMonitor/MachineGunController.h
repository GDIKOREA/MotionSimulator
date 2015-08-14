//
// �ӽŰ� ��ǽùķ����� ���� �Ѵ�.
//
// ���� ����, �ð�, �������� ������ �Ѵ�.
// 
// ������Ʈ �ӽ��� �̿��� ��ǽùķ������� �ൿ�� �����ϰ� �ִ�.
//
#pragma once


namespace MOTION_SIM_STATE {
	enum TYPE{
		NONE,
		START_MOTION,
		INIT, // Servo On (5 seconds) 
		START, // Start (1 seconds)
		ORIGIN, // Origin Position (5 seconds)
		ORIGIN_STOP, // Origin Position (5 seconds) -> Stop
		STOP_READY, // Stop -> Ready
		READY, // Wait
		LOAD, // Load MotionWave
		PLAYING, // Play MotionWave
		END, // Origin Position(5 seconds)
		END_STOP, // Stop
		RESTART, // Origin Position(5 seconds) -> Start
		STOP_MOTION,
		STOP, // Servo Off
		DELAY,
	};


	// ��� �ùķ����� ���� ����
	// INIT -> ORIGIN -> READY

	// ���ӽ���	
	// LOAD -> START -> PLAYING -> END

	// ��������, ���
	// ORIGIN -> STOP -> READY

	// �� ����
	// RESTART -> LOAD -> PLAYING -> END
	
	// ����
	// END -> END_STOP
}


class cMachineGunController : public common::cSingleton<cMachineGunController>
{
public:
	cMachineGunController();
	virtual ~cMachineGunController();

	void StartMotionSim();
	void StopMotionSim();
	void Update(const float deltaSeconds);
	void UpdateUDP(const sMotionPacket &packet);
	bool CheckChangeState();
	CString GetInnerStateName();


protected:
	void GameStart(const int stage);
	void GameStop();
	void SendMotionSimMessage(const int state);
	void Delay(const float delaySeconds, const MOTION_SIM_STATE::TYPE next);

	void State_Init(const float deltaSeconds);
	void State_Start(const float deltaSeconds);
	void State_Origin(const float deltaSeconds);
	void State_OriginStop(const float deltaSeconds);
	void State_StopReady(const float deltaSeconds);
	void State_Ready(const float deltaSeconds);
	void State_Load(const float deltaSeconds);
	void State_Playing(const float deltaSeconds);
	void State_End(const float deltaSeconds);
	void State_EndStop(const float deltaSeconds);
	void State_Restart(const float deltaSeconds);
	void State_Stop(const float deltaSeconds);
	void State_Delay(const float deltaSeconds);


public:
	MOTION_SIM_STATE::TYPE m_state;
	bool m_isPlayGame;
	int m_gameState;
	int m_gameMission;
	float m_playTime; // seconds
	float m_lastUDPUpdateTime; // ���� �ֱٿ� UDP ��Ŷ�� ���� �ð��� �����Ѵ�.

	bool m_changeInformationPulse; // game state�� �ٲ� ���� true �� �ȴ�.
	float m_delayTime;
	MOTION_SIM_STATE::TYPE m_delayNextState;
};

