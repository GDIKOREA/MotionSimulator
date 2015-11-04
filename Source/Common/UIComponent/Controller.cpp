
#include "stdafx.h"
#include "Controller.h"
#include "CubeFlight.h"
#include "SerialCommunication.h"
#include "UDPCommunication.h"


cController::cController() :
	m_globalSeconds(0)
	, m_actuatorSpeed(1)
	, m_actuatorYawPower(1)
	, m_playTime(60)
{
	//m_cubeFlight = new cCubeFlight();
	m_serialComm = new cSerialCommunication();
	m_udpComm = new cUDPCommunication();
}


cController::~cController()
{
	//SAFE_DELETE(m_cubeFlight);
	SAFE_DELETE(m_serialComm);
	SAFE_DELETE(m_udpComm);
}


// 큐브와 시리얼통신으로 연결을 시도한다.
bool cController::ConnectSerial(const int portNum, const int baudRate)
{
	RETV(!m_serialComm, false);
	return m_serialComm->ConnectSerial(portNum, baudRate);
}


// 시리얼 통신을 종료한다.
void cController::CloseSerial()
{
	RET(!m_serialComm);
	m_serialComm->Close();
}


void cController::Update(const float deltaSeconds)
{
	RET(!m_serialComm);
	RET(!m_udpComm);

	m_globalSeconds += deltaSeconds;

	m_serialComm->ProcessSerialCommunicatoin(deltaSeconds);	

	char buffer[256];
	const int bufferLen = m_udpComm->Update(deltaSeconds, buffer, sizeof(buffer));
	if (bufferLen > 0)
	{
		buffer[bufferLen] = NULL;
		NotifyUDPObserver(buffer, bufferLen);
	}

	NotifyUpdateObserver(deltaSeconds);
}
