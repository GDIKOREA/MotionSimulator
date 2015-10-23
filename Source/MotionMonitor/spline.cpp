
#include "stdafx.h"
#include "spline.h"


cSpline::cSpline()
	: m_enable(false)
	, m_isReadEnable(false)
	, m_storeCount(0)
	, m_samplingRate(1)
	, m_samplingTime(1)
	, m_interplationRate(1)
	, m_interpoationStep(0.1f)
	, m_incTime(0)
{

}

cSpline::~cSpline()
{

}


void cSpline::Init(const bool enable, const int samplingRate, const int interpolationRate)
{
	m_enable = enable;
	m_samplingRate = samplingRate;
	m_interplationRate = interpolationRate;
	m_samplingTime = 1.f / (float)samplingRate;
	m_interpoationStep = 1.f / (float)interpolationRate;
	m_storeCount = 0;
	m_incTime = 0;

	m_vec.resize(4); // 4�� ����Ʈ�� �����Ѵ�.
}


// ���ø� �ð��� �����ؼ� ���� �����Ѵ�.
bool cSpline::AddPoint(const float deltaSeconds, const Vector2 &pos)
{
	RETV(!m_enable, false);
	
	bool retValue = false;
	m_incTime += deltaSeconds;

	if (m_incTime > m_samplingTime)
	{
		m_incTime = 0;

		if (m_storeCount < 4)
		{
			m_vec[m_storeCount++] = pos;
		}
		else
		{
			// ó�� ������ �����, �������� ����Ʈ �Ѵ�.
			//std::rotate(m_vec.begin(), m_vec.begin() + 1, m_vec.end());
			m_vec[0] = m_vec[1];
			m_vec[1] = m_vec[2];
			m_vec[2] = m_vec[3];
			m_vec[3] = pos;

			// Spline ��� ����� �� ���� �� true�� �����Ѵ�.
			retValue = true;
			m_isReadEnable = true;
		}
	}

	return retValue;
}


// ���ø� ���ݿ� ������� �����Ѵ�.
void cSpline::AddPoint(const Vector2 &pos)
{
	RET(!m_enable);

	if (m_storeCount < 4)
	{
		m_vec[m_storeCount++] = pos;
	}
	else
	{
		// ó�� ������ �����, �������� ����Ʈ �Ѵ�.
		std::rotate(m_vec.begin(), m_vec.begin() + 1, m_vec.end());
		m_vec[3] = pos;

		// Spline ��� ����� �� ���� �� true�� �����Ѵ�.
		m_isReadEnable = true;
	}
}


Vector2 cSpline::GetInterpolation(const float t)
{
	if (m_storeCount < 4)
		return m_vec[m_storeCount];

	Vector2 p;
	D3DXVec2CatmullRom((D3DXVECTOR2*)&p,
		(D3DXVECTOR2*)&m_vec[0],
		(D3DXVECTOR2*)&m_vec[1],
		(D3DXVECTOR2*)&m_vec[2],
		(D3DXVECTOR2*)&m_vec[3], t);

	return p;
}


// s ���� t ������ ����Ʈ�� out�� ������ �����Ѵ�.
bool cSpline::GetInterpolations(const float s, const float t, vector<Vector2> &out)
{
	RETV(!m_isReadEnable, false);

	float m = s;
	while (m < t)
	{
		Vector2 p;
		p = GetInterpolation(m);
		out.push_back(p);

		m += m_interpoationStep;
	}

	m_isReadEnable = false; // �� �� �а� �� �Ŀ���, samplingTime�� �����Ŀ� ���� �� �ִ�.
	return true;
}
