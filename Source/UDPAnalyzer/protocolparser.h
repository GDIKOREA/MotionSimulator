#pragma once


namespace FEILD_TYPE
{
	enum TYPE {
		T_INT = 1,
		T_UINT,
		T_FLOAT,
		T_BOOL,
	};
};

struct sUDPField
{
	int bytes;
	FEILD_TYPE::TYPE type;
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
