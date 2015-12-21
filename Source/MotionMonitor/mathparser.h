//
// Mixing ��ɾ� ��ũ��Ʈ �Ľ�
//
#pragma once

#include "mathscript.h"


class cMathParser
{
public:
	cMathParser();
	virtual ~cMathParser();

	bool Read(const string &fileName);
	bool ParseStr(const string &source);

	mathscript::sStatement *m_stmt;


protected:
	// BNF
	//- statement -> { assign_stmt  | expression }
	//- assign_stmt -> id = expression
	//- expression -> term +- expression | term
	//- term -> factor */ factor | factor
	//- factor -> id, number, func, (expression)
	//- func -> id( expression )
	// 
	mathscript::sStatement* statement(string &src);
	mathscript::sAssignStmt* assign_stmt(string &src);
	mathscript::sExpr* expression(string &src);
	mathscript::sTerm* term(string &src);
	mathscript::sFactor* factor(string &src);
	mathscript::sFunc* func(const string &name, string &src);
	string id(string &src);
	float number(string &src);
};
