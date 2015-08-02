//
// Plot �Է����� �� ��Ʈ���� �����ϴ� ��ɾ �м��ϴ� Ŭ����.
//
#pragma once

#include "plotinputscript.h"


class cPlotInputParser
{
public:
	cPlotInputParser();
	virtual ~cPlotInputParser();

	bool Read(const string &fileName);
	bool ParseStr(const string &source);
	string Execute();


protected:
	plotinputscript::sStatement *m_stmt;

	
	//BNF
	// {[symbol] + [string]}
	plotinputscript::sStatement* statement(string &src);
	string symbol(string &src);
	string str(string &src);
};
