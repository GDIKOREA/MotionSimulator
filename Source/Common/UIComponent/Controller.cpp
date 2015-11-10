
#include "stdafx.h"
#include "Controller.h"
#include "SerialCommunication.h"



cController::cController() :
	m_globalSeconds(0)
{
	m_serialComm = new cSerialCommunication();
	m_udpServer.m_sleepMillis = 1;
}


cController::~cController()
{
	SAFE_DELETE(m_serialComm);
}


// ť��� �ø���������� ������ �õ��Ѵ�.
bool cController::ConnectSerial(const int portNum, const int baudRate)
{
	RETV(!m_serialComm, false);
	return m_serialComm->ConnectSerial(portNum, baudRate);
}


// �ø��� ����� �����Ѵ�.
void cController::CloseSerial()
{
	RET(!m_serialComm);
	m_serialComm->Close();
}


void cController::Update(const float deltaSeconds)
{
	RET(!m_serialComm);

	m_globalSeconds += deltaSeconds;

	m_serialComm->ProcessSerialCommunicatoin(deltaSeconds);	

	char buffer[512];
	const int bufferLen = m_udpServer.GetRecvData(buffer, sizeof(buffer));
	if (bufferLen > 0)
	{
		if (bufferLen < sizeof(buffer) / sizeof(char))
			buffer[bufferLen] = NULL;
		NotifyUDPObserver(buffer, bufferLen);
	}

	NotifyUpdateObserver(deltaSeconds);
}
