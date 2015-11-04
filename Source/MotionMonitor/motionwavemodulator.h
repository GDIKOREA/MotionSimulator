//
// 모션파형을 변조하는 기능을 한다.
//
#pragma once

#include "spline.h"


class cMotionWaveModulator : public common::cConfig
{
public:
	cMotionWaveModulator();
	virtual ~cMotionWaveModulator();

	void Update(const float deltaSeconds, const float yaw, const float pitch, const float roll, const float heave);
	void GetFinal(OUT float &yaw, OUT float &pitch, OUT float &roll, OUT float &heave);
	void GetOriginal(OUT float &yaw, OUT float &pitch, OUT float &roll, OUT float &heave);
	bool GetSplineInterpolations(const float s, const float t, vector<Vector2> &yawOut, vector<Vector2> &pitchOut,
		vector<Vector2> &rollOut, vector<Vector2> &heaveOut);

	virtual void InitDefault() override;
	virtual void UpdateParseData() override;


public:
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
	sAxisOption m_axis[4]; // yaw-pitch-roll-heave


	// Spline계산을 위해 4포인트의 값을 저장한다.
	cSpline m_spline[4];
	
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

