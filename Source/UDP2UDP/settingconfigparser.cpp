
#include "stdafx.h"
#include "settingconfigparser.h"



cSettingConfigParser::cSettingConfigParser()
{

}

cSettingConfigParser::~cSettingConfigParser()
{

}


bool cSettingConfigParser::Parse(const string &fileName, OUT vector<sUDPConnect> &out)
{
	using namespace std;

	ifstream ifs(fileName);
	if (!ifs.is_open())
		return false;

	stringstream ss(ifs);
	return ParseStream(ss, out);
}


bool cSettingConfigParser::ParseStr(const string &str, OUT vector<sUDPConnect> &out)
{
	stringstream ss(str);
	return ParseStream(ss, out);
}


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
bool cSettingConfigParser::ParseStream(std::stringstream &ss, OUT vector<sUDPConnect> &out)
{
	int state = 0;

	sUDPConnect udpCon;
	std::pair<string, int> clientCon;

	while (1)
	{
		if (state == 0) // parse receive server port
		{
			// 저장된 정보가 있으면, 추가한다.
			if (!udpCon.clients.empty())
			{
				out.push_back(udpCon);
				udpCon.clients.clear();
			}

			string id;
			ss >> id;
			if (id == "recv_port")
			{
				string eq;
				ss >> eq >> udpCon.serverPort;
				state = 1;
			}
			else if (id == "=")
			{
				ss >> udpCon.serverPort;
				state = 1;
			}
			else
			{
				// Parse 종료.
				break;
			}
		}
		else if (state == 1) // parse send client ip
		{
			string id;
			ss >> id;
			if (id == "send_ip")
			{
				string eq;
				ss >> eq >> clientCon.first;
				state = 2;
			}
			else if (id == "recv_port")
			{
				state = 0;
			}
			else
			{
				state = 0;
			}
		}
		else if (state == 2)// parse send client port
		{
			string id;
			ss >> id;
			if (id == "send_port")
			{
				string eq;
				ss >> eq >> clientCon.second;
				state = 1;

				udpCon.clients.push_back(clientCon);
			}
		}
	}

	return true;
}
