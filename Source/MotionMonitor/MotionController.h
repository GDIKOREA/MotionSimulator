//
//  ��� ���� ����� �Ѵ�.
//
#pragma once



class cMotionController : public common::cSingleton<cMotionController>
{
public:
	cMotionController();
	virtual ~cMotionController();


public:
	// ���������� ���� ��.
	float m_yaw;
	float m_pitch;
	float m_roll;
	float m_heave;

	
	// Joystick���κ��� �޾Ƽ� ���� ��
	cMotionWaveModulator m_joystickMod;

	// UDP�� ���� �޾Ƽ� ���� ��
	cMotionWaveModulator m_udpMod;

	// MotionWave�κ��� �޾Ƽ� ���� ��.
	cMotionWaveModulator m_mwavMod;
};
