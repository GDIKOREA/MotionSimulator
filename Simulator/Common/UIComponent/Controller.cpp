
#include "stdafx.h"
#include "Controller.h"
#include "CubeFlight.h"
#include "SerialCommunication.h"
#include "UDPCommunication.h"


cController::cController()
{
	m_cubeFlight = new cCubeFlight();
	m_serialComm = new cSerialCommunication();
	m_udpComm = new cUDPCommunication();
}


cController::~cController()
{
	SAFE_DELETE(m_cubeFlight);
	SAFE_DELETE(m_serialComm);
	SAFE_DELETE(m_udpComm);
}


// �������� �ʱ�ȭ �Ѵ�.
bool cController::Init()
{
	m_cubeFlight->Init();

	return true;
}


// ť��� �ø���������� ������ �õ��Ѵ�.
bool cController::ConnectSerial(const int portNum, const int baudRate)
{
	RETV(!m_serialComm, false);
	return m_serialComm->ConnectSerial(portNum, baudRate);
}


void cController::Update(const float deltaSeconds)
{
	RET(!m_serialComm);
	RET(!m_udpComm);

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