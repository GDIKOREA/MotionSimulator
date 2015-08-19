//
// UDP ����Ÿ�� ���Ͽ� ���� ����� �Ѵ�.
// ������� �����ϸ鼭, Close() �� ȣ�� �� ������, ���Ͽ� ����.
// �����۷� �����͸� �����Ѵ�.
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
	int m_bufferLen; // udp ������ ũ�� (��� ũ�⸦ ������ ũ�� (byte ����))
	int m_headerSize; // time(int) + len(int) = 8
	bool m_isWriteMode;	// true=������, false=�б���
	int m_totalElementSize; // �б��忡���� ��ȿ�� ����, ���� 
	bool m_isOpen;

	vector<sElement> m_ringBuffer;
	int m_head; // head == tail -> ���۰� ��� ���� ��. ������ ������ head ���� tail ����.
	int m_tail;

	vector<sElement> m_threadBuffer;

	HANDLE m_handle;
	DWORD m_threadId;
	CRITICAL_SECTION m_csRingBuffer;
	CRITICAL_SECTION m_csThreadBuffer;
	bool m_threadLoop;
	int m_treadSleepMillis; // sleep milliseconds
};

