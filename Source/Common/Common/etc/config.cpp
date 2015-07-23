
#include "stdafx.h"
#include "config.h"

using namespace common;


cConfig::cConfig()
{
}

cConfig::~cConfig()
{
}


bool cConfig::Init(const string &fileName)
{
	if (!Parse(fileName))
	{
		InitDefault();
		return false;
	}
	return true;
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


// 입력으로 들어온 스트링을 분석한다.
void cConfig::ParseStr(const string &str)
{
	using namespace std;

	stringstream ss(str);

	string id, eq, val;
	while (ss >> id)// >> eq >> val)
	{
		val.clear();

		if (id[0] == '#')
		{
			string line;
			getline(ss, line);
			continue;  // skip comments
		}
		else if (id == "plot_command")
		{
			ss >> eq;

			std::string strCmd((std::istreambuf_iterator<char>(ss)), std::istreambuf_iterator<char>());
 			common::trim(strCmd);
			m_options[id] = strCmd; 		}
		else
		{
			ss >> eq >> val;
			if (eq != "=") throw std::runtime_error("Parse error");
			m_options[id] = val;
		}
	}

	UpdateParseData();
}


bool cConfig::GetBool(const string &key, const bool defaultValue)
{
	auto it = m_options.find(key);
	if (m_options.end() == it)
		return defaultValue;

	return (atoi(m_options[key].c_str()) > 0) ? true : false;
}


float cConfig::GetFloat(const string &key, const float defaultValue)
{
	auto it = m_options.find(key);
	if (m_options.end() == it)
		return defaultValue;

	return (float)atof(m_options[key].c_str());
}


int cConfig::GetInt(const string &key, const int defaultValue)
{
	auto it = m_options.find(key);
	if (m_options.end() == it)
		return defaultValue;

	return atoi(m_options[key].c_str());
}


bool cConfig::Save(const string &fileName)
{
	using namespace std;
	ofstream ofs(fileName);
	if (!ofs.is_open())
		return false;

	for each (auto &kv in m_options)
	{
		ofs << kv.first << " = " << kv.second << endl;
	}

	return true;
}
