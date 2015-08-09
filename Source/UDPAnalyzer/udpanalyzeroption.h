#pragma once


class cUDPAnalyzerOption
{
public:
	cUDPAnalyzerOption();
	virtual ~cUDPAnalyzerOption();
	
	bool Read(const string &fileName, const bool showMsgBox=true);
	bool Write(const string &fileName);

	string m_fileName;

	int m_com;
	int m_baudRate;
	string m_ip;
	int m_port;
	int m_sendType;
	string m_sendFormat;
	string m_rollCmd;
	string m_pitchCmd;
	string m_yawCmd;
	string m_plotCmd;

	string m_mixingCmd;
	string m_plotViewCmd;
	string m_plotInputCmd;
	string m_udpProtocolCmd;
	int m_udpPort;

	bool m_initWindows;
};

extern cUDPAnalyzerOption g_option;
