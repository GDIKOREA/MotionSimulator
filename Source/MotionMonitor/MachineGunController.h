//
// 머신건 모션시뮬레이터 관리 한다.
//
// 게임 상태, 시간, 스테이지 관리를 한다.
// 
// 스테이트 머신을 이용해 모션시뮬레이터의 행동을 구현하고 있다.
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


	// 모션 시뮬레이터 실행 순서
	// INIT -> ORIGIN -> READY

	// 게임시작	
	// LOAD -> START -> PLAYING -> END

	// 게임종료, 대기
	// ORIGIN -> STOP -> READY

	// 재 게임
	// RESTART -> LOAD -> PLAYING -> END
	
	// 종료
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
	float m_lastUDPUpdateTime; // 가장 최근에 UDP 패킷을 받은 시간을 저장한다.

	bool m_changeInformationPulse; // game state가 바뀔 때만 true 가 된다.
	float m_delayTime;
	MOTION_SIM_STATE::TYPE m_delayNextState;
};

