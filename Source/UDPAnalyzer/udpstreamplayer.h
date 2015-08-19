//
// cUDPStream 객체를 이용해, *.udp 파일을 읽어와서 시간을 동기화 해가며, 정보를 리턴한다.
//
// 프레임 스킵 기능은 아직 없다.
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
	int m_startTime; // timeGetTime() 으로 리턴된 함수
	int m_startElementTime; // 첫번째 element의 t
	int m_curElementIndex; // 현재 동기화 되고 있는 Element 인덱스

};


inline bool cUDPStreamPlayer::IsPlay() const { return 1==m_playState; }
inline int cUDPStreamPlayer::GetCurrentPlayElementIndex() const { return m_curElementIndex; }
