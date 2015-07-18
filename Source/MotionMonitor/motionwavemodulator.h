//
// 모션파형을 변조하는 기능을 한다.
//
#pragma once


class cMotionWaveModulator : public common::cConfig
{
public:
	cMotionWaveModulator();
	virtual ~cMotionWaveModulator();


	bool ReadConfig(const string &fileName);
	bool ReadConfigStr(const string &str);

	void Update(const float deltaSeconds, const float yaw, const float pitch, const float roll, const float heave);
	void GetFinal(OUT float &yaw, OUT float &pitch, OUT float &roll, OUT float &heave);
	void GetOriginal(OUT float &yaw, OUT float &pitch, OUT float &roll, OUT float &heave);

	virtual void InitDefault() override;
	virtual void UpdateParseData() override;
	

public:
	// 최종 계산 결과 값은 마지막 인덱스에 저장된다.
	float m_yaw[2];
	float m_pitch[2];
	float m_roll[2];
	float m_heave[2];

	float m_originalYaw;
	float m_originalPitch;
	float m_originalRoll;
	float m_originalHeave;

	float m_yawProportion;
	float m_pitchProportion;
	float m_rollProportion;
	float m_heaveProportion;


	// (m_yawC1 * m_yaw[0]* m_yaw[0]) + (m_yawC2 * m_yaw[0]) + m_yawC3
	float m_yawC1;
	float m_yawC2;
	float m_yawC3;

	float m_pitchC1;
	float m_pitchC2;
	float m_pitchC3;

	float m_rollC1;
	float m_rollC2;
	float m_rollC3;

	float m_heaveC1;
	float m_heaveC2;
	float m_heaveC3;

	float m_incTime;
};
