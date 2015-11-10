//
// 하나의 UDP 서버에서 패킷을 받아, 한개 이상의 UDP Client를 통해, 받은 정보를 그대로 전송한다.
//
#pragma once

#include "settingconfigparser.h"


class cMultiProxy
{
public:
	cMultiProxy();
	virtual ~cMultiProxy();

	bool Init(cSettingConfigParser::sUDPConnect &con);
	void Update();
	void Close();
	network::cUDPServer* GetState(network::cUDPClient *out[], const int maxBuff, OUT int &clientCount);


	network::cUDPServer m_udpServer;
	vector<network::cUDPClient> m_clients;
	int m_ReceiveCount;
};
