//
//  ��� ���� ����� �Ѵ�.
//
#pragma once

#include "motionmonitorconfig.h"
#include "simplemodulator.h"


class cMotionController : public common::cSingleton<cMotionController>
{
public:
	cMotionController();
	virtual ~cMotionController();

	bool ReadConfigFile(const string &fileName);
	bool WriteConfigFile(const string &fileName);

	float GetActuatorSpeed() const;
	void SetActuatorSpeed(const float speed);
	float GetActuatorYawPower() const;
	void SetActuatorYawPower(const float power);
	float GetPlayTime() const;
	void SetPlayTime(const float t);


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

	// Pitch �� ����
	cSimpleModulator m_varModulator1;

	// Roll �� ����
	cSimpleModulator m_varModulator2;

	cMotionMonitorConfig m_config;

	float m_actuatorYawPower; // 0~1 ���� ��
	float m_playTime; // �� ���Ӵ� �÷��� �� �� �ִ� �ð�. seconds ����, default : 60
	float m_actuatorSpeed; // 0~1 ���� ��

	string m_loginId;
};


inline float cMotionController::GetActuatorSpeed() const { return m_actuatorSpeed; }
inline void cMotionController::SetActuatorSpeed(const float speed) { m_actuatorSpeed = speed; }
inline float cMotionController::GetPlayTime() const { return m_playTime; }
inline void cMotionController::SetPlayTime(const float t) { m_playTime = t; }
inline float cMotionController::GetActuatorYawPower() const { return m_actuatorYawPower; }
inline void cMotionController::SetActuatorYawPower(const float power) { m_actuatorYawPower = power; }
