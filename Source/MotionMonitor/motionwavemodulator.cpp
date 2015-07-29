
#include "stdafx.h"
#include "motionwavemodulator.h"


cMotionWaveModulator::cMotionWaveModulator()
	: m_incTime(0)
	, m_totalIncTime(0)
	, m_originalYaw(0)
	, m_originalPitch(0)
	, m_originalRoll(0)
	, m_originalHeave(0)

	, m_isSplineEnable(false)

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

	, m_splinePlotSamplingRate(10)
	, m_splineInterpolationRate(10)

	, m_motionviewSplineSamplingRate(2)
	, m_motionviewSplineInterpolationRate(2)
	, m_motionviewTimeScaling(1)
	, m_motionviewStartDelay(0)

	, m_yawRecoverEnable(false)
	, m_yawRecoverTarget(256)
    , m_yawRecoverProportion(0.1f)
    , m_yawMaxDifferent(300)
	, m_yawLimit(3.1415f)
	, m_yawX2Propertion(0.1f)
	, m_yawX2Limit(1)

	, m_heaveRecoverEnable(false)
    , m_heaveRecoverTarget(256)
    , m_heaveRecoverProportion(0.1f)
    , m_heaveMaxDifferent(300)
	, m_heaveLimit(3.1415f)

	, m_pitchMaxDifferenceEnable(false)
	, m_pitchMaxDifferenceLimit(1)
	, m_pitchMaxDifferenceProportion(0.5)

	, m_rollMaxDifferenceEnable(false)
	, m_rollMaxDifferenceLimit(1)
	, m_rollMaxDifferenceProportion(0.5)

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
	m_totalIncTime += deltaSeconds;
	if (m_incTime <= 0.001f)
		return;

	//---------------------------------------------------------------------------------------
	// PID
	float diffY = (yaw - m_yaw[0]) * m_yawProportion;
	float diffP = (pitch - m_pitch[0]) * m_pitchProportion;
	float diffR = (roll - m_roll[0]) * m_rollProportion;
	float diffH = (heave- m_heave[0]) * m_heaveProportion;

	if (m_yawRecoverEnable)
	{
		diffY = (yaw - m_originalYaw) * m_yawProportion;
		if (abs(yaw - m_originalYaw) > m_yawMaxDifferent)
		{
			diffY = (diffY > 0) ? (yaw - m_originalYaw - m_yawLimit) * m_yawProportion : 
				(yaw - m_originalYaw + m_yawLimit) * m_yawProportion;
		}
		else if (abs(yaw - m_originalYaw) > m_yawX2Limit)
		{
			const float x2 = (yaw - m_originalYaw) * (yaw - m_originalYaw);
			diffY *= m_yawX2Propertion;
		}
	}


	if (m_pitchMaxDifferenceEnable)
	{
		if (abs(pitch - m_originalPitch) > m_pitchMaxDifferenceLimit)
		{
			diffP *= m_pitchMaxDifferenceProportion;
		}
	}


	if (m_rollMaxDifferenceEnable)
	{
		if (abs(roll - m_originalRoll) > m_rollMaxDifferenceLimit)
		{
			diffR *= m_rollMaxDifferenceProportion;
		}
	}

	m_yaw[0] += diffY;
	m_pitch[0] += diffP;
	m_roll[0] += diffR;
	m_heave[0] += diffH;

	m_originalYaw = yaw;
	m_originalPitch = pitch;
	m_originalRoll = roll;
	m_originalHeave = heave;

	//---------------------------------------------------------------------------------------
	// Yaw Recovery
	if (m_yawRecoverEnable)
	{
		const float recoverYaw = (m_yawRecoverTarget - m_yaw[0]) * m_yawRecoverProportion;
		m_yaw[0] += recoverYaw;
	}


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


	//---------------------------------------------------------------------------------------
	// Spline
	if (m_yawSpline.IsEnable())
		m_yawSpline.AddPoint(m_incTime, Vector2(m_totalIncTime, m_yaw[1]));
	if (m_pitchSpline.IsEnable())
		m_pitchSpline.AddPoint(m_incTime, Vector2(m_totalIncTime, m_pitch[1]));
	if (m_rollSpline.IsEnable())
		m_rollSpline.AddPoint(m_incTime, Vector2(m_totalIncTime, m_roll[1]));
	if (m_heaveSpline.IsEnable())
		m_heaveSpline.AddPoint(m_incTime, Vector2(m_totalIncTime, m_heave[1]));


	m_incTime = 0;
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
	m_totalIncTime = 0;

    m_originalYaw = 0;
    m_originalPitch = 0;
    m_originalRoll = 0;
    m_originalHeave = 0;

	m_isSplineEnable = false;

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

	m_splinePlotSamplingRate = 10;
	m_splineInterpolationRate = 10;

	m_motionviewSplineSamplingRate = 2;
	m_motionviewSplineInterpolationRate = 2;
	m_motionviewTimeScaling = 1;
	m_motionviewStartDelay = 0;

	m_yawRecoverEnable = false;
	m_yawRecoverTarget = 256;
	m_yawRecoverProportion = 0.1f;
	m_yawMaxDifferent = 300;

	m_heaveRecoverEnable = false;
	m_heaveRecoverTarget = 256;
	m_heaveRecoverProportion = 0.1f;
	m_heaveMaxDifferent = 300;

	m_pitchMaxDifferenceEnable = false;
	m_pitchMaxDifferenceLimit = 1;
	m_pitchMaxDifferenceProportion = 0.5f;

	m_rollMaxDifferenceEnable = false;
	m_rollMaxDifferenceLimit = 1;
	m_rollMaxDifferenceProportion = 0.5f;
}


