//
//  ��� ���� ����� �Ѵ�.
//
#pragma once

#include "motionmonitorconfig.h"


class cMotionController : public common::cSingleton<cMotionController>
{
public:
	cMotionController();
	virtual ~cMotionController();

	bool ReadConfigFile(const string &fileName);
	bool WriteConfigFile(const string &fileName);


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


	cMotionMonitorConfig m_config;
};
