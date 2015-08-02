
#include "stdafx.h"
#include "protocolparser.h"


cProtocolParser::cProtocolParser()
{
}

cProtocolParser::~cProtocolParser()
{
}


// 파일을 읽어서 파싱한다.
bool cProtocolParser::Read(const string &fileName)
{
	using namespace std;
	ifstream ifs(fileName);
	if (!ifs.is_open())
		return false;

	std::string strCmd((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
	common::trim(strCmd);
	if (!ParseStr(strCmd))
		return false;

	return true;
}


// 문자열을 읽어서 파싱한다.
bool cProtocolParser::ParseStr(const string &source)
{
	using namespace script;

	std::map<string, FEILD_TYPE::TYPE> typeMap;
	typeMap["int"] = FEILD_TYPE::T_INT;
	typeMap["uint"] = FEILD_TYPE::T_UINT;
	typeMap["float"] = FEILD_TYPE::T_FLOAT;
	typeMap["bool"] = FEILD_TYPE::T_BOOL;

	m_fields.clear();
	m_fields.reserve(64);

	vector<string> out;
	out.reserve(64);
	common::tokenizer(source, "\r\n", "", out);

	for each (auto &str in out)
	{
		vector<string> tok;
		common::tokenizer(str, ",", "", tok);

		if (tok.size() >= 2)
		{
			sUDPField field;
			field.bytes = atoi(tok[0].c_str());
			
			// 주석을 뜻하는 #문자 이후의 문자를 무시한다.
			const int index1 = tok[1].find_first_of('#');
			string type;
			if (string::npos == index1)
				type = tok[1];
			else
				type = tok[1].substr(0, index1-1);

			const string t = trim(type);
			field.type = typeMap[t];

			if (field.type > 0)
			{ 
				m_fields.push_back(field);
			}
			else
			{
				AfxMessageBox(CString(L"Protocol Parse Field Error --> ") + str2wstr(type).c_str());
			}
		}
	}

	m_fieldsByteSize = 0;
	for each (auto &f in m_fields)
	{
		m_fieldsByteSize += f.bytes;
	}

	return true;
}

