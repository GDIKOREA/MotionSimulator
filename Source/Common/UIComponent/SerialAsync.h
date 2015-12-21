//
//  2015-12-17, jjuiddong
//
// �񵿱� �ø��� ���
// ���� Send ������ ó���Ѵ�.
//
#pragma once

#include "BufferedSerial.h"


class cSerialAsync
{
public:
	cSerialAsync();
	virtual ~cSerialAsync();

	bool Open(const int portNum, const int baudRate);
	bool IsOpen();
	int SendData(BYTE *buffer, const int bufferLen);
	void Close();


	enum { BUFLEN = 512, };
	bool m_isConnect;
	char m_buffer[BUFLEN];
	int m_bufferLen;
	bool m_isSendData;
	CBufferedSerial m_serial;
	int m_sleepMillis; // default = 10

	HANDLE m_handle;
	DWORD m_threadId;
	CRITICAL_SECTION m_CriticalSection;
	bool m_threadLoop;
};
