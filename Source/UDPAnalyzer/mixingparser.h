#pragma once


class cMixingParser
{
public:
	cMixingParser();
	virtual ~cMixingParser();

	bool Read(const string &fileName);
	bool ParseStr(const string &source);
};
