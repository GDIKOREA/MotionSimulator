
#include "stdafx.h"
#include "UDPCommunication.h"



cUDPCommunication::cUDPCommunication() :
m_isConnect(false)
{

}

cUDPCommunication::~cUDPCommunication()
{

}


// UDP ���� ����.
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


// ��Ʈ��ũ�κ��� ���� ��Ŷ�� ó���Ѵ�.
// ��Ŷ�� �޾�����, �ش� ��Ŷ�� �����Ѵ�.
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
		if (result == SOCKET_ERROR || result == 0) // ���� ��Ŷ����� 0�̸� ������ ������ ����ٴ� �ǹ̴�.
		{
			// �߰��� ���Ͽ����� �ߴ���, UDP ���������� ��� ����Ÿ�� ���� �� �ִ�.
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

