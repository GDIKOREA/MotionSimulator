#pragma once

#include "script.h"


struct sUDPField
{
	int bytes; // 차지하는 메모리 바이트 크기
	script::FEILD_TYPE::TYPE type; // 데이터 타입
};


class cProtocolParser
{
public:
	cProtocolParser();
	virtual ~cProtocolParser();

	bool Read(const string &fileName);
	bool ParseStr(const string &source);

	vector<sUDPField> m_fields;	
	int m_fieldsByteSize;
};
