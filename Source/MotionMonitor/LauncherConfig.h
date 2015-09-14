//
// 런쳐 설정파일을 관리한다.
//
#pragma once


class cLauncherConfig
{
public:
	cLauncherConfig();
	cLauncherConfig(const string &fileName);
	virtual ~cLauncherConfig();

	bool Read(const string &fileName);

	
public:
	string m_cameraCalibrationExePath;
	string m_cameraCheckExePath;
	string m_gameExePath;
	string m_cameraExePath;
};
