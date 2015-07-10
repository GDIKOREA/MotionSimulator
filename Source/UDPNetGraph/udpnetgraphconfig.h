//
// UDP Net Graph 프로젝트 환경설정 저장.
//
#pragma once

#include "config.h"

class cUDPNetGraphConfig : public cConfig
{
public:
	cUDPNetGraphConfig();
	virtual ~cUDPNetGraphConfig();

	virtual void InitDefault() override;
	virtual void UpdateParseData() override;
	void Save();


public:
	int m_agentType; // 0: client, 1: server
	int m_port;
	string m_ip;
	int m_windowPosLeft;
	int m_windowPosTop;
	int m_windowWidth;
	int m_windowHeight;
	string m_plotCommand;	
};
