
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


// UDP ��Ʈ���� ������ �о�´�.
bool cUDPStreamPlayer::Open(const string &fileName)
{
	if (!m_udpStream.Open(false, fileName))
		return false;

	Stop();

	return true;
}


// ��ü ������ ������ �����Ѵ�.
int cUDPStreamPlayer::GetTotalElementSize() const
{
	RETV(!m_udpStream.IsOpen(), 0);
	return m_udpStream.m_totalElementSize;
}


// ���Ͽ��� ������ �о��, ����ȭ�� ���簡�� ������ �����Ѵ�.
// return value = -1 ������ ��, ���̻� ������ ����.
//				= 0 ����ȭ�� �ȵ� ����
//				> 0 ������ ���ϵǸ�, ����ȭ�� UDP ������ �����Ѵ�.
//
// �ð� ����ȭ�� �ڵ����� ó���Ѵ�.
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

		// ���� ������ �о�´�.
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
		m_startTime = timeGetTime() - (m_element.t - m_startElementTime) - 10; // ������ 10 millisecond �߰�	
	}
	else
	{
		m_startTime = 0;
		m_startElementTime = 0;
		m_curElementIndex = 0;
	}

	m_playState = 1;
}


// UDP Player ����.
void cUDPStreamPlayer::Stop()
{
	m_playState = 0;
	m_udpStream.SeekToStart();
}


// �Ͻ� ����
void cUDPStreamPlayer::Pause()
{
	m_playState = 2;
}


// playElementIndex ��ġ�� �̵��Ѵ�.
int cUDPStreamPlayer::SetPlayPos(const int playElementIndex)
{
	RETV(2 != m_playState, 0); // Pause ������ ���� �۵��Ѵ�.

	if (m_udpStream.Seek(playElementIndex) < 0)
		return -1; // ����

	m_curElementIndex = playElementIndex;

	if (m_udpStream.Read(&m_element) < 0)
	{
		// Error
		Stop();
		return -1;
	}

	// ���� �ð��� �ٲ����, ����ȭ�� �����ϴ�.
	m_startTime = timeGetTime() - m_element.t - 10; // ������ 10 millisecond �߰�	

	return playElementIndex;
}

