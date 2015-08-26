//
// ��ũ��Ʈ�� �м��ϴµ� �ʿ���, ���� ������ �Լ����� ��Ƴ��Ҵ�.
//
#pragma once


namespace script
{
	// symbol
	namespace FEILD_TYPE
	{
		enum TYPE {
			T_INT = 1,
			T_UINT,
			T_FLOAT,
			T_DOUBLE,
			T_BOOL,
		};
	};

	struct sFieldData
	{
		union
		{
			bool bVal;
			int iVal;
			u_int uVal;
			float fVal;
			double dVal;
			char buff[8];
		};

		FEILD_TYPE::TYPE type;

		bool operator== (const sFieldData &rhs);
	};

	extern map<string, sFieldData> g_symbols;
	void ClearSymbols();



	extern const char *g_numStr;
	extern const int g_numLen;
	extern const char *g_strStr;
	extern const int g_strLen;
	extern const char *g_strStr2;
	extern const int g_strLen2;


	bool compare(string &src, char c);
	bool check(string &src, char c);
}
