
#include "stdafx.h"
#include "udpanalyzeroption.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>


// 전역 인스턴스
cUDPAnalyzerOption g_option;


cUDPAnalyzerOption::cUDPAnalyzerOption()
	: m_initWindows(false)
{

}

cUDPAnalyzerOption::~cUDPAnalyzerOption()
{

}


bool cUDPAnalyzerOption::Read(const string &fileName, const bool showMsgBox ) //showMsgBox=true
{

	try
	{
		// boost property tree
		using boost::property_tree::ptree;
		using std::string;
		ptree props;
		boost::property_tree::read_json(fileName, props);
		m_fileName = fileName;
		m_com = props.get<int>("COM", -1);
		m_baudRate = props.get<int>("BAUDRATE", 9600);
		m_ip = props.get<string>("IP", "127.0.0.1");
		m_port = props.get<int>("PORT", 8888);
		m_sendType = props.get<int>("SENDTYPE", 1);
		m_sendFormat = props.get<string>("SENDFORMAT", "$1;$2;$3");
		m_rollCmd = props.get<string>("ROLLCMD", "$1");
		m_pitchCmd = props.get<string>("PITCHCMD", "$2");
		m_yawCmd = props.get<string>("YAWCMD", "$3");
		m_plotCmd = props.get<string>("PLOTCMD", "");

		m_mixingCmd = props.get<string>("MIXINGCMD", "");
		m_plotViewCmd = props.get<string>("PLOTVIEWCMD", "");
		m_plotInputCmd = props.get<string>("PLOTINPUTCMD", "");
		m_udpProtocolCmd = props.get<string>("UDPPROTOCOLCMD", "");
		m_udpPort = props.get<int>("UDPPORT", 8888);

		m_udpPlayerIP = props.get<string>("UDPPLAYERIP", "127.0.0.1");
		m_udpPlayerPort = props.get<int>("UDPPLAYERPORT", 8888);

		m_initWindows = props.get<bool>("INITWINDOWS", false);
	}
	catch (std::exception&e)
	{
		if (showMsgBox)
			::AfxMessageBox(CString(L"Error!!\n") + str2wstr(e.what()).c_str());
	}

	return true;
}


bool cUDPAnalyzerOption::Write(const string &fileName)
{
	try
	{
		// boost property tree
		using boost::property_tree::ptree;
		using std::string;
		ptree props;
		props.add<int>("COM", m_com);
		props.add<int>("BAUDRATE", m_baudRate);
		props.add<string>("IP", m_ip);
		props.add<int>("PORT", m_port);
		props.add<int>("SENDTYPE", m_sendType);
		props.add<string>("SENDFORMAT", m_sendFormat);
		props.add<string>("ROLLCMD", m_rollCmd);
		props.add<string>("PITCHCMD", m_pitchCmd);
		props.add<string>("YAWCMD", m_yawCmd);
		props.add<string>("PLOTCMD", m_plotCmd);

		props.add<string>("MIXINGCMD", m_mixingCmd);
		props.add<string>("PLOTVIEWCMD", m_plotViewCmd);
		props.add<string>("PLOTINPUTCMD", m_plotInputCmd);
		props.add<string>("UDPPROTOCOLCMD", m_udpProtocolCmd);
		props.add<int>("UDPPORT", m_udpPort);
		props.add<string>("UDPPLAYERIP", m_udpPlayerIP);
		props.add<int>("UDPPLAYERPORT", m_udpPlayerPort);

		props.add<bool>("INITWINDOWS", m_initWindows);

		boost::property_tree::write_json(fileName, props);
	}
	catch (std::exception&e)
	{
		::AfxMessageBox(CString(L"Error!!\n") + str2wstr(e.what()).c_str());
	}

	return true;
}
