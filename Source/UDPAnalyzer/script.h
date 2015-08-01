// 믹싱 스크립트 구조체 선언

#pragma once

namespace script 
{

	struct sFactor;

	namespace OP_MULTI {
		enum TYPE {
			MULTI, DIVIDE
		};
	}

	namespace OP_PLUS {
		enum TYPE {
			PLUS, MINUS
		};
	}

	namespace FACTOR_TYPE {
		enum  TYPE
		{
			ID,
			NUMBER,
			EXPR,
		};
	}

	struct sTerm
	{
		sFactor *factor1;
		OP_MULTI::TYPE op;
		sFactor *factor2;
	};

	struct sExpr
	{
		sTerm *term1;
		OP_PLUS::TYPE op;
		sExpr *expr;
	};

	struct sFactor
	{
		FACTOR_TYPE::TYPE type;
		string id;
		float number;
		sExpr *expr;
	};

	struct sAssignStmt
	{
		string id;
		sExpr *expr;
	};

	struct sStatement
	{
		sAssignStmt *assign;
		sStatement *next;
	};


	void rm_statement(sStatement *p);
	void rm_assignstmt(sAssignStmt *p);
	void rm_factor(sFactor *p);
	void rm_term(sTerm *p);
	void rm_expr(sExpr *p);
}

