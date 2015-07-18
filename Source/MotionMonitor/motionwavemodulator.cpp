
#include "stdafx.h"
#include "motionwavemodulator.h"


cMotionWaveModulator::cMotionWaveModulator()
	: m_incTime(0)
	, m_originalYaw(0)
	, m_originalPitch(0)
	, m_originalRoll(0)
	, m_originalHeave(0)

	, m_yawProportion(1)
	, m_pitchProportion(1)
	, m_rollProportion(1)
	, m_heaveProportion(1)

	, m_yawC1(0)
	, m_yawC2(0)
	, m_yawC3(0)

	, m_pitchC1(0)
	, m_pitchC2(255)
	, m_pitchC3(255)

	, m_rollC1(0)
	, m_rollC2(255)
	, m_rollC3(255)

	, m_heaveC1(0)
	, m_heaveC2(0)
	, m_heaveC3(0)
{
	ZeroMemory(m_yaw, sizeof(m_yaw));
	ZeroMemory(m_pitch, sizeof(m_pitch));
	ZeroMemory(m_roll, sizeof(m_roll));
	ZeroMemory(m_heave, sizeof(m_heave));
}

cMotionWaveModulator::~cMotionWaveModulator()
{
	
}


// 정보를 업데이트 한다.
void cMotionWaveModulator::Update(const float deltaSeconds,
	const float yaw, const float pitch, const float roll, const float heave)
{
	m_incTime += deltaSeconds;
	if (m_incTime <= 0.001f)
		return;

	m_incTime = 0;


	//---------------------------------------------------------------------------------------
	// PID
	const float diffY = (yaw - m_yaw[0]) * m_yawProportion;
	const float diffP = (pitch - m_pitch[0]) * m_pitchProportion;
	const float diffR = (roll - m_roll[0]) * m_rollProportion;
	const float diffH = (heave- m_heave[0]) * m_heaveProportion;

	m_yaw[0] += diffY;
	m_pitch[0] += diffP;
	m_roll[0] += diffR;
	m_heave[0] += diffH;

	m_originalYaw = yaw;
	m_originalPitch = pitch;
	m_originalRoll = roll;
	m_originalHeave = heave;

	
	//---------------------------------------------------------------------------------------
	// Scaling
	m_yaw[1] =
		(m_yaw[0] * m_yaw[0] * m_yawC1) + (m_yaw[0] * m_yawC2) + m_yawC3;

	m_pitch[1] =
		(m_pitch[0] * m_pitch[0] * m_pitchC1) + (m_pitch[0] * m_pitchC2) + m_pitchC3;

	m_roll[1] =
		(m_roll[0] * m_roll[0] * m_rollC1) + (m_roll[0] * m_rollC2) + m_rollC3;

	m_heave[1] =
		(m_heave[0] * m_heave[0] * m_heaveC1) + (m_heave[0] * m_heaveC2) + m_heaveC3;
}


// 최종적으로 계산된 결과 값을 리턴한다.
void cMotionWaveModulator::GetFinal(OUT float &yaw, OUT float &pitch, OUT float &roll, OUT float &heave)
{
	// 최종 계산 결과 값은 마지막 인덱스에 저장된다.
	yaw = m_yaw[_countof(m_yaw) - 1];
	pitch = m_pitch[_countof(m_pitch) - 1];
	roll = m_roll[_countof(m_roll) - 1];
	heave = m_heave[_countof(m_heave) - 1];
}


void cMotionWaveModulator::GetOriginal(OUT float &yaw, OUT float &pitch, OUT float &roll, OUT float &heave)
{
	yaw = m_originalYaw;
	pitch = m_originalPitch;
	roll = m_originalRoll;
	heave = m_originalHeave;
}


// 변수 초기값 설정
void cMotionWaveModulator::InitDefault()
{
    m_incTime = 0;
    m_originalYaw = 0;
    m_originalPitch = 0;
    m_originalRoll = 0;
    m_originalHeave = 0;

    m_yawProportion = 0;
    m_pitchProportion = 0;
    m_rollProportion = 0;
    m_heaveProportion = 0;

	m_yawC1 = 0;
	m_yawC2 = 0;
	m_yawC3 = 0;

	m_pitchC1 = 0;
	m_pitchC2 = 255;
	m_pitchC3 = 255;

	m_rollC1 = 0;
	m_rollC2 = 255;
	m_rollC3 = 255;

	m_heaveC1 = 0;
	m_heaveC2 = 0;
	m_heaveC3 = 0;

}


void cMotionWaveModulator::UpdateParseData()
{
	m_incTime = 0;

	m_yawProportion = GetFloat("yaw_proportion");
	m_pitchProportion = GetFloat("pitch_proportion");
	m_rollProportion = GetFloat("roll_proportion");
	m_heaveProportion = GetFloat("heave_proportion");

	m_yawC1 = GetFloat("yaw_c1");
	m_yawC2 = GetFloat("yaw_c2");
	m_yawC3 = GetFloat("yaw_c3");

	m_pitchC1 = GetFloat("pitch_c1");
	m_pitchC2 = GetFloat("pitch_c2");
	m_pitchC3 = GetFloat("pitch_c3");

	m_rollC1 = GetFloat("roll_c1");
	m_rollC2 = GetFloat("roll_c2");
	m_rollC3 = GetFloat("roll_c3");

	m_heaveC1 = GetFloat("heave_c1");
	m_heaveC2 = GetFloat("heave_c2");
	m_heaveC3 = GetFloat("heave_c3");
}
