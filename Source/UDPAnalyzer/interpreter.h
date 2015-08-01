//
// Mixing 명령어 실행기
//
#pragma once

#include "script.h"


namespace script
{

	class cMixingInterpreter
	{
	public:
		cMixingInterpreter();
		virtual ~cMixingInterpreter();

		float Excute(const sStatement *p);

		map<string, float> m_symbols;


	protected:
		float assignstmt(const sAssignStmt *p);
		float expr(const sExpr*p);
		float term(const sTerm *p);
		float factor(const sFactor *p);
	};

}
