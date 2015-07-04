
#include "stdafx.h"
#include "MotionController.h"



cMotionController::cMotionController()
	: m_yaw(0)
	, m_pitch(0)
	, m_roll(0)
	, m_originalYaw(0)
	, m_originalPitch(0)
	, m_originalRoll(0)
	, m_maxYawDeriv(5)
	, m_maxPitchDeriv(5)
	, m_maxRollDeriv(5)
	, m_YawProportion(0.1f)
	, m_PitchProportion(0.1f)
	, m_RollProportion(0.1f)
	, m_YawScale(1.f)
	, m_PitchScale(1.f)
	, m_RollScale(1.f)
	, m_YawScale2(0.1f)
	, m_PitchScale2(0.1f)
	, m_RollScale2(0.1f)
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

	float diffY = (yaw - m_yaw) * m_YawProportion;
	float diffP = (pitch - m_pitch) * m_PitchProportion;
	float diffR = (roll - m_roll) * m_RollProportion;

// 	const float maxDiffYaw = m_maxYawDeriv;
// 	const float maxDiffPitch = m_maxPitchDeriv;
// 	const float maxDiffRoll = m_maxRollDeriv;
	
// 	if (abs(diffY) > maxDiffYaw)
// 		diffY = (diffY > 0) ? maxDiffYaw: -maxDiffYaw;
// 	if (abs(diffP) > maxDiffPitch)
// 		diffP = (diffP > 0) ? maxDiffPitch: -maxDiffPitch;
// 	if (abs(diffR) > maxDiffRoll)
// 		diffR = (diffR > 0) ? maxDiffRoll: -maxDiffRoll;

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


void cMotionController::GetProportion(OUT float &yaw, OUT float &pitch, OUT float &roll)
{
	yaw = m_YawProportion;
	pitch = m_PitchProportion;
	roll = m_RollProportion;
}


void cMotionController::GetScale(OUT float &yaw, OUT float &pitch, OUT float &roll)
{
	yaw = m_YawScale;
	pitch = m_PitchScale;
	roll = m_RollScale;
}


void cMotionController::GetScale2(OUT float &yaw, OUT float &pitch, OUT float &roll)
{
	yaw = m_YawScale2;
	pitch = m_PitchScale2;
	roll = m_RollScale2;
}

