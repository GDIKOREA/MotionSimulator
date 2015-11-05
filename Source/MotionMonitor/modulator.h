#pragma once


class cModulator
{
public:
	cModulator();
	virtual ~cModulator();

	struct sAxisOption
	{
		// pid control
		float Kp; // p
		float Ki; // i
		float Kd; // d
		float oldDiff;
		float incDiff;
		//

		bool recoverEnable;
		float recoverTarget;
		float recoverProportion;

		bool maxDifferenceEnable;
		float maxDifference;
		float maxDifferenceProportion;
		float range;

		// ax^2 + bx + c
		float a;
		float b;
		float c;
		float value[3]; // original, pid, final
	};


	void Update(const float deltaSeconds, const float x);
	void GetFinal(OUT float &out);
	void GetOriginal(OUT float &out);


protected:
	float GetDifference(const float x0, const float x1,
		const float proportionK, const float integralK, const float differenceK,
		const float range, const float maxDiff, const float maxDiffProportion);
	float GetManufactureValue(const float x0, const float x1,
		cModulator::sAxisOption &option);
	float GetNormalDifference(const float x0, const float x1, const float proportion);
	float GetRecoverValue(const float x, const float target, const float proportion);


public:
	sAxisOption m_axis;


	// Spline����� ���� 4����Ʈ�� ���� �����Ѵ�.
	cSpline m_spline;

	// spline
	bool m_isSplineEnable;
	int m_splinePlotSamplingRate;
	int m_splineInterpolationRate;

	// motion wave
	int m_motionviewSplineSamplingRate;
	int m_motionviewSplineInterpolationRate;
	float m_motionviewTimeScaling;
	int m_motionviewStartDelay;

	float m_incTime;
	float m_totalIncTime;
};
