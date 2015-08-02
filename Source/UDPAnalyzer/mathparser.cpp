
#include "stdafx.h"
#include "mathparser.h"
#include "script.h"

using namespace mathscript;
using namespace script;


cMathParser::cMathParser() : 
m_stmt(NULL)
{
}

cMathParser::~cMathParser()
{
	if (m_stmt)
	{
		rm_statement(m_stmt);
		m_stmt = NULL;
	}
}


// read script file
bool cMathParser::Read(const string &fileName)
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


// parse script
bool cMathParser::ParseStr(const string &source)
{
	string src = source;

	rm_statement(m_stmt);
	m_stmt = statement(src);

	return (m_stmt != NULL);
}


//statement -> {assign_stmt}
sStatement* cMathParser::statement(string &src)
{
	trim(src);

	if (src.empty())
		return NULL;

	sStatement *stmt = new sStatement({});
	stmt->assign = assign_stmt(src);
	if (!stmt->assign)
		stmt->expr = expression(src);

	stmt->next = statement(src);
	return stmt;
}


// assign_stmt -> id = exp
sAssignStmt* cMathParser::assign_stmt(string &src)
{
	trim(src);

	sAssignStmt *assign = new sAssignStmt({});
	const int idx = src.find_first_of('=');
	if (idx == string::npos)
	{
		delete assign;
		return NULL;
	}

	assign->id = id(src);
	trim(src);
	check(src, '=');
	trim(src);
	assign->expr = expression(src);
	return assign;
}


//- exp -> term +- expr | term
sExpr* cMathParser::expression(string &src)
{
	trim(src);

	sExpr *expr = new sExpr({});

	expr->term1 = term(src);
	trim(src);

	if (check(src, '+'))
	{
		expr->op = OP_PLUS::PLUS;
		expr->expr = expression(src);
	}
	else if (check(src, '-'))
	{
		expr->op = OP_PLUS::MINUS;
		expr->expr = expression(src);
	}

	return expr;
}


//term -> factor */ factor | factor
sTerm* cMathParser::term(string &src)
{
	trim(src);

	sTerm *t = new sTerm({});
	t->factor1 = factor(src);
	trim(src);

	if (check(src, '*'))
	{
		t->op = OP_MULTI::MULTI;
		t->factor2 = factor(src);
	}
	else if (check(src, '/'))
	{
		t->op = OP_MULTI::DIVIDE;
		t->factor2 = factor(src);
	}

	return t;
}


//factor -> id, number, (exp)
sFactor* cMathParser::factor(string &src)
{
	trim(src);

	sFactor *factor = new sFactor({});

	bool checkNum = false;
	for (int i = 0; i < g_numLen; ++i)
	{
		if (compare(src, g_numStr[i]))
		{
			checkNum = true;
			break;
		}
	}

	if (checkNum)
	{
		factor->number = number(src);
		factor->type = FACTOR_TYPE::NUMBER;
	}
	else if (check(src, '('))
	{
		factor->expr = expression(src);
		factor->type = FACTOR_TYPE::EXPR;
		check(src, ')');
	}
	else
	{
		factor->id = id(src);
		factor->type = FACTOR_TYPE::ID;
	}

	return factor;
}


// lex -> alphabet + {alphabet or number}
string cMathParser::id(string &src)
{
	trim(src);	

	string str;

	// check start id token
	bool findFirstTok = false;
	for (int i = 0; i < g_strLen; ++i)
	{
		if (compare(src, g_strStr[i]))
		{
			findFirstTok = true;
			break;
		}
	}

	if (!findFirstTok)
		return "";

	str += src[0];
	rotatepopvector(src, 0);
	
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

		str += src[0];
		rotatepopvector(src, 0);
	}

	return str;
}

// lex -> [+,-] {number or .}
float cMathParser::number(string &src)
{
	trim(src);

	string str;
	if (src[0] == '+')
	{
		check(src, '+');
	}
	else if (src[0] == '-')
	{
		check(src, '-');
		str += '-';
	}

	while (!src.empty())
	{
		bool findtok = false;
		for (int i = 0; i < g_numLen; ++i)
		{
			if (compare(src, g_numStr[i]))
			{
				findtok = true;
				break;
			}
		}

		if (!findtok)
			break;

		str += src[0];
		rotatepopvector(src, 0);
	}

	const float n = (float)atof(str.c_str());
	return n;
}

