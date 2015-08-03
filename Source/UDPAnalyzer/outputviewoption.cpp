
#include "stdafx.h"
#include "outputviewoption.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>



cOutputViewOption::cOutputViewOption()
{

}

cOutputViewOption::~cOutputViewOption()
{

}


bool cOutputViewOption::Read(const string &fileName)
{

	try
	{
		// boost property tree
		using boost::property_tree::ptree;
		using std::string;
		ptree props;
		boost::property_tree::read_json(fileName, props);
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
	}
	catch (std::exception&e)
	{
		::AfxMessageBox(CString(L"Error!!\n") + str2wstr(e.what()).c_str());
	}

	return true;
}


bool cOutputViewOption::Write(const string &fileName)
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

		boost::property_tree::write_json(fileName, props);
	}
	catch (std::exception&e)
	{
		::AfxMessageBox(CString(L"Error!!\n") + str2wstr(e.what()).c_str());
	}

	return true;
}
