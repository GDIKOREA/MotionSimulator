
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
{
	const int len = sizeof(m_axis) / sizeof(sAxisOption);
	for (int i = 0; i < len; ++i)
	{
		m_axis[i].recoverEnable = true;
		m_axis[i].maxDifferenceEnable = true;
		m_axis[i].Kp = 1.f;
		m_axis[i].Ki = 0;
		m_axis[i].Kd = 0;
		m_axis[i].oldDiff = 0;
		m_axis[i].incDiff = 0;
		m_axis[i].recoverTarget = 0;
		m_axis[i].recoverProportion = 0.01f;
		m_axis[i].maxDifference = MATH_PI;
		m_axis[i].range = MATH_PI * 2.f;

		m_axis[i].a = 0;
		m_axis[i].b = 1;
		m_axis[i].c = 0;
		ZeroMemory(m_axis[i].value, sizeof(m_axis[i]));
	}
}

cMotionWaveModulator::~cMotionWaveModulator()
{
}



// ��ȭ���� �����Ѵ�.
// ���� ���� �Ѿ�� ���� �Ǵ� ���, range, maxDiff ���� �̿��ؼ� ó���Ѵ�.
// x0 : �� �� ����
// x1 : ���� ����
// proportion : PID, P
// range : x0,x1 range
// maxDiff : �ִ� ��ȭ ��
// maxDiffProportion : �ִ� ��ȭ���� �Ѿ��� �� proportion �� ����
// x2Proportion : ��ȭ���� ������ ���� ��
float GetDifference(const float x0, const float x1,
	const float proportionK, const float integralK, const float differenceK, 
	const float range, const float maxDiff, const float maxDiffProportion)
{
	float diff = (x1 - x0) * proportionK;

	if (abs(x1 - x0) > maxDiff)
	{
		if (x1 * x0 < 0)
		{// �ؼ��� �ٸ� ��.. �� limit ���� �Ѱ��� ��..
			// limit �� �ѱ� ���� �����Ѵ�.

			float newDiff = range - abs(x1 - x0);
			if (x1 > 0)
				newDiff = -newDiff;

			diff = newDiff * proportionK;
		}
		else
		{// ���� �ؼ��� ��.. ū ��ȭ�� �Ͼ� ���� ��..

			// proportion�� �� ���� �����Ѵ�.
			diff = diff * maxDiffProportion;
		}
	}

	return diff;
}


float GetManufactureValue(const float x0, const float x1,
	cMotionWaveModulator::sAxisOption &option)
{
	float diff = (x1 - x0);
	if (abs(x1 - x0) > option.maxDifference)
	{
		if (x1 * x0 < 0)
		{// �ؼ��� �ٸ� ��.. �� limit ���� �Ѱ��� ��..
			// limit �� �ѱ� ���� �����Ѵ�.

			float newDiff = option.range - abs(x1 - x0);
			if (x1 > 0)
				newDiff = -newDiff;

			diff = newDiff * option.Kp;
		}
// 		else
// 		{// ���� �ؼ��� ��.. ū ��ȭ�� �Ͼ� ���� ��..
// 
// 			// proportion�� �� ���� �����Ѵ�.
// 			diff = diff * option.maxDifferenceProportion;
// 		}
	}

	float ndiff = diff - option.oldDiff;
	float mv = option.Kp * diff + option.Kd * ndiff + option.Ki * option.incDiff;
	option.oldDiff = diff;
	option.incDiff += diff;

	return mv;
}


// ��ȭ���� �����Ѵ�.
float GetNormalDifference(const float x0, const float x1, const float proportion)
{
	float diff = (x1 - x0) * proportion;
	return diff;
}


// x���� target���� �����ϴ� ������ �����Ѵ�.
float GetRecoverValue(const float x, const float target, const float proportion)
{
	return (target - x) * proportion;
}


// ������ ������Ʈ �Ѵ�.
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
		float diff = 0;
		if (m_axis[i].maxDifferenceEnable)
		{
			diff = GetManufactureValue(m_axis[i].value[0], value[i], m_axis[i]);
			m_axis[i].value[1] += diff;
			m_axis[i].value[1] = common::clamp(-MATH_PI, MATH_PI, m_axis[i].value[1]);
		}
		else
		{
			diff = GetNormalDifference(m_axis[i].value[1], value[i], m_axis[i].Kp);
			m_axis[i].value[1] += diff;
		}

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



// ���������� ���� ��� ���� �����Ѵ�.
void cMotionWaveModulator::GetFinal(OUT float &yaw, OUT float &pitch, OUT float &roll, OUT float &heave)
{
	// ���� ��� ��� ���� ������ �ε����� ����ȴ�.
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


// ���� �ʱⰪ ����
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
		m_axis[i].range = MATH_PI * 2.f;
	}

}


void cMotionWaveModulator::UpdateParseData()
{
	m_incTime = 0;

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

	string axis[4] = { "yaw", "pitch", "roll", "heave" };
	for (int i = 0; i < 4; ++i)
	{
		m_axis[i].Kp = GetFloat(axis[i] + "_kp", 1.f);
		m_axis[i].Ki = GetFloat(axis[i] + "_ki", 0.f);
		m_axis[i].Kd = GetFloat(axis[i] + "_kd", 0.f);
		m_axis[i].oldDiff = 0;
		m_axis[i].incDiff = 0;

		m_axis[i].recoverEnable = GetBool(axis[i] + "_recover_enable", true);
		m_axis[i].recoverTarget = GetFloat(axis[i] + "_recover_target", 0);
		m_axis[i].recoverProportion = GetFloat(axis[i] + "_recover_proportion", 0.01f);

		m_axis[i].maxDifferenceEnable = GetBool(axis[i] + "_max_difference_enable", true);
		m_axis[i].maxDifference = GetFloat(axis[i] + "_max_difference", MATH_PI);
		m_axis[i].maxDifferenceProportion = GetFloat(axis[i] + "_max_difference_proportion", 0.1f);
		m_axis[i].range = GetFloat(axis[i] + "_range", MATH_PI * 2.f);
	}

}


// ���ö��� � ����
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
