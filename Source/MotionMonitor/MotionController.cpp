
#include "stdafx.h"
#include "MotionController.h"



cMotionController::cMotionController()
	: m_udpYaw(0)
	, m_udpPitch(0)
	, m_udpRoll(0)
	, m_joystickYaw(0)
	, m_joystickPitch(0)
	, m_joystickRoll(0)
	, m_joystickHeave(0)
	, m_mwaveYaw(0)
	, m_mwavePitch(0)
	, m_mwaveRoll(0)
	, m_mwaveHeave(0)
	, m_udpOriginalYaw(0)
	, m_udpOriginalPitch(0)
	, m_udpOriginalRoll(0)
	, m_maxYawDeriv(5)
	, m_maxPitchDeriv(5)
	, m_maxRollDeriv(5)
	, m_udpYawProportion(0.1f)
	, m_udpPitchProportion(0.1f)
	, m_udpRollProportion(0.1f)
	, m_udpYawScale(1.f)
	, m_udpPitchScale(1.f)
	, m_udpRollScale(1.f)
	, m_udpYawScale2(0.1f)
	, m_udpPitchScale2(0.1f)
	, m_udpRollScale2(0.1f)
{

}

cMotionController::~cMotionController()
{

}


void cMotionController::SetUDPMotion(const float yaw, const float pitch, const float roll)
{
	static int oldT = timeGetTime();
	int curT = timeGetTime();
	int deltaT = curT - oldT;
	const float deltaSeconds = deltaT / 1000.f;
	if (deltaSeconds <= 0.001f)
		return;

	oldT = curT;

	float diffY = (yaw - m_udpYaw) * m_udpYawProportion;
	float diffP = (pitch - m_udpPitch) * m_udpPitchProportion;
	float diffR = (roll - m_udpRoll) * m_udpRollProportion;

// 	const float maxDiffYaw = m_maxYawDeriv;
// 	const float maxDiffPitch = m_maxPitchDeriv;
// 	const float maxDiffRoll = m_maxRollDeriv;
	
// 	if (abs(diffY) > maxDiffYaw)
// 		diffY = (diffY > 0) ? maxDiffYaw: -maxDiffYaw;
// 	if (abs(diffP) > maxDiffPitch)
// 		diffP = (diffP > 0) ? maxDiffPitch: -maxDiffPitch;
// 	if (abs(diffR) > maxDiffRoll)
// 		diffR = (diffR > 0) ? maxDiffRoll: -maxDiffRoll;

	m_udpYaw += diffY;
	m_udpPitch += diffP;
	m_udpRoll += diffR;

	m_udpOriginalYaw = yaw;
	m_udpOriginalPitch = pitch;
	m_udpOriginalRoll = roll;
}


void cMotionController::GetUDPMotion(float &yaw, float &pitch, float &roll) 
{
	yaw = m_udpYaw;
	pitch = m_udpPitch;
	roll = m_udpRoll;
}


void cMotionController::GetUDPOriginalMotion(float &yaw, float &pitch, float &roll)
{
	yaw = m_udpOriginalYaw;
	pitch = m_udpOriginalPitch;
	roll = m_udpOriginalRoll;
}


void cMotionController::GetProportion(OUT float &yaw, OUT float &pitch, OUT float &roll)
{
	yaw = m_udpYawProportion;
	pitch = m_udpPitchProportion;
	roll = m_udpRollProportion;
}


void cMotionController::GetScale(OUT float &yaw, OUT float &pitch, OUT float &roll)
{
	yaw = m_udpYawScale;
	pitch = m_udpPitchScale;
	roll = m_udpRollScale;
}


void cMotionController::GetScale2(OUT float &yaw, OUT float &pitch, OUT float &roll)
{
	yaw = m_udpYawScale2;
	pitch = m_udpPitchScale2;
	roll = m_udpRollScale2;
}

