
#include <iostream>
#include <string>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

using namespace std;


void main()
{
	string fileName = "hardware_settings_config.xml";


	//------------------------------------------------------------------------------------------------------------------------------------
	// xml read
	try
	{
		// boost property tree
		using boost::property_tree::ptree;
		ptree props;
		boost::property_tree::read_xml(fileName, props);

		bool parallelUpdateRender = props.get<bool>("hardware_settings_config.cpu.threadStrategy.<xmlattr>.parallelUpdateRender", false);
		cout << "parallelUpdateRender = " << parallelUpdateRender << endl;

		string workerMapFile = props.get<string>("hardware_settings_config.cpu.threadStrategy.<xmlattr>.workerMapFile", "");
		cout << "workerMapFile = " << workerMapFile << endl;
	}
	catch (std::exception&e)
	{
		cout << "Read Error " << e.what() << endl;
	}


	//------------------------------------------------------------------------------------------------------------------------------------
	// xml write
	try
	{
		// boost property tree
		using boost::property_tree::ptree;
		ptree props;
		
		props.put<bool>("hardware_settings_config.cpu.threadStrategy.<xmlattr>.parallelUpdateRender", false);
		boost::property_tree::write_xml("wr.xml", props);
	}
	catch (std::exception&e)
	{
		cout << "Write Error " << e.what() << endl;
	}

}

