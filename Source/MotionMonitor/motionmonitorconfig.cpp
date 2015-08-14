
#include "stdafx.h"
#include "motionmonitorconfig.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>



cMotionMonitorConfig::cMotionMonitorConfig()
{
}

cMotionMonitorConfig::~cMotionMonitorConfig()
{
}


// ȯ�������� �д´�.
bool cMotionMonitorConfig::ReadConfigFile(const string &fileName)
{
	try
	{
		// boost property tree
		using boost::property_tree::ptree;
		using std::string;
		ptree props;
		boost::property_tree::read_json(fileName, props);

		m_fileName = fileName;

		m_udpPort = props.get<int>("UDP_PORT", 8888);
		m_udpPlotCommand = props.get<string>("UDP_PLOT_COMMAND", "");
		m_udpModCommand = props.get<string>("UDP_MOD_COMMAND", "");
		m_mwaveModCommand = props.get<string>("MWAVE_MOD_COMMAND", "");
		m_joystickModCommand = props.get<string>("JOYSTICK_MOD_COMMAND", "");
		m_mixingModCommand = props.get<string>("MIXING_COMMAND", "");
		m_motionOutputComPort = props.get<int>("MOTION_OUTPUT_COMPORT", 0);
		m_motionOutputBaudRate = props.get<int>("MOTION_OUTPUT_BAUDRATE", 0);
	}
	catch (std::exception&e)
	{
		::AfxMessageBox(CString(L"Error!!\n") + str2wstr(e.what()).c_str());
	}

	return true;
}


// ȯ�������� �����Ѵ�.
bool cMotionMonitorConfig::WriteConfigFile(const string &fileName)
{
	try
	{
		// boost property tree
		using boost::property_tree::ptree;
		using std::string;
		ptree props;
		
		m_fileName = fileName;

		props.add<int>("UDP_PORT", m_udpPort);
		props.add<string>("UDP_PLOT_COMMAND", m_udpPlotCommand);
		props.add<string>("UDP_MOD_COMMAND", m_udpModCommand);
		props.add<string>("MWAVE_MOD_COMMAND", m_mwaveModCommand);
		props.add<string>("JOYSTICK_MOD_COMMAND", m_joystickModCommand);
		props.add<string>("MIXING_COMMAND", m_mixingModCommand);
		props.add<int>("MOTION_OUTPUT_COMPORT", m_motionOutputComPort);
		props.add<int>("MOTION_OUTPUT_BAUDRATE", m_motionOutputBaudRate);

		boost::property_tree::write_json(fileName, props);
	}
	catch (std::exception&e)
	{
		::AfxMessageBox(CString(L"Error!!\n") + str2wstr(e.what()).c_str());
	}
	return true;
}

