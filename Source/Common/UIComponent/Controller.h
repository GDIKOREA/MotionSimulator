//
// ���� ���� ��ü
//
// ť�� ����� ���α׷��� �����Ѵ�.
//
#pragma once

#include "UpdateObserver.h"
#include "UDPObserver.h"


class cCubeFlight;
class cSerialCommunication;
class cUDPCommunication;
class cController : public common::cSingleton<cController>
							, public cUpdateObservable
							, public cUDPObservable
{
public:
	cController();
	virtual ~cController();

	bool Init(graphic::cRenderer &renderer);
	bool ConnectSerial(const int portNum, const int baudRate);
	void CloseSerial();
	void Update(const float deltaSeconds);

	float GetGlobalSeconds() const;
	float GetActuatorSpeed() const;
	void SetActuatorSpeed(const float speed);
	float GetActuatorYawPower() const;
	void SetActuatorYawPower(const float power);

	float GetPlayTime() const;
	void SetPlayTime(const float t);

	cSerialCommunication& GetSerialComm();
	cUDPCommunication& GetUDPComm();


protected:
	cSerialCommunication *m_serialComm;
	cUDPCommunication *m_udpComm;
	float m_globalSeconds;
	float m_actuatorSpeed; // 0~1 ���� ��
	float m_actuatorYawPower; // 0~1 ���� ��
	float m_playTime; // �� ���Ӵ� �÷��� �� �� �ִ� �ð�. seconds ����, default : 60
};


inline cSerialCommunication& cController::GetSerialComm() { return *m_serialComm;  }
inline cUDPCommunication& cController::GetUDPComm() { return *m_udpComm; }
inline float cController::GetGlobalSeconds() const { return m_globalSeconds; }
inline float cController::GetActuatorSpeed() const { return m_actuatorSpeed; }
inline void cController::SetActuatorSpeed(const float speed) { m_actuatorSpeed = speed; }
inline float cController::GetPlayTime() const { return m_playTime;  }
inline void cController::SetPlayTime(const float t) { m_playTime = t; }
inline float cController::GetActuatorYawPower() const { return m_actuatorYawPower; }
inline void cController::SetActuatorYawPower(const float power) { m_actuatorYawPower = power; }
