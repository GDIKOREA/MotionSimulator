//
//
// Dirt3 ��� ��Ʈ�ѷ�
//
//
#pragma once

#include "vitconmotionsim2.h"


class cDirt3Controller : public common::cSingleton<cDirt3Controller>
{
public:
	cDirt3Controller();
	virtual ~cDirt3Controller();

	enum STATE {
		OFF,				// �����ġ Off
		OFF_PROCESS,  // �����ġ On, ��� ��ġ ���� �غ� ��
		READY,		// �����ġ On, UDP ������ �޾� ��� ��ġ�� ������ �غ� �� ����
		PLAY,			// �����ġ On, UDP ������ �޾� ��� ��ġ�� �����̴� ����
		TIMEUPSTOP,	// �����ġ On, Ư�� ���ǿ� ���� �����ġ�� Stop ���� ���� (�ð� �ʰ�, ���� Stop)
	};

	void StartMotionSim(const string &configFileName, const bool isStartMotionSimOut=false);
	void StopMotionSim();
	void Update(const float deltaSeconds);
	void UpdateUDP(const char *buffer, const int bufferLen);
	cVitconMotionSim2& GetMotionSim();
	STATE GetState() const;
	string GetStateStr() const;

	
protected:
	STATE m_state;
	cVitconMotionSim2 m_vitconMotionSim;
	cVitconMotionSim2::STATE m_oldState;
	float m_lastUDPUpdateTime; // ���� �ֱٿ� UDP ��Ŷ�� ���� �ð��� �����Ѵ�.
	bool m_isLapTimeProgress; // @laptime �� �����Ǳ� �����ϸ�, true �� �ȴ�.
	float m_lastLapTime; // �� �������� lap time
};


inline cVitconMotionSim2& cDirt3Controller::GetMotionSim() { return m_vitconMotionSim; }
inline cDirt3Controller::STATE cDirt3Controller::GetState() const { return m_state;  }
