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

	string m_mode; // machinegun_stand, machinegun, dirt3, joystick, mwav

	int m_udpPort;
	string m_udpPlotCommand;
	string m_udpModCommand;
	string m_udpPlotInputCmd;
	string m_mwaveModCommand;
	string m_joystickModCommand;
	string m_mixingModCommand;
	int m_motionOutputComPort;
	int m_motionOutputBaudRate;

	string m_udpProtocolCommand;
	string m_mixingScript;
	string m_rollCommand;
	string m_pitchCommand;
	string m_yawCommand;
	string m_heaveCommand;
	int m_udpParsePort;

	string m_plotViewPlotCmd;
	string m_plotViewPlotInputCmd;
};

