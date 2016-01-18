
#include "stdafx.h"
#include "udp2udpconfig.h"


const string configFileName = "udp2udpconfig.cfg";


cUDP2UDPConfig::cUDP2UDPConfig()
{
	Read(configFileName);
}

cUDP2UDPConfig::~cUDP2UDPConfig()
{
	Save();
}


void cUDP2UDPConfig::InitDefault()
{
	sSessionData data;
	data.receivePort = 8888;
	data.sendIP = "127.0.0.1";
	data.sendPort = 8889;
	m_sessions.push_back(data);
}


// 최대 10개의 udp2udp 접속 정보를 읽는다.
void cUDP2UDPConfig::UpdateParseData()
{
	using namespace common;

	m_sessions.reserve(10);

	int i = 1;
	for (int i = 0; i < 10; ++i)
	{
		const string strRcvPort = format("receive_port%d", i);
		const string strSndIP = format("send_ip%d", i);
		const string strSndPort = format("send_port%d", i);
		
		auto it1 = m_options.find(strRcvPort);
		auto it2 = m_options.find(strSndIP);
		auto it3 = m_options.find(strSndPort);
		
		// 세 정보 모두 없다면, 다음으로 넘어간다.
		if ((it1 == m_options.end()) && 
			(it2 == m_options.end()) && 
			(it3 == m_options.end()))
			continue;

		sSessionData data;
		if (it1 != m_options.end())
			data.receivePort = atoi(it1->second.c_str());
		if (it2 != m_options.end())
			data.sendIP = it2->second.c_str();
		if (it3 != m_options.end())
			data.sendPort = atoi(it3->second.c_str());

		m_sessions.push_back(data);
	}
}


void cUDP2UDPConfig::Save()
{
	using namespace std;
	ofstream ofs(configFileName);
	if (!ofs.is_open())
		return;

	int n = 1;
	for each (auto &session in m_sessions)
	{
		ofs << "receive_port" << n << " = " << session.receivePort << endl;
		ofs << "send_ip" << n << " = " << session.sendIP << endl;
		ofs << "send_port" << n << " = " << session.sendPort << endl;
		ofs << endl;
		ofs << endl;
		++n;
	}
}
