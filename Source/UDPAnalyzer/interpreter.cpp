
#include "stdafx.h"
#include "interpreter.h"

using namespace script;


cMixingInterpreter::cMixingInterpreter()
{

}

cMixingInterpreter::~cMixingInterpreter()
{

}


float cMixingInterpreter::Excute(const sStatement *p)
{
	RETV(!p, 0);
	
	const float val = assignstmt(p->assign);
	Excute(p->next);
	
	return val;
}


float cMixingInterpreter::assignstmt(const sAssignStmt *p)
{
	RETV(!p, 0);

	const float val = expr(p->expr);
	m_symbols[p->id] = val;

	return val;
}


float cMixingInterpreter::expr(const sExpr*p)
{
	RETV(!p, 0);
	float val = term(p->term1);

	if (p->expr)
	{
		switch (p->op)
		{
		case OP_PLUS::PLUS:
			val += expr(p->expr);
			break;
		case OP_PLUS::MINUS:
			val -= expr(p->expr);
			break;
		}
	}

	return val;
}


float cMixingInterpreter::term(const sTerm *p)
{
	RETV(!p, 0);
	float val = factor(p->factor1);

	if (p->factor2)
	{
		switch (p->op)
		{
		case OP_MULTI::MULTI:
			val *= factor(p->factor2);
			break;
		case OP_MULTI::DIVIDE:
			val /= factor(p->factor2);
			break;
		}
	}

	return val;
}


float cMixingInterpreter::factor(const sFactor *p)
{
	RETV(!p, 0);
	float val = 0;

	switch (p->type)
	{
	case FACTOR_TYPE::ID:
		val = m_symbols[p->id];
		break;
	case FACTOR_TYPE::NUMBER:
		val = p->number;
		break;
	case FACTOR_TYPE::EXPR:
		val = expr(p->expr);
		break;
	}

	return val;
}
