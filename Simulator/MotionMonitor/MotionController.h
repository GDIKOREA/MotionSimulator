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

	void SetYawProportion(const float prop);
	void SetPitchProportion(const float prop);
	void SetRollProportion(const float prop);

	void SetYawScale(const float scale);
	void SetPitchScale(const float scale);
	void SetRollScale(const float scale);

	void SetYawScale2(const float scale);
	void SetPitchScale2(const float scale);
	void SetRollScale2(const float scale);

	void GetProportion(OUT float &yaw, OUT float &pitch, OUT float &roll);
	void GetScale(OUT float &yaw, OUT float &pitch, OUT float &roll);
	void GetScale2(OUT float &yaw, OUT float &pitch, OUT float &roll);


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

	float m_YawProportion;
	float m_PitchProportion;
	float m_RollProportion;

	float m_YawScale;
	float m_PitchScale;
	float m_RollScale;

	float m_YawScale2;
	float m_PitchScale2;
	float m_RollScale2;
};


inline void cMotionController::SetMaxYawDerivation(const float deriv) { m_maxYawDeriv = deriv;  }
inline void cMotionController::SetMaxPitchDerivation(const float deriv) { m_maxPitchDeriv = deriv;  }
inline void cMotionController::SetMaxRollDerivation(const float deriv) { m_maxRollDeriv = deriv; }
inline void cMotionController::SetYawProportion(const float prop) { m_YawProportion = prop;  }
inline void cMotionController::SetPitchProportion(const float prop) { m_PitchProportion = prop; }
inline void cMotionController::SetRollProportion(const float prop) { m_RollProportion = prop; }
inline void cMotionController::SetYawScale(const float scale) { m_YawScale = scale; }
inline void cMotionController::SetPitchScale(const float scale) { m_PitchScale = scale; }
inline void cMotionController::SetRollScale(const float scale) { m_RollScale = scale; }
inline void cMotionController::SetYawScale2(const float scale) { m_YawScale2 = scale; }
inline void cMotionController::SetPitchScale2(const float scale) { m_PitchScale2 = scale; }
inline void cMotionController::SetRollScale2(const float scale) { m_RollScale2 = scale; }
