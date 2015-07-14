//
// v.1
// udp client, send, receive ±â´É
//
// v.2
// close(), waitforsingleobject
// isConnect()
// thread restart bug fix
//
#pragma once


namespace network
{

	class cUDPClient
	{
	public:
		cUDPClient();
		virtual ~cUDPClient();

		enum { BUFLEN = 64, };

		bool Init(const string &ip, const int port);
		void SendData(const char *buff, const int buffLen);
		int GetReceiveData(char *dst, const int maxbuffLen);
		void Close(const bool isWait = false);
		bool IsConnect() const;


		string m_ip;
		int m_port;

		bool m_isConnect;
		SOCKET m_socket;
		sockaddr_in m_si_other;
		char m_sndBuffer[BUFLEN];
		int m_sndBuffLen;
		char m_rcvBuffer[BUFLEN];
		int m_rcvBuffLen;
		bool m_isReceiveData;

		HANDLE m_handle;
		DWORD m_threadId;
		CRITICAL_SECTION m_sndCriticalSection;
		CRITICAL_SECTION m_rcvCriticalSection;
		bool m_threadLoop;
	};


	inline bool cUDPClient::IsConnect() const { return m_isConnect; }

}
