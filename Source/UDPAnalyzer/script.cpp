
#include "stdafx.h"
#include "script.h"


namespace script
{
	const char *g_numStr = "1234567890.";
	const int g_numLen = 11;
	const char *g_strStr = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ@$_";
	const int g_strLen = 55;
	const char *g_strStr2 = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ@$_1234567890";
	const int g_strLen2 = 65;

	map<string, sFieldData> g_symbols;
}


// 비교 연산자
// 같은 정보라면 true를 리턴한다.
bool script::sFieldData::operator == (const sFieldData &rhs)
{
	// 타입은 비교하지 않고, 정보만 비교한다.
	return (buff[0] == rhs.buff[0]) &&
		(buff[1] == rhs.buff[1]) &&
		(buff[2] == rhs.buff[2]) &&
		(buff[3] == rhs.buff[3]) &&
		(buff[4] == rhs.buff[4]) &&
		(buff[5] == rhs.buff[5]) &&
		(buff[6] == rhs.buff[6]) &&
		(buff[7] == rhs.buff[7]);
}


void script::ClearSymbols()
{
	g_symbols.clear();
}


bool script::compare(string &src, char c)
{
	if (src.empty())
		return false;
	return src[0] == c;
}


bool script::check(string &src, char c)
{
	if (src.empty())
		return false;
	if (src[0] == c)
	{
		rotatepopvector(src, 0);
	}
	else
	{
		return false;
	}

	return true;
}