void cMotionWaveModulator::UpdateParseData()
{
	m_incTime = 0;

	m_yawProportion = GetFloat("yaw_proportion");
	m_pitchProportion = GetFloat("pitch_proportion");
	m_rollProportion = GetFloat("roll_proportion");
	m_heaveProportion = GetFloat("heave_proportion");

	m_isSplineEnable = GetBool("spline_enable");

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

	m_splinePlotSamplingRate = GetInt("spline_plot_sampling_rate", 10);
	m_splineInterpolationRate = GetInt("spline_interpolation_rate", 10);

	m_motionviewSplineSamplingRate = GetInt("motionview_spline_sampling_rate", 2);
	m_motionviewSplineInterpolationRate = GetInt("motionview_spline_interpolation_rate", 2);
	m_motionviewTimeScaling = GetFloat("motionview_time_scaling", 1);
	m_motionviewStartDelay = GetInt("motionview_start_delay", 0);

	m_yawSpline.Init(m_isSplineEnable, m_splinePlotSamplingRate, m_splineInterpolationRate);
	m_pitchSpline.Init(m_isSplineEnable, m_splinePlotSamplingRate, m_splineInterpolationRate);
	m_rollSpline.Init(m_isSplineEnable, m_splinePlotSamplingRate, m_splineInterpolationRate);
	m_heaveSpline.Init(m_isSplineEnable, m_splinePlotSamplingRate, m_splineInterpolationRate);

	m_yawRecoverEnable = GetBool("yaw_recover_enable", false);
	m_yawRecoverTarget = GetFloat("yaw_recover_target");
	m_yawRecoverProportion = GetFloat("yaw_recover_proportion");
	m_yawMaxDifferent = GetFloat("yaw_max_different");
	m_yawLimit = GetFloat("yaw_limit");
	m_yawX2Propertion = GetFloat("yaw_x2_proportion", 0.1f);
	m_yawX2Limit = GetFloat("yaw_x2_limit", 1.f);

	m_heaveRecoverEnable = GetBool("heave_recover_enable", false);
	m_heaveRecoverTarget = GetFloat("heave_recover_target");
	m_heaveRecoverProportion = GetFloat("heave_recover_proportion");
	m_heaveMaxDifferent = GetFloat("yaw_x2_proportion");
	m_heaveLimit = GetFloat("heave_limit");

	m_pitchMaxDifferenceEnable = GetBool("pitch_max_difference_enable", false);
	m_pitchMaxDifferenceLimit = GetFloat("pitch_max_difference_limit", 1);
	m_pitchMaxDifferenceProportion = GetFloat("pitch_max_difference_proportion", 0.2f);

	m_rollMaxDifferenceEnable = GetBool("roll_max_difference_enable", false);
	m_rollMaxDifferenceLimit = GetFloat("roll_max_difference_limit", 1);
	m_rollMaxDifferenceProportion = GetFloat("roll_max_difference_proportion", 0.2f);

	m_rollMaxDifferenceEnable = false;
	m_rollMaxDifferenceLimit = 1;
	m_rollMaxDifferenceProportion = 0.5f;

}


// 스플라인 곡선 리턴
bool cMotionWaveModulator::GetSplineInterpolations(const float s, const float t, vector<Vector2> &yawOut, vector
	<Vector2> &pitchOut, vector<Vector2> &rollOut, vector<Vector2> &heaveOut)
{
	yawOut.reserve(4);
	pitchOut.reserve(4);
	rollOut.reserve(4);
	heaveOut.reserve(4);

	const bool ret1 = m_yawSpline.GetInterpolations(s, t, yawOut);
	const bool ret2 = m_pitchSpline.GetInterpolations(s, t, pitchOut);
	const bool ret3 = m_rollSpline.GetInterpolations(s, t, rollOut);
	const bool ret4 = m_heaveSpline.GetInterpolations(s, t, heaveOut);

	return ret1 && ret2 && ret3 && ret4;
}
