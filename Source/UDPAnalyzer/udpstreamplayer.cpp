
#include "stdafx.h"
#include "udpstreamplayer.h"



cUDPStreamPlayer::cUDPStreamPlayer() :
	m_startTime(0)
	, m_playState(0)
	, m_curElementIndex(0)
{
	m_element.t = 0;
}

cUDPStreamPlayer::~cUDPStreamPlayer()
{
	Stop();
}


// UDP 스트리밍 파일을 읽어온다.
bool cUDPStreamPlayer::Open(const string &fileName)
{
	if (!m_udpStream.Open(false, fileName))
		return false;

	Stop();

	return true;
}


// 전체 아이템 개수를 리턴한다.
int cUDPStreamPlayer::GetTotalElementSize() const
{
	RETV(!m_udpStream.IsOpen(), 0);
	return m_udpStream.m_totalElementSize;
}


// 파일에서 정보를 읽어와, 동기화를 맞춰가며 정보를 리턴한다.
// return value = -1 파일의 끝, 더이상 정보가 없음.
//				= 0 동기화가 안된 상태
//				> 0 정보가 리턴되며, 동기화된 UDP 정보를 리턴한다.
//
// 시간 동기화를 자동으로 처리한다.
//
int cUDPStreamPlayer::Update(char *outBuffer, const int maxBufferLen)
{
	RETV(1 != m_playState, 0);
	RETV(!m_udpStream.IsOpen(), -1);

	// First Start
	if (m_startTime == 0)
	{
		m_startTime = timeGetTime();
		if (m_udpStream.Read(&m_element) < 0)
		{
			// Error
			Stop();
			return -1;
		}

		m_startElementTime = m_element.t;
		++m_curElementIndex;
	}

	const int curT = timeGetTime();
	const int elapsT = curT - m_startTime;
	const int elemElapsT = m_element.t - m_startElementTime;

	if (elapsT > elemElapsT)
	{
		const int bufferLen = m_element.len - (sizeof(m_element) - cUDPStream::MAX_BUFLEN);
		if (bufferLen <= maxBufferLen)
			memcpy(outBuffer, m_element.data, bufferLen);

		// 다음 정보를 읽어온다.
		if (m_udpStream.Read(&m_element) < 0)
		{
			// Error
			Stop();
			return -1;
		}

		++m_curElementIndex;

		return bufferLen;
	}

	return 0;
}


// UDP Player Play.
void cUDPStreamPlayer::Play()
{
	if (2 == m_playState)
	{
		// resume
		m_startTime = timeGetTime() - (m_element.t - m_startElementTime) - 10; // 여분의 10 millisecond 추가	
	}
	else
	{
		m_startTime = 0;
		m_startElementTime = 0;
		m_curElementIndex = 0;
	}

	m_playState = 1;
}


// UDP Player 종료.
void cUDPStreamPlayer::Stop()
{
	m_playState = 0;
	m_udpStream.SeekToStart();
}


// 일시 정지
void cUDPStreamPlayer::Pause()
{
	m_playState = 2;
}


// playElementIndex 위치로 이동한다.
int cUDPStreamPlayer::SetPlayPos(const int playElementIndex)
{
	RETV(2 != m_playState, 0); // Pause 상태일 때만 작동한다.

	if (m_udpStream.Seek(playElementIndex) < 0)
		return -1; // 실패

	m_curElementIndex = playElementIndex;

	if (m_udpStream.Read(&m_element) < 0)
	{
		// Error
		Stop();
		return -1;
	}

	// 시작 시간을 바꿔줘야, 동기화가 가능하다.
	m_startTime = timeGetTime() - m_element.t - 10; // 여분의 10 millisecond 추가	

	return playElementIndex;
}

