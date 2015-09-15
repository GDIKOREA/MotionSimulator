
#include "stdafx.h"
#include "ExeTracker.h"


unsigned WINAPI ExeTrackerThreadFunction(void* arg);


cExeTracker::cExeTracker() 
	: m_terminateCallBack(NULL)
	, m_handle(NULL)
	, m_arg(NULL)
{
}

cExeTracker::~cExeTracker()
{
}


// 프로그램을 실행한다.
// 프로그램이 종료되면, terminateCallBack 함수를 호출한다.
bool cExeTracker::Execute(const string &fileName, const string &commandLine, const int id, void (*terminateCallBack)(int,void*), void*arg)
{
	if (m_handle)
		return false; // 이미 실행 중이면, 다시 실행 할 수 없다.

	m_id = id;
	m_exeFileName = fileName;
	m_commandLine = commandLine;
	m_arg = arg;
	m_terminateCallBack = terminateCallBack;

	m_handle = (HANDLE)_beginthreadex(NULL, 0, ExeTrackerThreadFunction, this, 0, (unsigned*)&m_threadId);

	return true;
}


// 프로그램을 실행하고, 종료될 때까지 대기한다.
unsigned WINAPI ExeTrackerThreadFunction(void* arg)
{
	cExeTracker *tracker = (cExeTracker*)arg;

	string exeDir = common::GetFilePathExceptFileName(tracker->m_exeFileName);
	if (exeDir.empty())
		exeDir = "./";
	else
		exeDir += "\\";
	const string exeName = tracker->m_exeFileName;

	STARTUPINFOA StartupInfo = { 0 };
	StartupInfo.cb = sizeof(STARTUPINFOA);
	PROCESS_INFORMATION ProcessInfo;

	const BOOL result = CreateProcessA(exeName.c_str(), 
		(char*)tracker->m_commandLine.c_str(),
		NULL, NULL, FALSE,
		NORMAL_PRIORITY_CLASS,
		NULL, exeDir.c_str(), &StartupInfo, &ProcessInfo);

	if (result)
	{
		WaitForSingleObject(ProcessInfo.hProcess, INFINITE);
	}

	tracker->m_terminateCallBack(tracker->m_id, tracker->m_arg);
	tracker->m_handle = NULL;

	return 0;
}

