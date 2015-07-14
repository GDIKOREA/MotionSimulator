//
// UDP2UDP 환경설정 파일 로딩, 저장
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

