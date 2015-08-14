#pragma once


class cMotionMonitorConfig
{
public:
	cMotionMonitorConfig();
	virtual ~cMotionMonitorConfig();

	bool ReadConfigFile(const string &fileName);
	bool WriteConfigFile(const string &fileName);


public:
	string m_fileName;

	int m_udpPort;
	string m_udpPlotCommand;
	string m_udpModCommand;
	string m_mwaveModCommand;
	string m_joystickModCommand;
	string m_mixingModCommand;
	int m_motionOutputComPort;
	int m_motionOutputBaudRate;
};

