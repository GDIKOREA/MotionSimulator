
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


// 파일 생성시 isCreate 가 true 가 되어야 한다.
// 이미 존재하는 파일을 읽어 올 때는 false가 되어야 한다.
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
		// 쓰기 모드
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
		// 읽기 모드
		m_ifs.open(fileName.c_str(), std::ios_base::binary);
		if (!m_ifs.is_open())
			return false;

		char header[3];
		m_ifs >> header[0] >> header[1] >> header[2];

		if (('U' != header[0]) || ('D' != header[1]) || ('P' != header[2]))
			return false;

		int version;
		m_ifs.read((char*)&version, sizeof(version));

		int elementBufferLen; // element 헤더까지 포함된 바이트 수
		m_ifs.read((char*)&elementBufferLen, sizeof(elementBufferLen));

		const auto curPos = m_ifs.tellg();
		m_ifs.seekg(0, std::ios_base::end);
		int pos = (int)m_ifs.tellg();
		pos -= (int)curPos; // 파일 헤더 사이즈 제외
		m_totalElementSize = pos / elementBufferLen; // 총 아이템 개수를 계산한다.
		m_ifs.seekg(curPos); // 원래 위치로 복귀

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


// 파일이 열려있다면 true를 리턴.
bool cUDPStream::IsOpen() const
{
	return m_isOpen;
}


// 파일에서 정보를 읽어온다.
// 파일을 모두 읽었다면, -1 을 리턴한다.
int cUDPStream::Read(sElement *out)
{
	if (m_isWriteMode)
		return -1; // 읽기모드에서만 작동한다.

	if (!m_isOpen)
		return -1;

	cAutoCS cs1(m_csRingBuffer);
	if (m_tail == m_head)
	{
		cs1.Leave();

		// 파일에서 정보를 읽어온다.
		ReadFromFile();

		cAutoCS cs2(m_csThreadBuffer);
		if (!m_threadBuffer.empty())
			CopyToRingBuffer(m_ifs.eof());
		cs2.Leave();

		cs1.Enter();
		if (m_tail == m_head)
		{
			// 그래도 버퍼가 비어있다면, 파일을 모두 읽은 것이므로 Close한다.
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


// write 버퍼 크기를 리턴한다.
int cUDPStream::Write(const char *buff, const int len)
{
	if (!m_isWriteMode)
		return -1; // 쓰기모드에서만 작동한다.

	if (!m_isOpen)
		return -1;

	cAutoCS cs1(m_csRingBuffer);

	// 이렇게 되면, 버퍼 하나를 못쓴다.
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


// 링버퍼에 저장된 아이템 개수를 리턴한다.
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


// 버퍼에 남겨지 데이터를 모두 쓴 후, 종료한다.
bool cUDPStream::Close()
{
	if (m_isWriteMode)
	{
		// 링버퍼에 데이터가 남아있거나, 쓰기버퍼에 데이터가 남아있다면, 모두 쓰고 Close 한다.
		CopyToThreadBuffer();

		// 모두 파일에 쓴다.
		cAutoCS cs1(m_csThreadBuffer);
		for each (auto &elem in m_threadBuffer)
		{
			m_ofs.write((char*)&elem, elem.len);
		}
		m_threadBuffer.clear();
	}
	else
	{
		// 읽기 모드시, 추가 작업 없이, 버퍼를 비운다.
		cAutoCS cs1(m_csThreadBuffer);
		m_threadBuffer.clear();
	}

	// 링버퍼 초기화
	m_head = 0;
	m_tail = 0;

	m_ofs.close();
	m_ifs.close();

	m_isOpen = false;

	return true;
}


// 링버퍼 정보를 쓰기버퍼로 모두 복사한다.
void cUDPStream::CopyToThreadBuffer()
{
	cAutoCS cs1(m_csThreadBuffer);
	cAutoCS cs2(m_csRingBuffer); // critical section enter 순서가 중요한다. c1 -> c2 로 호출되어야 함.

	while (m_head != m_tail)
	{
		m_threadBuffer.push_back(m_ringBuffer[m_head]);
		m_head = (m_head + 1) % MAX_RINGBUFF;
	}
}


// 쓰레드버퍼의 정보를 링버퍼로 모두 복사한다.
void cUDPStream::CopyToRingBuffer(const bool isFileEnd) //isFileEnd=false
{
	// 모두 파일에 쓴다.
	cAutoCS cs1(m_csThreadBuffer);
	cAutoCS cs2(m_csRingBuffer);

	for each (auto &elem in m_threadBuffer)
	{
		// 이렇게 되면, 버퍼 하나를 못쓴다.
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


// 파일에서 정보를 읽어와 버퍼에 저장한다.
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
		// 링버퍼에 공간이 남아있다면, 
		// 파일에서 정보를 읽어와, 링버퍼에 복사한다.
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


// 파일의 처음부분으로 돌아간다.
bool cUDPStream::SeekToStart()
{
	RETV(m_isWriteMode, false); // 읽기 모드에서만 동작한다.

	cAutoCS cs1(m_csThreadBuffer);
	cAutoCS cs2(m_csRingBuffer);
	
	if (!m_ifs.is_open())
		return false; // 파일이 열려있지 않으면 Error

	m_ifs.seekg(11); // header 11 byte 

	// 링버퍼 초기화
	m_head = 0;
	m_tail = 0;

	// 쓰레드 버퍼 초기화
	m_threadBuffer.clear();

	return true;
}


// 특정 아이템 위치로 파일 포인터를 이동한다.
// 파일 포인터 위치를 리턴한다.
// 읽기 모드에서만 작동한다.
int cUDPStream::Seek(const int elementIndex)
{
	RETV(m_isWriteMode, -1);
	RETV(elementIndex < 0, -1);
	RETV(elementIndex >= m_totalElementSize, -1);

	cAutoCS cs1(m_csThreadBuffer);
	cAutoCS cs2(m_csRingBuffer);

	const int bufferLen = m_bufferLen + (sizeof(sElement) - MAX_BUFLEN); // 헤더까지 포함된 버퍼 크기
	const int pos = (elementIndex * bufferLen) + 11; // 헤더 크기 추가
	m_ifs.seekg(pos);

	// 버퍼를 모두 지운다.
	
	// 링버퍼 초기화
	m_head = 0;
	m_tail = 0;

	// 쓰레드 버퍼 초기화
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
			// 쓰기 모드
			if (udpStream->m_ofs.is_open())
			{
				const int bufferSize = udpStream->GetBufferSize();
				if (bufferSize > cUDPStream::MAX_RINGBUFF / 2)
				{
					// 링버퍼에 정보가 쌓여있다면,
					// 링버퍼를 복사해와 파일에 저장한다.
					udpStream->CopyToThreadBuffer();

					// 파일에 쓴다.
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
			// 읽기 모드
			if (udpStream->ReadFromFile() > 0)
				udpStream->CopyToRingBuffer(udpStream->m_ifs.eof());

		}

		Sleep(udpStream->m_treadSleepMillis);
	}

	return 0;
}
