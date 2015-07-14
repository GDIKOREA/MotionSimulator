//
// UDP2UDP ȯ�漳�� ���� �ε�, ����
//
#pragma once

struct sSessionData
{
	int receivePort;
	string sendIP;
	int sendPort;
};


class cUDP2UDPConfig : public common::cConfig
{
public:
	cUDP2UDPConfig();
	virtual ~cUDP2UDPConfig();

	virtual void InitDefault() override;
	virtual void UpdateParseData() override;
	void Save();


public:
	vector<sSessionData> m_sessions;
};

