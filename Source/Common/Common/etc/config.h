//
// 환경파일 읽고, 저장하는 기능.
//
#pragma once

namespace common
{

	class cConfig
	{
	public:
		cConfig();
		virtual ~cConfig();

		void Init(const string &fileName);
		bool Save(const string &fileName);
		void ParseStr(const string &str);


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

}
