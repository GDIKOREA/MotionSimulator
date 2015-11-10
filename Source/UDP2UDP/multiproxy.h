//
// �ϳ��� UDP �������� ��Ŷ�� �޾�, �Ѱ� �̻��� UDP Client�� ����, ���� ������ �״�� �����Ѵ�.
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
