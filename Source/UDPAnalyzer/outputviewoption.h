#pragma once


class cOutputViewOption
{
public:
	cOutputViewOption();
	virtual ~cOutputViewOption();
	
	bool Read(const string &fileName);
	bool Write(const string &fileName);


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
};
