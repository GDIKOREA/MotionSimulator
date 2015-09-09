
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


// 환경파일을 읽는다.
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

		m_udpParsePort = props.get<int>("UDP_PARSE_PORT", 8888);
		m_udpProtocolCommand = props.get<string>("UDP_PROTOCOL_COMMAND", "");
		m_mixingScript = props.get<string>("MIXING_SCRIPT", "");
		m_rollCommand = props.get<string>("UDP_INPUT_ROLL_COMMAND", "");
		m_pitchCommand = props.get<string>("UDP_INPUT_PITCH_COMMAND", "");
		m_yawCommand = props.get<string>("UDP_INPUT_YAW_COMMAND", "");
		m_heaveCommand = props.get<string>("UDP_INPUT_HEAVE_COMMAND", "");

		m_plotViewPlotCmd = props.get<string>("PLOTVIEW_PLOT_COMMAND", "");
		m_plotViewPlotInputCmd = props.get<string>("PLOTVIEW_PLOTINPUT_COMMAND", "");
	}
	catch (std::exception&e)
	{
		::AfxMessageBox(CString(L"Error!!\n") + str2wstr(e.what()).c_str());
	}

	return true;
}


// 환경파일을 저장한다.
bool cMotionMonitorConfig::WriteConfigFile(const string &fileName)
{
	if (fileName.empty())
		return false;

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

		props.add<int>("UDP_PARSE_PORT", m_udpParsePort);
		props.add<string>("UDP_PROTOCOL_COMMAND", m_udpProtocolCommand);
		props.add<string>("MIXING_SCRIPT", m_mixingScript);
		props.add<string>("UDP_INPUT_ROLL_COMMAND", m_rollCommand);
		props.add<string>("UDP_INPUT_PITCH_COMMAND", m_pitchCommand);
		props.add<string>("UDP_INPUT_YAW_COMMAND", m_yawCommand);
		props.add<string>("UDP_INPUT_HEAVE_COMMAND", m_heaveCommand);

		props.add<string>("PLOTVIEW_PLOT_COMMAND", m_plotViewPlotCmd);
		props.add<string>("PLOTVIEW_PLOTINPUT_COMMAND", m_plotViewPlotInputCmd);

		boost::property_tree::write_json(fileName, props);
	}
	catch (std::exception&e)
	{
		::AfxMessageBox(CString(L"Error!!\n") + str2wstr(e.what()).c_str());
	}

	return true;
}

