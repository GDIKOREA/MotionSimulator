

#include <iostream>
#include <string>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include "hardwaresettingconfig.h"

using namespace std;


void main()
{
	string fileName = "hardware_settings_config.xml";

	cHardwareSettingConfig config;
	config.Read(fileName);

	config.Write("test.xml");
}


