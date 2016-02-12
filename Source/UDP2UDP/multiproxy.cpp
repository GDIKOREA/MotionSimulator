
#include "stdafx.h"
#include "multiproxy.h"


cMultiProxy::cMultiProxy() 
	: m_ReceiveCount(0)
{

}

cMultiProxy::~cMultiProxy()
{
	Close();
}


bool cMultiProxy::Init(cSettingConfigParser::sUDPConnect &con)
{
	Close();

	m_ReceiveCount = 0;

	if (!m_udpServer.Init(0, con.serverPort))
		return false;

	m_clients.clear();
	m_clients.resize(con.clients.size());
	for (u_int i = 0; i < con.clients.size(); ++i)
	{
		m_clients[i].Init(con.clients[i].first, con.clients[i].second, 1);
	}

	return true;
}


void cMultiProxy::Update()
{
	RET(!m_udpServer.IsConnect());

	using namespace network;

	if (m_udpServer.m_isReceiveData)
	{
		BYTE buff[network::BUFFER_LENGTH];
		if (const int buffLen = m_udpServer.GetRecvData(buff, network::BUFFER_LENGTH))
		{
			for (u_int i = 0; i < m_clients.size(); ++i)
				m_clients[i].SendData((BYTE*)buff, buffLen);

			++m_ReceiveCount;
		}
	}
}


void cMultiProxy::Close()
{
	m_udpServer.Close();

	for (u_int i = 0; i < m_clients.size(); ++i)
		m_clients[i].Close();
}


network::cUDPServer* cMultiProxy::GetState(network::cUDPClient *out[], const int maxBuff, OUT int &clientCount)
{
	for (u_int i = 0; i < m_clients.size(); ++i)
	{
		if ((int)i < maxBuff)
			out[i] = &m_clients[i];
	}
	clientCount = (int)m_clients.size();

	return &m_udpServer;
}
