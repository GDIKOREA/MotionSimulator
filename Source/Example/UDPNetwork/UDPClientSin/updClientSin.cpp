// 특정 UDP 서버로 Sin 파형을 보낸다.

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

/*
Simple udp client
Silver Moon (m00n.silv3r@gmail.com)
*/
#include<stdio.h>
#include<winsock2.h>
#include <math.h>
#include<mmsystem.h>
#pragma comment(lib,"ws2_32.lib") //Winsock Library
#pragma comment(lib,"winmm.lib")

#define SERVER "127.0.0.1"  //ip address of udp server
#define BUFLEN 64  //Max length of buffer
#define PORT 8888   //The port on which to listen for incoming data

int main(void)
{
	struct sockaddr_in si_other;
	int s, slen = sizeof(si_other);
	char buf[BUFLEN];
	//	char message[BUFLEN];
	WSADATA wsa;

	//Initialise winsock
	printf("\nInitialising Winsock...");
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("Failed. Error Code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}
	printf("Initialised.\n");
	printf("UDP Client Initialized\n");
	printf("ip = %s  port = %d\n\n", SERVER, PORT);

	//create socket
	if ((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == SOCKET_ERROR)
	{
		printf("socket() failed with error code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}

	//setup address structure
	memset((char *)&si_other, 0, sizeof(si_other));
	si_other.sin_family = AF_INET;
	si_other.sin_port = htons(PORT);
	si_other.sin_addr.S_un.S_addr = inet_addr(SERVER);

	int oldT = timeGetTime();
	int incT = 0;
	float x = 0;

	//start communication
	printf("send sin wave\n");
	while (1)
	{
		int curT = timeGetTime();
		int deltaT = curT - oldT;
		oldT = curT;
		incT += deltaT;

		if (incT < 30)
			continue;

		x += (incT * 0.001f);
		incT = 0;

		//send the message
		float value = (float)sin(x);
		sprintf(buf, "%f\n", value);

		if (sendto(s, buf, sizeof(buf), 0, (struct sockaddr *) &si_other, slen) == SOCKET_ERROR)
		{
			printf("sendto() failed with error code : %d", WSAGetLastError());
			exit(EXIT_FAILURE);
		}
	}

	closesocket(s);
	WSACleanup();

	return 0;
}
