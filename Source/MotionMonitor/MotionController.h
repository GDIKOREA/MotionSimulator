//
//  모션 제어 계산을 한다.
//
#pragma once


class cMotionController : public common::cSingleton<cMotionController>
{
public:
	cMotionController();
	virtual ~cMotionController();

	void SetUDPMotion(const float yaw, const float pitch, const float roll);
	void GetUDPMotion(float &yaw, float &pitch, float &roll);
	void GetUDPOriginalMotion(float &yaw, float &pitch, float &roll);

	void GetProportion(OUT float &yaw, OUT float &pitch, OUT float &roll);
	void GetScale(OUT float &yaw, OUT float &pitch, OUT float &roll);
	void GetScale2(OUT float &yaw, OUT float &pitch, OUT float &roll);


public:
	float m_udpYaw;
	float m_udpPitch;
	float m_udpRoll;

	float m_joystickYaw;
	float m_joystickPitch;
	float m_joystickRoll;
	float m_joystickHeave;

	float m_mwaveYaw;
	float m_mwavePitch;
	float m_mwaveRoll;
	float m_mwaveHeave;

	float m_udpOriginalYaw;
	float m_udpOriginalPitch;
	float m_udpOriginalRoll;

	float m_maxYawDeriv;
	float m_maxPitchDeriv;
	float m_maxRollDeriv;

	float m_udpYawProportion;
	float m_udpPitchProportion;
	float m_udpRollProportion;

	float m_udpYawScale;
	float m_udpPitchScale;
	float m_udpRollScale;

	float m_udpYawScale2;
	float m_udpPitchScale2;
	float m_udpRollScale2;
};
