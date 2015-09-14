
#include "stdafx.h"
#include "LauncherConfig.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/xml_parser.hpp>


cLauncherConfig::cLauncherConfig()
{
}

cLauncherConfig::cLauncherConfig(const string &fileName)
{
	Read(fileName);
}

cLauncherConfig::~cLauncherConfig()
{	
}


// json 파일을 읽는다.
bool cLauncherConfig::Read(const string &fileName)
{
	try
	{
		// boost property tree
		using boost::property_tree::ptree;
		using std::string;
		ptree props;
		boost::property_tree::read_json(fileName, props);

		m_cameraCalibrationExePath = props.get<string>("CAMERA_CALIBRATION_EXE_PATH", "");
		m_cameraCheckExePath = props.get<string>("CAMERA_CHECK_EXE_PATH", "");
		m_gameExePath = props.get<string>("GAME_EXE_PATH", "");
		m_cameraExePath = props.get<string>("CAMERA_EXE_PATH", "");
	}
	catch (std::exception&e)
	{
		::AfxMessageBox(CString(L"Error!!\n") + str2wstr(e.what()).c_str());
	}

	return true;
}
