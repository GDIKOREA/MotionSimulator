
#include "stdafx.h"
#include "modulator.h"


cModulator::cModulator()
{
	m_incTime = 0;
	m_totalIncTime = 0;

	m_axis.recoverEnable = true;
	m_axis.maxDifferenceEnable = true;
	m_axis.Kp = 1.f;
	m_axis.Ki = 0;
	m_axis.Kd = 0;
	m_axis.oldDiff = 0;
	m_axis.incDiff = 0;
	m_axis.recoverTarget = 0;
	m_axis.recoverProportion = 0.01f;
	m_axis.maxDifference = MATH_PI;
	m_axis.range = MATH_PI * 2.f;

	m_axis.a = 0;
	m_axis.b = 1;
	m_axis.c = 0;
	ZeroMemory(m_axis.value, sizeof(m_axis));
}

cModulator::~cModulator()
{
}


// 변화값을 리턴한다.
// 범위 값을 넘어가서 반전 되는 경우, range, maxDiff 값을 이용해서 처리한다.
// x0 : 그 전 정보
// x1 : 현재 정보
// proportion : PID, P
// range : x0,x1 range
// maxDiff : 최대 변화 값
// maxDiffProportion : 최대 변화값을 넘었을 때 proportion 값 정의
// x2Proportion : 변화값의 제곱의 비율 값
float cModulator::GetDifference(const float x0, const float x1,
	const float proportionK, const float integralK, const float differenceK,
	const float range, const float maxDiff, const float maxDiffProportion)
{
	float diff = (x1 - x0) * proportionK;

	if (abs(x1 - x0) > maxDiff)
	{
		if (x1 * x0 < 0)
		{// 극성이 다를 때.. 즉 limit 값을 넘겼을 때..
			// limit 를 넘긴 값을 보정한다.

			float newDiff = range - abs(x1 - x0);
			if (x1 > 0)
				newDiff = -newDiff;

			diff = newDiff * proportionK;
		}
		else
		{// 같은 극성일 때.. 큰 변화가 일어 났을 때..

			// proportion을 한 번더 적용한다.
			diff = diff * maxDiffProportion;
		}
	}

	return diff;
}


float cModulator::GetManufactureValue(const float x0, const float x1,
	cModulator::sAxisOption &option)
{
	float diff = (x1 - x0);
	if (abs(x1 - x0) > option.maxDifference)
	{
		if (x1 * x0 < 0)
		{// 극성이 다를 때.. 즉 limit 값을 넘겼을 때..
			// limit 를 넘긴 값을 보정한다.

			float newDiff = option.range - abs(x1 - x0);
			if (x1 > 0)
				newDiff = -newDiff;

			diff = newDiff * option.Kp;
		}
		// 		else
		// 		{// 같은 극성일 때.. 큰 변화가 일어 났을 때..
		// 
		// 			// proportion을 한 번더 적용한다.
		// 			diff = diff * option.maxDifferenceProportion;
		// 		}
	}

	float ndiff = diff - option.oldDiff;
	float mv = option.Kp * diff + option.Kd * ndiff + option.Ki * option.incDiff;
	option.oldDiff = diff;
	option.incDiff += diff;

	return mv;
}


// 변화값을 리턴한다.
float cModulator::GetNormalDifference(const float x0, const float x1, const float proportion)
{
	float diff = (x1 - x0) * proportion;
	return diff;
}


// x에서 target으로 복귀하는 정도를 리턴한다.
float cModulator::GetRecoverValue(const float x, const float target, const float proportion)
{
	return (target - x) * proportion;
}


void cModulator::Update(const float deltaSeconds, const float x)
{
	m_incTime += deltaSeconds;
	m_totalIncTime += deltaSeconds;
	if (m_incTime <= 0.001f)
		return;

	const int axisLen = sizeof(m_axis) / sizeof(sAxisOption);

	//---------------------------------------------------------------------------------------
	// PID
	float diff = 0;
	if (m_axis.maxDifferenceEnable)
	{
		diff = GetManufactureValue(m_axis.value[0], x, m_axis);
		m_axis.value[1] += diff;
		m_axis.value[1] = common::clamp(-MATH_PI, MATH_PI, m_axis.value[1]);
	}
	else
	{
		diff = GetNormalDifference(m_axis.value[1], x, m_axis.Kp);
		m_axis.value[1] += diff;
	}

	m_axis.value[0] = x;


	//---------------------------------------------------------------------------------------
	// Recovery
	if (m_axis.recoverEnable)
		m_axis.value[1] += GetRecoverValue(m_axis.value[1], m_axis.recoverTarget, 
		m_axis.recoverProportion);

	//---------------------------------------------------------------------------------------
	// Scaling
	const float v = m_axis.value[1];
	m_axis.value[2] = (v * v * m_axis.a) + (v * m_axis.b) + m_axis.c;


	//---------------------------------------------------------------------------------------
	// Spline
	if (m_spline.IsEnable())
		m_spline.AddPoint(m_incTime, Vector2(m_totalIncTime, m_axis.value[2]));

	m_incTime = 0;

}


void cModulator::GetFinal(OUT float &out)
{
	out = m_axis.value[_countof(m_axis.value) - 1];
}


void cModulator::GetOriginal(OUT float &out)
{
	out = m_axis.value[0];
}
