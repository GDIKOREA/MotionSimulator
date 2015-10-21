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

	cSerialCommunication& GetSerialComm();
	cUDPCommunication& GetUDPComm();


protected:
	cSerialCommunication *m_serialComm;
	cUDPCommunication *m_udpComm;
	float m_globalSeconds;
};


inline cSerialCommunication& cController::GetSerialComm() { return *m_serialComm;  }
inline cUDPCommunication& cController::GetUDPComm() { return *m_udpComm; }
inline float cController::GetGlobalSeconds() const { return m_globalSeconds; }
