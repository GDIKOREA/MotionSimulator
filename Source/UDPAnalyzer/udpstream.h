//
// UDP 데이타를 파일에 쓰는 기능을 한다.
// 쓰레드로 동작하면서, Close() 가 호출 될 때까지, 파일에 쓴다.
// 링버퍼로 데이터를 관리한다.
// 
// Version 1.0
//	- header
//		- UDP (3 byte ASCII code)
//		- version (int)
//		- element buffer length (int)
//
//
#pragma once



class cUDPStream
{
public:
	cUDPStream();
	virtual ~cUDPStream();
	enum { MAX_BUFLEN = 128, MAX_RINGBUFF=32, };

	struct sElement
	{
		int len; // buffer length
		int t; // time, milliseconds
		char data[MAX_BUFLEN];
	};

	bool Open(bool isCreate, const string &fileName, const int bufferLen=MAX_BUFLEN, const int threadSleepMillis=30);
	int Read(sElement *out);
	int Write(const char *buff, const int len);
	int GetBufferSize();
	void CopyToThreadBuffer();
	void CopyToRingBuffer(const bool isFileEnd=false);
	bool Close();
	bool IsOpen() const;
	int ReadFromFile();
	bool SeekToStart();
	int Seek(const int elementIndex);


public:
	std::ofstream m_ofs;
	std::ifstream m_ifs;
	int m_version; // default = 1, ~
	int m_bufferLen; // udp 정보의 크기 (헤더 크기를 제외한 크기 (byte 단위))
	int m_headerSize; // time(int) + len(int) = 8
	bool m_isWriteMode;	// true=쓰기모드, false=읽기모드
	int m_totalElementSize; // 읽기모드에서만 유효한 정보, 파일 
	bool m_isOpen;

	vector<sElement> m_ringBuffer;
	int m_head; // head == tail -> 버퍼가 비어 있을 때. 정보의 시작은 head 부터 tail 까지.
	int m_tail;

	vector<sElement> m_threadBuffer;

	HANDLE m_handle;
	DWORD m_threadId;
	CRITICAL_SECTION m_csRingBuffer;
	CRITICAL_SECTION m_csThreadBuffer;
	bool m_threadLoop;
	int m_treadSleepMillis; // sleep milliseconds
};

