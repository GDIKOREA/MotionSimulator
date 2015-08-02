
#include "stdafx.h"
#include "script.h"


namespace script
{
	const char *g_numStr = "1234567890.";
	const int g_numLen = 11;
	const char *g_strStr = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ$_";
	const int g_strLen = 54;
	const char *g_strStr2 = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ$_1234567890";
	const int g_strLen2 = 64;

	map<string, sFieldData> g_symbols;
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
