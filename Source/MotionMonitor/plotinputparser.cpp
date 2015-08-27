
#include "stdafx.h"
#include "plotinputparser.h"
#include "script.h"


cPlotInputParser::cPlotInputParser():
m_stmt(NULL)
{
}

cPlotInputParser::~cPlotInputParser()
{
	plotinputscript::rm_statement(m_stmt);
}


// read file and parsing
bool cPlotInputParser::Read(const string &fileName)
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


// read string and parsing
bool cPlotInputParser::ParseStr(const string &source)
{
	string src = source;

	plotinputscript::rm_statement(m_stmt);
	m_stmt = statement(src);
	return m_stmt? true : false;
}


// ����
string cPlotInputParser::Execute()
{
	using namespace script;

	RETV(!m_stmt, "");

	string makeStr;

	plotinputscript::sStatement *p = m_stmt;
	while (p)
	{
		if (!p->symbol.empty())
		{
			auto it = g_symbols.find(p->symbol);
			if (g_symbols.end() != it)
			{
				switch (it->second.type)
				{
				case FEILD_TYPE::T_BOOL:
					makeStr += it->second.bVal ? "true" : "false";
					break;
				case FEILD_TYPE::T_INT:
					makeStr += format("%d", it->second.iVal);
					break;
				case FEILD_TYPE::T_UINT:
					makeStr += format("%u", it->second.uVal);
					break;
				case FEILD_TYPE::T_FLOAT:
					makeStr += format("%f", it->second.fVal);
					break;
				case FEILD_TYPE::T_DOUBLE:
					makeStr += format("%f", it->second.dVal);
					break;
				default:
					makeStr += "XXX";
					break;
				}
			}
			else
			{
				makeStr += "XXX";
			}
		}

		makeStr += p->str;
		p = p->next;
	}

	return makeStr;
}


// {[symbol] + [string]}
plotinputscript::sStatement* cPlotInputParser::statement(string &src)
{
	if (src.empty())
		return NULL;

	plotinputscript::sStatement *p = new plotinputscript::sStatement();
	p->symbol = symbol(src);
	p->str = str(src);
	p->next = statement(src);

	return p;
}


// $ ���ڷ� �����ؼ�, �����̽�, Ư�����ڰ� ���� ������ �о �����Ѵ�. 
// symbol -> ${alphabet + number}
string cPlotInputParser::symbol(string &src)
{
	using namespace script;

	trim(src);

	if (src.empty())
		return "";

	// ��� �ɺ����� $,@���� �����Ѵ�.
	if ((src[0] != '$') && (src[0] != '@'))
		return "";

	string sym;
	sym += src[0];

	check(src, src[0]);

	// check next id token
	while (!src.empty())
	{
		bool findtok = false;
		for (int i = 0; i < g_strLen2; ++i)
		{
			if (compare(src, g_strStr2[i]))
			{
				findtok = true;
				break;
			}
		}

		if (!findtok)
			break;

		// �ɺ��ϳ��� $,@���ڰ� �ι� ���� �� ����.
		if ((src[0] == '$') || (src[0] == '@'))
			break;

		sym += src[0];
		rotatepopvector(src, 0);
	}

	return sym;
}


// lex -> {alphabet + space + special character} except $
string cPlotInputParser::str(string &src)
{
	string ret;
	while (!src.empty())
	{
		if ((src[0] == '$') || (src[0] == '@'))
			break;

		ret += src[0];
		rotatepopvector(src, 0);
	}
	return ret;
}
