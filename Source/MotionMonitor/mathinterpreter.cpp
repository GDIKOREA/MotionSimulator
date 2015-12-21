
#include "stdafx.h"
#include "mathinterpreter.h"

using namespace mathscript;


cMathInterpreter::cMathInterpreter()
{
}

cMathInterpreter::~cMathInterpreter()
{
}


float cMathInterpreter::Excute(const sStatement *p)
{
	RETV(!p, 0);
	
	float val = 0;
	if (p->assign)
		val = assignstmt(p->assign);
	else if (p->expr)
		val = expr(p->expr);

	Excute(p->next);
	
	return val;
}


float cMathInterpreter::assignstmt(const sAssignStmt *p)
{
	RETV(!p, 0);

	const float val = expr(p->expr);
	script::g_symbols[p->id].fVal = val;
	script::g_symbols[p->id].type = script::FEILD_TYPE::T_FLOAT;

	return val;
}


float cMathInterpreter::expr(const sExpr*p)
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


float cMathInterpreter::term(const sTerm *p)
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


float cMathInterpreter::func(const sFunc *p)
{
	using namespace script;

	RETV(!p, 0);
	float val = 0;

	if (p->id == "abs")
	{
		val = abs( expr(p->expr) );
	}

	return val;
}


float cMathInterpreter::factor(const sFactor *p)
{
	using namespace script;

	RETV(!p, 0);
	float val = 0;

	switch (p->type)
	{
	case FACTOR_TYPE::ID:
		switch (g_symbols[p->id].type)
		{
		case FEILD_TYPE::T_BOOL: val = (float)(script::g_symbols[p->id].bVal? 1 : 0); break;
		case FEILD_TYPE::T_UINT: val = (float)script::g_symbols[p->id].uVal; break;
		case FEILD_TYPE::T_INT: val = (float)script::g_symbols[p->id].iVal; break;
		case FEILD_TYPE::T_FLOAT: val = script::g_symbols[p->id].fVal; break;
		case FEILD_TYPE::T_DOUBLE: val = (float)script::g_symbols[p->id].dVal; break;
		default:
			break;
		}
		break;
	case FACTOR_TYPE::NUMBER:
		val = p->number;
		break;
	case FACTOR_TYPE::EXPR:
		val = expr(p->expr);
		break;
	case FACTOR_TYPE::FUNC:
		val = func(p->func);
		break;
	}

	return val;
}
