//
// Plot �Է����� �� ��Ʈ���� ����� ��ɾ �м��� �� �ʿ���, �Լ� �� ����ü�� �����Ѵ�.
//
#pragma once

#include "protocolparser.h"


namespace plotinputscript
{

	struct sStatement
	{
		string symbol;
		string str;
		sStatement *next;
	};
	
	
	void rm_statement(sStatement *p);
}
