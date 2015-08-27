
#include "stdafx.h"
#include "motionwavemodulator.h"


cMotionWaveModulator::cMotionWaveModulator()
	: m_incTime(0)
	, m_totalIncTime(0)
	, m_isSplineEnable(false)
	, m_splinePlotSamplingRate(10)
	, m_splineInterpolationRate(10)

	, m_motionviewSplineSamplingRate(2)
	, m_motionviewSplineInterpolationRate(2)
	, m_motionviewTimeScaling(1)
	, m_motionviewStartDelay(0)

	, m_yawX2Propertion(0.1f)
	, m_yawX2Limit(1)
{
	const int len = sizeof(m_axis) / sizeof(sAxisOption);
	for (int i = 0; i < len; ++i)
	{
		m_axis[i].recoverEnable = true;
		m_axis[i].maxDifferenceEnable = true;
		m_axis[i].proportion = 1.f;
		m_axis[i].recoverTarget = 0;
		m_axis[i].recoverProportion = 0.01f;
		m_axis[i].maxDifference = MATH_PI;
		m_axis[i].limit = MATH_PI * 2.f;

		m_axis[i].a = 0;
		m_axis[i].b = 1;
		m_axis[i].c = 0;
		ZeroMemory(m_axis[i].value, sizeof(m_axis[i]));
	}

}

cMotionWaveModulator::~cMotionWaveModulator()
{
}


float climp(const float _min, const float _max, const float val)
{
	if (val < _min)
		return _min;
	if (val > _max)
		return _max;
	return val;
}


// 변화값을 리턴한다.
// x0 그 전 정보
// x1 현재 정보
// proportion 비율
// limit x0,x1 range
// maxDiff 최대 변화 값
float GetDifference(const float x0, const float x1,
	const float proportion, const float limit, const float maxDiff)
{
	float diff = (x1 - x0) * proportion;
	if (abs(x1 - x0) > maxDiff)
	{
		if (x1 * x0 < 0)
		{// 극성이 다를 때.. 즉 limit 값을 넘겼을 때..
			// limit 를 넘긴 값을 보정한다.

			float newDiff = limit - abs(x1 - x0);
			if (x1 > 0)
				newDiff = -newDiff;

			diff = newDiff * proportion;
		}
		else
		{// 같은 극성일 때.. 큰 변화가 일어 났을 때..

			// proportion을 한 번더 적용한다.
			diff = diff * proportion;
		}
	}

	return diff;
}


// x에서 target으로 복귀하는 정도를 리턴한다.
float GetRecoverValue(const float x, const float target, const float proportion)
{
	return (target - x) * proportion;
}



// 정보를 업데이트 한다.
void cMotionWaveModulator::Update(const float deltaSeconds,
	const float yaw, const float pitch, const float roll, const float heave)
{
	m_incTime += deltaSeconds;
	m_totalIncTime += deltaSeconds;
	if (m_incTime <= 0.001f)
		return;

	const int axisLen = sizeof(m_axis) / sizeof(sAxisOption);

	//---------------------------------------------------------------------------------------
	// PID
	float value[4] = { yaw, pitch, roll, heave };
	for (int i = 0; i < axisLen; ++i)
	{
		const float diff = GetDifference(m_axis[i].value[0], value[i], m_axis[i].proportion, m_axis[i].limit, m_axis[i].maxDifference);
		m_axis[i].value[1] += diff;
		m_axis[i].value[1] = climp(-MATH_PI, MATH_PI, m_axis[i].value[1]);
		m_axis[i].value[0] = value[i];
	}


	//---------------------------------------------------------------------------------------
	// Recovery
	for (int i = 0; i < axisLen; ++i)
	{
		if (m_axis[i].recoverEnable)
			m_axis[i].value[1] += GetRecoverValue(m_axis[i].value[1], m_axis[i].recoverTarget, m_axis[i].recoverProportion);
	}

	//---------------------------------------------------------------------------------------
	// Scaling
	for (int i = 0; i < axisLen; ++i)
	{
		const float x = m_axis[i].value[1];
		m_axis[i].value[2] = (x * x * m_axis[i].a) + (x * m_axis[i].b) + m_axis[i].c;
	}


	//---------------------------------------------------------------------------------------
	// Spline
	for (int i = 0; i < axisLen; ++i)
		if (m_spline[i].IsEnable())
			m_spline[i].AddPoint(m_incTime, Vector2(m_totalIncTime, m_axis[i].value[2]));

	m_incTime = 0;
}



