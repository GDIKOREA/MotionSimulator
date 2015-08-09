#pragma once

#include "script.h"


struct sUDPField
{
	int bytes; // �����ϴ� �޸� ����Ʈ ũ��
	script::FEILD_TYPE::TYPE type; // ������ Ÿ��
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
