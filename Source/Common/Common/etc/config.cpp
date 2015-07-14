
#include "stdafx.h"
#include "config.h"

using namespace common;


cConfig::cConfig()
{
}

cConfig::~cConfig()
{
}


void cConfig::Init(const string &fileName)
{
	if (!Parse(fileName))
		InitDefault();
}


// 환경설정 파일을 읽어서 저장한다.
bool cConfig::Parse(const string &fileName)
{
	std::ifstream cfgfile(fileName);
	if (!cfgfile.is_open())
	{
		printf("not exist config file!!\n");
		return false;
	}

	string id, eq, val;
	while (cfgfile >> id)// >> eq >> val)
	{
		val.clear();

		if (id[0] == '#')
		{
			string line;
			getline(cfgfile, line);
			continue;  // skip comments
		}
		else if (id == "plot_command")
		{
			cfgfile >> eq;

			std::string strCmd((std::istreambuf_iterator<char>(cfgfile)), std::istreambuf_iterator<char>());
			common::trim(strCmd);
			m_options[id] = strCmd;
		}
		else
		{
			cfgfile >> eq >> val;
			if (eq != "=") throw std::runtime_error("Parse error");
			m_options[id] = val;
		}
	}

	UpdateParseData();

	return true;
}


bool cConfig::GetBool(const string &key)
{
	return (atoi(m_options[key].c_str()) > 0) ? true : false;
}


float cConfig::GetFloat(const string &key)
{
	return (float)atof(m_options[key].c_str());
}


int cConfig::GetInt(const string &key)
{
	return atoi(m_options[key].c_str());
}
