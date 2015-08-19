
#include "stdafx.h"
#include "udpclient.h"
#include <iostream>
#include <process.h> 

using namespace std;
using namespace network;

unsigned WINAPI UDPClientThreadFunction(void* arg);


cUDPClient::cUDPClient() : 
	m_isConnect(false)
	, m_threadLoop(true)
	, m_sndBuffLen(0)
	, m_rcvBuffLen(0)
	, m_handle(NULL)
	, m_isReceiveData(false)
	, m_isSendData(false)
	, m_sleepMillis(30)
{
	InitializeCriticalSectionAndSpinCount(&m_sndCriticalSection, 0x00000400);
	InitializeCriticalSectionAndSpinCount(&m_rcvCriticalSection, 0x00000400);
}

cUDPClient::~cUDPClient()
{
	m_threadLoop = false;

	::WaitForSingleObject(m_handle, 1000);

	DeleteCriticalSection(&m_sndCriticalSection);
	DeleteCriticalSection(&m_rcvCriticalSection);
	closesocket(m_socket);
}


// UDP Ŭ���Ʈ ����, ip, port �� ������ �õ��Ѵ�.
bool cUDPClient::Init(const string &ip, const int port, const int sleepMillis) //sleepMillis=30
{
	Close();

	m_ip = ip;
	m_port = port;
	m_sleepMillis = sleepMillis;

	if (network::LaunchUDPClient(ip, port, m_si_other, m_socket))
	{
		cout << "Connect UDP Client ip= " << ip << ", port= " << port << endl;

		m_isConnect = true;
		m_threadLoop = true;
		if (!m_handle)
		{
			m_handle = (HANDLE)_beginthreadex(NULL, 0, UDPClientThreadFunction, this, 0, (unsigned*)&m_threadId);
		}
	}
	else
	{
		cout << "Error!! Connect UDP Client ip=" << ip << ", port=" << port << endl;
		return false;
	}
	
	return true;
}


// ������ ������ �����Ѵ�.
void cUDPClient::SendData(const char *buff, const int buffLen)
{
	EnterCriticalSection(&m_sndCriticalSection);
	memcpy(m_sndBuffer, buff, buffLen);
	m_sndBuffLen = buffLen;
	m_isSendData = true;
	LeaveCriticalSection(&m_sndCriticalSection);
}


// ���� ������ �����´�.
int cUDPClient::GetReceiveData(char *dst, const int maxbuffLen)
{
	EnterCriticalSection(&m_rcvCriticalSection);
	if (maxbuffLen < m_rcvBuffLen)
	{
		LeaveCriticalSection(&m_rcvCriticalSection);
		return 0;
	}

	if (!m_isReceiveData || (m_rcvBuffLen <= 0))
	{
		LeaveCriticalSection(&m_rcvCriticalSection);
		return 0;
	}

	memcpy(dst, m_rcvBuffer, m_rcvBuffLen);
	const int ret = m_rcvBuffLen;
	m_isReceiveData = false;
	LeaveCriticalSection(&m_rcvCriticalSection);
	return ret;
}


// ������ ���´�.
void cUDPClient::Close()
{
	m_isConnect = false;
	Sleep(100);
	closesocket(m_socket);
	m_socket = INVALID_SOCKET;
}


// UDP ��Ʈ��ũ ������.
unsigned WINAPI UDPClientThreadFunction(void* arg)
{
	cUDPClient *udp = (cUDPClient*)arg;

	int recv_len;
	char sndBuff[cUDPClient::BUFLEN];
	memset(sndBuff, '\0', cUDPClient::BUFLEN);

	while (udp->m_threadLoop)
	{
		if (!udp->m_isConnect || (INVALID_SOCKET == udp->m_socket))
		{
			Sleep(udp->m_sleepMillis);
			continue;
		}

		const int slen = sizeof(udp->m_si_other);

		// Send
		bool isSend = false;
 		EnterCriticalSection(&udp->m_sndCriticalSection);
		if (udp->m_isSendData)
		{
			memcpy(sndBuff, udp->m_sndBuffer, udp->m_sndBuffLen);
			recv_len = udp->m_sndBuffLen;
			udp->m_isSendData = false;
			isSend = true;
		}
		LeaveCriticalSection(&udp->m_sndCriticalSection);

		if (isSend)
		{
			if (sendto(udp->m_socket, (char*)sndBuff,
				recv_len, 0, (struct sockaddr*) &udp->m_si_other, slen) == SOCKET_ERROR)
			{
				//printf("sendto() failed with error code : %d", WSAGetLastError());
				//exit(EXIT_FAILURE);
			}
		}


		// Receive
		const timeval t = { 0, 1 }; // 10 millisecond
		fd_set readSockets;
		FD_ZERO(&readSockets);
		FD_SET(udp->m_socket, &readSockets);

		const int ret = select(readSockets.fd_count, &readSockets, NULL, NULL, &t);
		if (ret != 0 && ret != SOCKET_ERROR)
		{
			char rcvBuff[cUDPClient::BUFLEN];
			const int result = recv(readSockets.fd_array[0], rcvBuff, sizeof(rcvBuff), 0);
			if (result == SOCKET_ERROR || result == 0) // ���� ��Ŷ����� 0�̸� ������ ������ ����ٴ� �ǹ̴�.
			{
// 				closesocket(udp->m_socket);
// 				udp->m_threadLoop = false;
			}
			else
			{
				EnterCriticalSection(&udp->m_rcvCriticalSection);
				memcpy(udp->m_rcvBuffer, rcvBuff, result);
				udp->m_rcvBuffLen = result;
				udp->m_isReceiveData = true;
				LeaveCriticalSection(&udp->m_rcvCriticalSection);
			}
		}

		Sleep(udp->m_sleepMillis);
	}

	return 0;
}
