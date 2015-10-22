	
#include "stdafx.h"
#include "UDPCommunication.h"



cUDPCommunication::cUDPCommunication() :
m_isConnect(false)
{

}

cUDPCommunication::~cUDPCommunication()
{

}


// UDP 서버 실행.
bool cUDPCommunication::InitServer(const int port)
{
	m_port = port;

	if (network::LaunchUDPServer(port, m_socket))
	{
		m_isConnect = true;
	}
	else
	{

	}

	return m_isConnect;
}


void cUDPCommunication::Close()
{
	m_isConnect = false;
	closesocket(m_socket);
}


// 네트워크로부터 받은 패킷을 처리한다.
// 패킷을 받았으면, 해당 패킷을 리턴한다.
int cUDPCommunication::Update(const float deltaSeconds, OUT char *buffer, const int bufferLen)
{
	if (!m_isConnect)
		return 0;

	const timeval t = { 0, 1 }; // 10 millisecond
	fd_set readSockets;
	FD_ZERO(&readSockets);
	FD_SET(m_socket, &readSockets);

	const int ret = select(readSockets.fd_count, &readSockets, NULL, NULL, &t);
	if (ret != 0 && ret != SOCKET_ERROR)
	{
		const int result = recv(readSockets.fd_array[0], buffer, bufferLen, 0);
		if (result == SOCKET_ERROR || result == 0) // 받은 패킷사이즈가 0이면 서버와 접속이 끊겼다는 의미다.
		{
			// 중간에 소켓에러가 뜨더라도, UDP 프로토콜은 계속 데이타를 받을 수 있다.
// 			Close();
// 			return 0;
		}

		return result;
	}

	return 0;
}


int cUDPCommunication::SendData(const char *buff, const int buffLen)
{
	const int result = send(m_socket, (char*)buff, buffLen, 0);
	return result;
}

