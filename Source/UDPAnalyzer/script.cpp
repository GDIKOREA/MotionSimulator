
#include "stdafx.h"
#include "script.h"

using namespace script;


void script::rm_statement(sStatement *p)
{
	RET(!p);
	rm_assignstmt(p->assign);
	rm_statement(p->next);
	delete p;
}

void script::rm_assignstmt(sAssignStmt *p)
{
	RET(!p);
	rm_expr(p->expr);
	delete p;
}

void script::rm_factor(sFactor *p)
{
	RET(!p);
	rm_expr(p->expr);
	delete p;
}

void script::rm_term(sTerm *p)
{
	RET(!p);
	rm_factor(p->factor1);
	rm_factor(p->factor2);
	delete p;
}

void script::rm_expr(sExpr *p)
{
	RET(!p);
	rm_term(p->term1);
	rm_expr(p->expr);
	delete p;
}

