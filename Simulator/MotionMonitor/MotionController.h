//
//  모션 제어 계산을 한다.
//
#pragma once


class cMotionController : public common::cSingleton<cMotionController>
{
public:
	cMotionController();
	virtual ~cMotionController();

	void SetMotion(const float yaw, const float pitch, const float roll);
	void GetMotion(float &yaw, float &pitch, float &roll);
	void GetOriginalMotion(float &yaw, float &pitch, float &roll);

	void SetMaxYawDerivation(const float deriv);
	void SetMaxPitchDerivation(const float deriv);
	void SetMaxRollDerivation(const float deriv);


protected:
	float m_yaw;
	float m_pitch;
	float m_roll;

	float m_originalYaw;
	float m_originalPitch;
	float m_originalRoll;

	float m_maxYawDeriv;
	float m_maxPitchDeriv;
	float m_maxRollDeriv;
};


inline void cMotionController::SetMaxYawDerivation(const float deriv) { m_maxYawDeriv = deriv;  }
inline void cMotionController::SetMaxPitchDerivation(const float deriv) { m_maxPitchDeriv = deriv;  }
inline void cMotionController::SetMaxRollDerivation(const float deriv) { m_maxRollDeriv = deriv; }

