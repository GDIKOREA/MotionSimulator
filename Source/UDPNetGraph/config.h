//
// 환경파일 읽고, 저장하는 기능.
//
#pragma once


class cConfig
{
public:
	cConfig();
	virtual ~cConfig();

	void Init(const string &fileName);
	void Save(const string &fileName);


protected:
	bool Parse(const string &fileName);
	virtual void InitDefault() {}
	virtual void UpdateParseData() {}

	bool GetBool(const string &key);
	float GetFloat(const string &key);
	int GetInt(const string &key);


public:
	map<string, string> m_options;
};
