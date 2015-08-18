//
// 데이타를 파일에 쓰는 기능을 한다.
// 쓰레드로 동작하면서, Close() 가 호출 될 때까지, 파일에 쓴다.
// 링버퍼로 데이터를 관리한다.
// 
#pragma once


class cUDPStream
{
public:
	cUDPStream();
	virtual ~cUDPStream();
	enum { MAX_BUFLEN = 128, MAX_RINGBUFF=32, };

	bool Open(const string &fileName, const int bufferLen=MAX_BUFLEN, const int threadSleepMillis=30);
	int Write(const char *buff, const int len);
	int GetBufferSize();
	void CopyToWriteBuffer();
	bool Close();


//protected:
public:
	std::ofstream m_ofs;
	int m_bufferLen;
	int m_headerSize; // time(int) = 4

	struct sElement 
	{
		int len; //' buffer length
		int t; // time, milliseconds
		char data[MAX_BUFLEN]; 
	};
	vector<sElement> m_ringBuffer;
	int m_head; // head == tail -> 버퍼가 비어 있을 때. 정보의 시작은 head 부터 tail 까지.
	int m_tail;

	vector<sElement> m_writeBuffer;

	HANDLE m_handle;
	DWORD m_threadId;
	CRITICAL_SECTION m_csRingBuffer;
	CRITICAL_SECTION m_csWriteBuffer;
	bool m_threadLoop;
	int m_treadSleepMillis; // sleep milliseconds
};
