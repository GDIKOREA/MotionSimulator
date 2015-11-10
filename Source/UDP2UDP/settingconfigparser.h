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
// ������ ���� ��ũ��Ʈ�� �м���, ������ �����Ѵ�.
// recv_port ���� ������ send_ip ������ '\t' ���� �־�� �νĵȴ�. 
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