// 최종적으로 계산된 결과 값을 리턴한다.
void cMotionWaveModulator::GetFinal(OUT float &yaw, OUT float &pitch, OUT float &roll, OUT float &heave)
{
	// 최종 계산 결과 값은 마지막 인덱스에 저장된다.
	yaw = m_axis[0].value[_countof(m_axis[0].value) - 1];
	pitch = m_axis[1].value[_countof(m_axis[0].value) - 1];
	roll = m_axis[2].value[_countof(m_axis[0].value) - 1];
	heave = m_axis[3].value[_countof(m_axis[0].value) - 1];
}


void cMotionWaveModulator::GetOriginal(OUT float &yaw, OUT float &pitch, OUT float &roll, OUT float &heave)
{
	yaw = m_axis[0].value[0];
	pitch = m_axis[1].value[0];
	roll = m_axis[2].value[0];
	heave = m_axis[3].value[0];
}


// 변수 초기값 설정
void cMotionWaveModulator::InitDefault()
{
    m_incTime = 0;
	m_totalIncTime = 0;

	m_isSplineEnable = false;
	
	m_splinePlotSamplingRate = 10;
	m_splineInterpolationRate = 10;

	m_motionviewSplineSamplingRate = 2;
	m_motionviewSplineInterpolationRate = 2;
	m_motionviewTimeScaling = 1;
	m_motionviewStartDelay = 0;
	
	const int len = sizeof(m_axis) / sizeof(sAxisOption);
	for (int i = 0; i < len; ++i)
	{
		m_axis[i].recoverEnable = true;
		m_axis[i].maxDifferenceEnable = true;
		m_axis[i].recoverTarget = 0;
		m_axis[i].recoverProportion = 0.01f;
		m_axis[i].maxDifference = MATH_PI;
		m_axis[i].limit = MATH_PI * 2.f;
	}

}


