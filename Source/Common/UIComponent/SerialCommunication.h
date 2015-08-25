//
// �ø�������� ���� ������ �ְ� �޴� ������ �Ѵ�.
//
#pragma once

#include "SerialObserver.h"


class cSerialCommunication : public cSerialObservable
{
public:
	cSerialCommunication();
	virtual ~cSerialCommunication();

	bool ConnectSerial(const int portNum, const int baudRate);
	void Close();
	bool ProcessSerialCommunicatoin(const float deltaSeconds);
	bool IsConnect() const;
	CSerial& GetSerial();


protected: 
	enum { MAX_BUFFER = 256 };

	//CSerial m_serial;
	CBufferedSerial m_serial;
	char m_buff[ MAX_BUFFER]; // ���๮�ڰ� �� �������� ������ �����Ѵ�.
	int m_currentBuffIndex = 0;
};


inline bool cSerialCommunication::IsConnect() const { return m_serial.IsOpened()? true : false;  }
inline CSerial& cSerialCommunication::GetSerial() { return m_serial; }
