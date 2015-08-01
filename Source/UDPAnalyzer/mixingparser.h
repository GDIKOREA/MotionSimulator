//
// Mixing 명령어 스크립트 파싱
//
#pragma once

#include "script.h"


class cMixingParser
{
public:
	cMixingParser();
	virtual ~cMixingParser();

	bool Read(const string &fileName);
	bool ParseStr(const string &source);

	script::sStatement *m_stmt;


protected:
	// BNF
	//- statement -> { assign_stmt }
	//- assign_stmt -> id = expression
	//- expression -> term +- expression | term
	//- term -> factor */ factor | factor
	//- factor -> id, number, (expression)
	// 
	script::sStatement* statement(string &src);
	script::sAssignStmt* assign_stmt(string &src);
	script::sExpr* expression(string &src);
	script::sTerm* term(string &src);
	script::sFactor* factor(string &src);
	string id(string &src);
	float number(string &src);
	bool compare(string &src, char c);
	bool check(string&src, char c);
};
