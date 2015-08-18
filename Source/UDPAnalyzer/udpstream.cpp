
#include "stdafx.h"
#include "udpstream.h"


unsigned WINAPI UDPStreamThreadFunction(void* arg);



cUDPStream::cUDPStream()
	: m_handle(NULL)
	, m_threadId(0)
	, m_threadLoop(true)
	, m_treadSleepMillis(30)
	, m_head(0)
	, m_tail(0)
	, m_bufferLen(0)
	, m_headerSize(sizeof(int))
{
	InitializeCriticalSectionAndSpinCount(&m_csRingBuffer, 0x00000400);
	InitializeCriticalSectionAndSpinCount(&m_csWriteBuffer, 0x00000400);
}

cUDPStream::~cUDPStream()
{
	Close();

	m_threadLoop = false;
	::WaitForSingleObject(m_handle, 1000);

	DeleteCriticalSection(&m_csRingBuffer);
	DeleteCriticalSection(&m_csWriteBuffer);
}


bool cUDPStream::Open(const string &fileName, 
	const int bufferLen, const int threadSleepMillis) //bufferLen = MAX_BUFLEN, threadSleepMillis=30
{
	Close();

	m_ofs.open(fileName.c_str(), std::ios_base::binary);
	if (!m_ofs.is_open())
		return false;

	m_bufferLen = bufferLen;
	m_treadSleepMillis = threadSleepMillis;

	m_ringBuffer.resize(MAX_RINGBUFF);
	m_head = 0;
	m_tail = 0;

	if (!m_handle)
	{
		m_threadLoop = true;
		m_handle = (HANDLE)_beginthreadex(NULL, 0, UDPStreamThreadFunction, this, 0, (unsigned*)&m_threadId);
	}

	return true;
}


// write ���� ũ�⸦ �����Ѵ�.
int cUDPStream::Write(const char *buff, const int len)
{
	if (!m_ofs.is_open())
		return 0;

	cAutoCS cs1(m_csRingBuffer);

	// �̷��� �Ǹ�, ���� �ϳ��� ������.
	const int nextTailIdx = (m_tail + 1) % MAX_RINGBUFF;
	if (nextTailIdx == m_head)
		return 0; // full buffer

	m_ringBuffer[m_tail].len = m_bufferLen + (sizeof(sElement) - MAX_BUFLEN);
	m_ringBuffer[m_tail].t = timeGetTime();
	memcpy(m_ringBuffer[m_tail].data, buff, len);

	m_tail = nextTailIdx;

	return len;
}


// �����ۿ� ����� ������ ������ �����Ѵ�.
int cUDPStream::GetBufferSize()
{
	cAutoCS cs(m_csRingBuffer);

	if (m_tail == m_head)
		return 0;

	if (m_tail > m_head)
	{
		return m_tail - m_head;
	}
	else
	{
		return MAX_RINGBUFF - (m_head - m_tail);
	}

	return 0;
}


// ���ۿ� ������ �����͸� ��� �� ��, �����Ѵ�.
bool cUDPStream::Close()
{
	// �����ۿ� �����Ͱ� �����ְų�, ������ۿ� �����Ͱ� �����ִٸ�, ��� ���� Close �Ѵ�.
	CopyToWriteBuffer();

	// ��� ���Ͽ� ����.
	cAutoCS cs1(m_csWriteBuffer);
	for each (auto &elem in m_writeBuffer)
	{
		m_ofs.write((char*)&elem, elem.len);
	}
	m_writeBuffer.clear();

	m_ofs.close();

	return true;
}


void cUDPStream::CopyToWriteBuffer()
{
	cAutoCS cs1(m_csWriteBuffer);
	cAutoCS cs2(m_csRingBuffer); // critical section enter ������ �߿��Ѵ�. c1 -> c2 �� ȣ��Ǿ�� ��.

	while (m_head != m_tail)
	{
		m_writeBuffer.push_back(m_ringBuffer[m_head]);
		m_head = (m_head + 1) % MAX_RINGBUFF;
	}
}


unsigned WINAPI UDPStreamThreadFunction(void* arg)
{
	cUDPStream *udpStream = (cUDPStream*)arg;

	while (udpStream->m_threadLoop)
	{
		const int bufferSize = udpStream->GetBufferSize();
		if (bufferSize > cUDPStream::MAX_RINGBUFF / 2)
		{
			// �����۸� �����ؿ� ���Ͽ� �����Ѵ�.
			udpStream->CopyToWriteBuffer();

			// ���Ͽ� ����.
			cAutoCS cs1(udpStream->m_csWriteBuffer);
			for each (auto &elem in udpStream->m_writeBuffer)
			{
				udpStream->m_ofs.write((char*)&elem, elem.len);
			}
			udpStream->m_writeBuffer.clear();
		}

		Sleep(udpStream->m_treadSleepMillis);
	}

	return 0;
}
