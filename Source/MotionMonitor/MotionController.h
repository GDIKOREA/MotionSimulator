//
//  모션 제어 계산을 한다.
//
#pragma once



class cMotionController : public common::cSingleton<cMotionController>
{
public:
	cMotionController();
	virtual ~cMotionController();


public:
	// 최종적으로 계산된 값.
	float m_yaw;
	float m_pitch;
	float m_roll;
	float m_heave;

	
	// Joystick으로부터 받아서 계산된 값
	cMotionWaveModulator m_joystickMod;

	// UDP로 부터 받아서 계산된 값
	cMotionWaveModulator m_udpMod;

	// MotionWave로부터 받아서 계산된 값.
	cMotionWaveModulator m_mwavMod;
};
