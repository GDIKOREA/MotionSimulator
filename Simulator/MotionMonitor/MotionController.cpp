
#include "stdafx.h"
#include "MotionController.h"



cMotionController::cMotionController()
	: m_yaw(0)
	, m_pitch(0)
	, m_roll(0)
	, m_maxYawDeriv(5)
	, m_maxPitchDeriv(5)
	, m_maxRollDeriv(5)
{

}

cMotionController::~cMotionController()
{

}


void cMotionController::SetMotion(const float yaw, const float pitch, const float roll)
{
	static int oldT = timeGetTime();
	int curT = timeGetTime();
	int deltaT = curT - oldT;
	const float deltaSeconds = deltaT / 1000.f;
	if (deltaSeconds <= 0.001f)
		return;

	oldT = curT;

	float diffY = (yaw - m_yaw) / deltaSeconds;
	float diffP = (pitch - m_pitch) / deltaSeconds;
	float diffR = (roll - m_roll) / deltaSeconds;

	const float maxDiffYaw = m_maxYawDeriv;
	const float maxDiffPitch = m_maxPitchDeriv;
	const float maxDiffRoll = m_maxRollDeriv;
	
	if (abs(diffY) > maxDiffYaw)
		diffY = (diffY > 0) ? maxDiffYaw*deltaSeconds : -maxDiffYaw*deltaSeconds;
	if (abs(diffP) > maxDiffPitch)
		diffP = (diffP > 0) ? maxDiffPitch*deltaSeconds : -maxDiffPitch*deltaSeconds;
	if (abs(diffR) > maxDiffRoll)
		diffR = (diffR > 0) ? maxDiffRoll*deltaSeconds : -maxDiffRoll*deltaSeconds;

	m_yaw += diffY;
	m_pitch += diffP;
	m_roll += diffR;

	m_originalYaw = yaw;
	m_originalPitch = pitch;
	m_originalRoll = roll;
}


void cMotionController::GetMotion(float &yaw, float &pitch, float &roll) 
{
	yaw = m_yaw;
	pitch = m_pitch;
	roll = m_roll;
}


void cMotionController::GetOriginalMotion(float &yaw, float &pitch, float &roll)
{
	yaw = m_originalYaw;
	pitch = m_originalPitch;
	roll = m_originalRoll;
}