void cMotionWaveModulator::UpdateParseData()
{
	m_incTime = 0;

	m_axis[0].proportion = GetFloat("yaw_proportion");
	m_axis[1].proportion = GetFloat("pitch_proportion");
	m_axis[2].proportion = GetFloat("roll_proportion");
	m_axis[3].proportion = GetFloat("heave_proportion");

	m_isSplineEnable = GetBool("spline_enable");

	m_axis[0].a = GetFloat("yaw_c1");
	m_axis[0].b = GetFloat("yaw_c2");
	m_axis[0].c = GetFloat("yaw_c3");

	m_axis[1].a = GetFloat("pitch_c1");
	m_axis[1].b = GetFloat("pitch_c2");
	m_axis[1].c = GetFloat("pitch_c3");

	m_axis[2].a = GetFloat("roll_c1");
	m_axis[2].b = GetFloat("roll_c2");
	m_axis[2].c = GetFloat("roll_c3");

	m_axis[3].a = GetFloat("heave_c1");
	m_axis[3].b = GetFloat("heave_c2");
	m_axis[3].c = GetFloat("heave_c3");

	m_splinePlotSamplingRate = GetInt("spline_plot_sampling_rate", 10);
	m_splineInterpolationRate = GetInt("spline_interpolation_rate", 10);

	m_motionviewSplineSamplingRate = GetInt("motionview_spline_sampling_rate", 2);
	m_motionviewSplineInterpolationRate = GetInt("motionview_spline_interpolation_rate", 2);
	m_motionviewTimeScaling = GetFloat("motionview_time_scaling", 1);
	m_motionviewStartDelay = GetInt("motionview_start_delay", 0);

	m_spline[0].Init(m_isSplineEnable, m_splinePlotSamplingRate, m_splineInterpolationRate);
	m_spline[1].Init(m_isSplineEnable, m_splinePlotSamplingRate, m_splineInterpolationRate);
	m_spline[2].Init(m_isSplineEnable, m_splinePlotSamplingRate, m_splineInterpolationRate);
	m_spline[3].Init(m_isSplineEnable, m_splinePlotSamplingRate, m_splineInterpolationRate);

	m_yawX2Propertion = GetFloat("yaw_x2_proportion", 0.1f);
	m_yawX2Limit = GetFloat("yaw_x2_limit", 1.f);	

	m_axis[0].recoverEnable = GetBool("yaw_recover_enable", true);
	m_axis[0].maxDifferenceEnable = GetBool("yaw_max_difference_enable", true);
	m_axis[0].recoverTarget = GetFloat("yaw_recover_target", 0);
	m_axis[0].recoverProportion = GetFloat("yaw_recover_proportion", 0.01f);
	m_axis[0].maxDifference = GetFloat("yaw_max_difference", MATH_PI);
	m_axis[0].limit = GetFloat("yaw_limit", MATH_PI * 2.f);

	m_axis[1].recoverEnable = GetBool("pitch_recover_enable", true);
	m_axis[1].maxDifferenceEnable = GetBool("pitch_max_difference_enable", true);
	m_axis[1].recoverTarget = GetFloat("pitch_recover_target", 0);
	m_axis[1].recoverProportion = GetFloat("pitch_recover_proportion", 0.01f);
	m_axis[1].maxDifference = GetFloat("pitch_max_difference", MATH_PI);
	m_axis[1].limit = GetFloat("pitch_limit", MATH_PI * 2.f);

	m_axis[2].recoverEnable = GetBool("roll_recover_enable", true);
	m_axis[2].maxDifferenceEnable = GetBool("roll_max_difference_enable", true);
	m_axis[2].recoverTarget = GetFloat("roll_recover_target", 0);
	m_axis[2].recoverProportion = GetFloat("roll_recover_proportion", 0.01f);
	m_axis[2].maxDifference = GetFloat("roll_max_difference", MATH_PI);
	m_axis[2].limit = GetFloat("roll_limit", MATH_PI * 2.f);

	m_axis[3].recoverEnable = GetBool("heave_recover_enable", true);
	m_axis[3].maxDifferenceEnable = GetBool("heave_max_difference_enable", true);
	m_axis[3].recoverTarget = GetFloat("heave_recover_target", 0);
	m_axis[3].recoverProportion = GetFloat("heave_recover_proportion", 0.01f);
	m_axis[3].maxDifference = GetFloat("heave_max_difference", 1000);
	m_axis[3].limit = GetFloat("heave_limit", 100000000);
	
}


// 스플라인 곡선 리턴
bool cMotionWaveModulator::GetSplineInterpolations(const float s, const float t, vector<Vector2> &yawOut, vector
	<Vector2> &pitchOut, vector<Vector2> &rollOut, vector<Vector2> &heaveOut)
{
	yawOut.reserve(4);
	pitchOut.reserve(4);
	rollOut.reserve(4);
	heaveOut.reserve(4);

	const bool ret1 = m_spline[0].GetInterpolations(s, t, yawOut);
	const bool ret2 = m_spline[1].GetInterpolations(s, t, pitchOut);
	const bool ret3 = m_spline[2].GetInterpolations(s, t, rollOut);
	const bool ret4 = m_spline[3].GetInterpolations(s, t, heaveOut);

	return ret1 && ret2 && ret3 && ret4;
}
