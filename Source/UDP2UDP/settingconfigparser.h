//
//recv_port = 1100
//	send_ip = 127.0.0.1
//	send_port = 10001
//	send_ip = 127.0.0.1
//	send_port = 10002
//
//recv_port = 1101
//	send_ip = 127.0.0.1
//	send_port = 11001
//	send_ip = 127.0.0.1
//	send_port = 11002
//
// 다음과 같은 스크립트를 분석해, 정보를 도출한다.
// recv_port 정보 이후의 send_ip 정보는 '\t' 탭을 넣어야 인식된다. 
//
#pragma once


class cSettingConfigParser
{
public:
	cSettingConfigParser();
	virtual ~cSettingConfigParser();

	struct sUDPConnect
	{
		int serverPort;
		vector< std::pair<string, int> > clients;
	};

	bool Parse(const string &fileName, OUT vector<sUDPConnect> &out);
	bool ParseStr(const string &str, OUT vector<sUDPConnect> &out);


protected:
	bool ParseStream(std::stringstream &ss, OUT vector<sUDPConnect> &out);
};
