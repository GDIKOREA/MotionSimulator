
#include "stdafx.h"
#include "udpnetgraphconfig.h"
#include <fstream>

const string configFileName = "udpnetgraphconfig.cfg";


cUDPNetGraphConfig::cUDPNetGraphConfig()
{
	Read(configFileName);
}

cUDPNetGraphConfig::~cUDPNetGraphConfig()
{
	Save();
}


void cUDPNetGraphConfig::InitDefault()
{
	m_agentType = 1;
	m_port = 10000;
	m_ip = "127.0.0.1";
	m_windowPosLeft = -1;
	m_windowPosTop = -1;
	m_windowWidth = -1;
	m_windowHeight = -1;

	m_plotCommand = "plot1 = 0, 0, 0, 0, 0\r\n\
string1 = %f;\r\n\
plot2 = 0, 0, 0, 0, 0\r\n\
string2 = %*f; %f;\r\n\
plot3 = 0, 0, 0, 0, 0\r\n\
string3 = %*f; %*f; %f;\r\n\
plot4 = 0, 0, 0, 0, 0\r\n\
string4 = %*f; %*f; %*f; %f;\r\n";
}


void cUDPNetGraphConfig::UpdateParseData()
{
	m_agentType = atoi(m_options["agentType"].c_str());
	m_port = atoi(m_options["port"].c_str());
	m_ip = m_options["ip"];

	m_windowPosLeft = atoi(m_options["plotwindow_left"].c_str());
	m_windowPosTop = atoi(m_options["plotwindow_top"].c_str());
	m_windowWidth = atoi(m_options["plotwindow_width"].c_str());
	m_windowHeight = atoi(m_options["plotwindow_height"].c_str());

	m_plotCommand = m_options["plot_command"];
}


void cUDPNetGraphConfig::Save()
{
	using namespace std;
	ofstream ofs(configFileName);
	if (!ofs.is_open())
		return;

	ofs << "agentType = " << m_agentType << endl;
	ofs << "port = " << m_port << endl;
	ofs << "ip = " << m_ip << endl;
	ofs << "plotwindow_left = " << m_windowPosLeft << endl;
	ofs << "plotwindow_top = " << m_windowPosTop  << endl;
	ofs << "plotwindow_width = " << m_windowWidth << endl;
	ofs << "plotwindow_height = " << m_windowHeight << endl;
	ofs << "plot_command = " << m_plotCommand << endl;
}
