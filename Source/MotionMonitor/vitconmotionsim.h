//
// VitCon ������ �ʱ�ȭ �� ���۽�Ű�� ���� ����ϴ� Ŭ������.
//
// State Machine���� �����Ǿ� �ִ�.
//
#pragma once


class cVitconMotionSim
{
public:
	cVitconMotionSim();
	virtual ~cVitconMotionSim();

	enum STATE {
		OFF,	// SUBSTATE_OFF�� ��
		INIT,	// On() �� ��
		READY,  // SUBSTATE_READY�� �� 
		PLAY,	// SUBSTATE_PLAYING�� ��
		END,	// Off() �� ��
	};


protected:
	enum SUB_STATE {
		SUBSTATE_OFF,
		SUBSTATE_START_MOTION,
		SUBSTATE_INIT, // Servo On (5 seconds) 
		SUBSTATE_START, // Start (1 seconds)
		SUBSTATE_ORIGIN, // Origin Position (5 seconds)
		SUBSTATE_ORIGIN_STOP, // Origin Position (5 seconds) -> Stop
		SUBSTATE_STOP_READY, // Stop -> Ready
		SUBSTATE_READY, // Wait
		SUBSTATE_LOAD, // Load MotionWave
		SUBSTATE_PLAYING, // Play MotionWave
		SUBSTATE_END, // Origin Position(5 seconds)
		SUBSTATE_END_STOP, // Stop
		SUBSTATE_RESTART, // Origin Position(5 seconds) -> Start
		SUBSTATE_STOP_MOTION,
		SUBSTATE_STOP, // Servo Off
		SUBSTATE_DELAY,

		// VitCon ��� �ùķ����� ���� ����
		// INIT -> ORIGIN -> READY

		// ���ӽ���	
		// LOAD -> START -> PLAYING -> END

		// ��������, ���
		// ORIGIN -> STOP -> READY

		// �� ����
		// RESTART -> LOAD -> PLAYING -> END

		// ����
		// END -> END_STOP
	};


public:
	void Update(const float deltaSeconds);
	void On();
	void Ready();
	void Play();
	void Off();
	STATE GetState() const;
	

protected:
	void State_Off(const float deltaSeconds);
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
	void Delay(const float delaySeconds, const SUB_STATE next);


protected:
	STATE m_state;
	SUB_STATE m_subState;

	float m_playTime; // seconds
	float m_lastUDPUpdateTime; // ���� �ֱٿ� UDP ��Ŷ�� ���� �ð��� �����Ѵ�.

	bool m_changeInformationPulse; // game state�� �ٲ� ���� true �� �ȴ�.
	float m_delayTime;
	SUB_STATE m_delayNextState;
};


inline cVitconMotionSim::STATE cVitconMotionSim::GetState() const { return m_state;  }
