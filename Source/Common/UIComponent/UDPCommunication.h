//
// UDP 네트워크 통신 객체.
//
#pragma once


class cUDPCommunication
{
public:
	cUDPCommunication();
	virtual ~cUDPCommunication();

	bool InitServer(const int port);
	void Close();
	int Update(const float deltaSeconds, OUT char *buffer, const int bufferLen);
	int SendData(const char *buff, const int buffLen);
	bool IsConnect() const;


protected:
	SOCKET m_socket;
	int m_port;
	bool m_isConnect;
};


inline bool cUDPCommunication::IsConnect() const { return m_isConnect; }
