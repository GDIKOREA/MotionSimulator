//
// cUDPStream ��ü�� �̿���, *.udp ������ �о�ͼ� �ð��� ����ȭ �ذ���, ������ �����Ѵ�.
//
// ������ ��ŵ ����� ���� ����.
//
#pragma once

#include "udpstream.h"


class cUDPStreamPlayer
{
public:
	cUDPStreamPlayer();
	virtual ~cUDPStreamPlayer();

	bool Open(const string &fileName);
	void Play();
	void Stop();
	void Pause();
	int Update(char *outBuffer, const int maxBufferLen);
	int SetPlayPos(const int playElementIndex);
	bool IsPlay() const;
	int GetTotalElementSize() const;
	int GetCurrentPlayElementIndex() const;


protected:
	int m_playState; // 0=stop, 1=play, 2=pause

	cUDPStream m_udpStream;
	cUDPStream::sElement m_element;
	int m_startTime; // timeGetTime() ���� ���ϵ� �Լ�
	int m_startElementTime; // ù��° element�� t
	int m_curElementIndex; // ���� ����ȭ �ǰ� �ִ� Element �ε���

};


inline bool cUDPStreamPlayer::IsPlay() const { return 1==m_playState; }
inline int cUDPStreamPlayer::GetCurrentPlayElementIndex() const { return m_curElementIndex; }
