
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
	, m_headerSize(8)
	, m_isWriteMode(true)
	, m_isOpen(false)
{
	InitializeCriticalSectionAndSpinCount(&m_csRingBuffer, 0x00000400);
	InitializeCriticalSectionAndSpinCount(&m_csThreadBuffer, 0x00000400);
}

cUDPStream::~cUDPStream()
{
	Close();

	m_threadLoop = false;
	::WaitForSingleObject(m_handle, 1000);

	DeleteCriticalSection(&m_csRingBuffer);
	DeleteCriticalSection(&m_csThreadBuffer);
}


// ���� ������ isCreate �� true �� �Ǿ�� �Ѵ�.
// �̹� �����ϴ� ������ �о� �� ���� false�� �Ǿ�� �Ѵ�.
bool cUDPStream::Open(bool isCreate, const string &fileName,
	const int bufferLen, const int threadSleepMillis) //bufferLen = MAX_BUFLEN, threadSleepMillis=30
{
	const int VERSION = 1;

	Close();

	cAutoCS cs1(m_csRingBuffer);
	cAutoCS cs2(m_csThreadBuffer);

	m_isWriteMode = isCreate;

	if (isCreate) 
	{
		// ���� ���
		m_ofs.open(fileName.c_str(), std::ios_base::binary);
		if (!m_ofs.is_open())
			return false;

		// file header setting
		m_ofs << "UDP"; // header
		const int version = VERSION;
		m_ofs.write((char*)&version, sizeof(version));
		const int elementBufferSize = bufferLen + (sizeof(sElement) - MAX_BUFLEN); // element buffer size
		m_ofs.write((char*)&elementBufferSize, sizeof(elementBufferSize));
		// 

		m_version = VERSION;
		m_bufferLen = bufferLen;
	}
	else
	{ 
		// �б� ���
		m_ifs.open(fileName.c_str(), std::ios_base::binary);
		if (!m_ifs.is_open())
			return false;

		char header[3];
		m_ifs >> header[0] >> header[1] >> header[2];

		if (('U' != header[0]) || ('D' != header[1]) || ('P' != header[2]))
			return false;

		int version;
		m_ifs.read((char*)&version, sizeof(version));

		int elementBufferLen; // element ������� ���Ե� ����Ʈ ��
		m_ifs.read((char*)&elementBufferLen, sizeof(elementBufferLen));

		const auto curPos = m_ifs.tellg();
		m_ifs.seekg(0, std::ios_base::end);
		int pos = (int)m_ifs.tellg();
		pos -= (int)curPos; // ���� ��� ������ ����
		m_totalElementSize = pos / elementBufferLen; // �� ������ ������ ����Ѵ�.
		m_ifs.seekg(curPos); // ���� ��ġ�� ����

		m_version = version;
		m_bufferLen = elementBufferLen - (sizeof(sElement) - MAX_BUFLEN);
	}

	m_isOpen = true;
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


// ������ �����ִٸ� true�� ����.
bool cUDPStream::IsOpen() const
{
	return m_isOpen;
}


// ���Ͽ��� ������ �о�´�.
// ������ ��� �о��ٸ�, -1 �� �����Ѵ�.
int cUDPStream::Read(sElement *out)
{
	if (m_isWriteMode)
		return -1; // �б��忡���� �۵��Ѵ�.

	if (!m_isOpen)
		return -1;

	cAutoCS cs1(m_csRingBuffer);
	if (m_tail == m_head)
	{
		cs1.Leave();

		// ���Ͽ��� ������ �о�´�.
		ReadFromFile();

		cAutoCS cs2(m_csThreadBuffer);
		if (!m_threadBuffer.empty())
			CopyToRingBuffer(m_ifs.eof());
		cs2.Leave();

		cs1.Enter();
		if (m_tail == m_head)
		{
			// �׷��� ���۰� ����ִٸ�, ������ ��� ���� ���̹Ƿ� Close�Ѵ�.
			cs1.Leave();
			Close();
			return -1;
		}
	}
	
	const int nextHeadIdx = (m_head + 1) % MAX_RINGBUFF;

	// copy
	memcpy((char*)out, &m_ringBuffer[m_head], m_ringBuffer[m_head].len);
	m_head = nextHeadIdx;

	return out->len;
}


// write ���� ũ�⸦ �����Ѵ�.
int cUDPStream::Write(const char *buff, const int len)
{
	if (!m_isWriteMode)
		return -1; // �����忡���� �۵��Ѵ�.

	if (!m_isOpen)
		return -1;

	cAutoCS cs1(m_csRingBuffer);

	// �̷��� �Ǹ�, ���� �ϳ��� ������.
	const int nextTailIdx = (m_tail + 1) % MAX_RINGBUFF;
	if (nextTailIdx == m_head)
		return 0; // full buffer

	m_ringBuffer[m_tail].len = m_bufferLen + (sizeof(sElement) - MAX_BUFLEN);
	m_ringBuffer[m_tail].t = timeGetTime();

	const int cpLen = min(MAX_BUFLEN, len);
	memcpy(m_ringBuffer[m_tail].data, buff, cpLen);

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
	if (m_isWriteMode)
	{
		// �����ۿ� �����Ͱ� �����ְų�, ������ۿ� �����Ͱ� �����ִٸ�, ��� ���� Close �Ѵ�.
		CopyToThreadBuffer();

		// ��� ���Ͽ� ����.
		cAutoCS cs1(m_csThreadBuffer);
		for each (auto &elem in m_threadBuffer)
		{
			m_ofs.write((char*)&elem, elem.len);
		}
		m_threadBuffer.clear();
	}
	else
	{
		// �б� ����, �߰� �۾� ����, ���۸� ����.
		cAutoCS cs1(m_csThreadBuffer);
		m_threadBuffer.clear();
	}

	// ������ �ʱ�ȭ
	m_head = 0;
	m_tail = 0;

	m_ofs.close();
	m_ifs.close();

	m_isOpen = false;

	return true;
}


// ������ ������ ������۷� ��� �����Ѵ�.
void cUDPStream::CopyToThreadBuffer()
{
	cAutoCS cs1(m_csThreadBuffer);
	cAutoCS cs2(m_csRingBuffer); // critical section enter ������ �߿��Ѵ�. c1 -> c2 �� ȣ��Ǿ�� ��.

	while (m_head != m_tail)
	{
		m_threadBuffer.push_back(m_ringBuffer[m_head]);
		m_head = (m_head + 1) % MAX_RINGBUFF;
	}
}


// ����������� ������ �����۷� ��� �����Ѵ�.
void cUDPStream::CopyToRingBuffer(const bool isFileEnd) //isFileEnd=false
{
	// ��� ���Ͽ� ����.
	cAutoCS cs1(m_csThreadBuffer);
	cAutoCS cs2(m_csRingBuffer);

	for each (auto &elem in m_threadBuffer)
	{
		// �̷��� �Ǹ�, ���� �ϳ��� ������.
		const int nextTailIdx = (m_tail + 1) % MAX_RINGBUFF;
		if (nextTailIdx == m_head)
			break; // full buffer

		m_ringBuffer[m_tail] = elem;
		m_tail = nextTailIdx;
	}
	m_threadBuffer.clear();

	if (isFileEnd)
  		m_ifs.close();
}


// ���Ͽ��� ������ �о�� ���ۿ� �����Ѵ�.
int cUDPStream::ReadFromFile()
{
	if (!m_ifs.is_open())
		return 0;
	
	if (m_ifs.eof())
		return 0;
	
	int retVal = 0;
	const int bufferSize = GetBufferSize();
	if (bufferSize < MAX_RINGBUFF / 2)
	{
		// �����ۿ� ������ �����ִٸ�, 
		// ���Ͽ��� ������ �о��, �����ۿ� �����Ѵ�.
		cAutoCS cs1(m_csThreadBuffer);
		int cnt = 0;
		while (!m_ifs.eof() && (cnt < MAX_RINGBUFF / 2))
		{
			sElement elem;
			m_ifs.read((char*)&elem.len, sizeof(int));
			if (!m_ifs.eof() && (elem.len > 0))
			{
				m_ifs.read((char*)&elem.t, elem.len - sizeof(int));
				m_threadBuffer.push_back(elem);

				++cnt;
			}
		}
		retVal = cnt;
	}

	return retVal;
}


// ������ ó���κ����� ���ư���.
bool cUDPStream::SeekToStart()
{
	RETV(m_isWriteMode, false); // �б� ��忡���� �����Ѵ�.

	cAutoCS cs1(m_csThreadBuffer);
	cAutoCS cs2(m_csRingBuffer);
	
	if (!m_ifs.is_open())
		return false; // ������ �������� ������ Error

	m_ifs.seekg(11); // header 11 byte 

	// ������ �ʱ�ȭ
	m_head = 0;
	m_tail = 0;

	// ������ ���� �ʱ�ȭ
	m_threadBuffer.clear();

	return true;
}


// Ư�� ������ ��ġ�� ���� �����͸� �̵��Ѵ�.
// ���� ������ ��ġ�� �����Ѵ�.
// �б� ��忡���� �۵��Ѵ�.
int cUDPStream::Seek(const int elementIndex)
{
	RETV(m_isWriteMode, -1);
	RETV(elementIndex < 0, -1);
	RETV(elementIndex >= m_totalElementSize, -1);

	cAutoCS cs1(m_csThreadBuffer);
	cAutoCS cs2(m_csRingBuffer);

	const int bufferLen = m_bufferLen + (sizeof(sElement) - MAX_BUFLEN); // ������� ���Ե� ���� ũ��
	const int pos = (elementIndex * bufferLen) + 11; // ��� ũ�� �߰�
	m_ifs.seekg(pos);

	// ���۸� ��� �����.
	
	// ������ �ʱ�ȭ
	m_head = 0;
	m_tail = 0;

	// ������ ���� �ʱ�ȭ
	m_threadBuffer.clear();

	return pos;
}


unsigned WINAPI UDPStreamThreadFunction(void* arg)
{
	cUDPStream *udpStream = (cUDPStream*)arg;

	while (udpStream->m_threadLoop)
	{
		if (udpStream->m_isWriteMode)
		{
			// ���� ���
			if (udpStream->m_ofs.is_open())
			{
				const int bufferSize = udpStream->GetBufferSize();
				if (bufferSize > cUDPStream::MAX_RINGBUFF / 2)
				{
					// �����ۿ� ������ �׿��ִٸ�,
					// �����۸� �����ؿ� ���Ͽ� �����Ѵ�.
					udpStream->CopyToThreadBuffer();

					// ���Ͽ� ����.
					cAutoCS cs1(udpStream->m_csThreadBuffer);
					for each (auto &elem in udpStream->m_threadBuffer)
					{
						udpStream->m_ofs.write((char*)&elem, elem.len);
					}
					udpStream->m_threadBuffer.clear();
				}
			}
		}
		else
		{
			// �б� ���
			if (udpStream->ReadFromFile() > 0)
				udpStream->CopyToRingBuffer(udpStream->m_ifs.eof());

		}

		Sleep(udpStream->m_treadSleepMillis);
	}

	return 0;
}
