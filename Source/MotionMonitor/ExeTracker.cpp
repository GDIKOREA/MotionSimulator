
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


// ���α׷��� �����Ѵ�.
// ���α׷��� ����Ǹ�, terminateCallBack �Լ��� ȣ���Ѵ�.
bool cExeTracker::Execute(const string &fileName, const string &commandLine, const int id, void (*terminateCallBack)(int,void*), void*arg)
{
	if (m_handle)
		return false; // �̹� ���� ���̸�, �ٽ� ���� �� �� ����.

	m_id = id;
	m_exeFileName = fileName;
	m_commandLine = commandLine;
	m_arg = arg;
	m_terminateCallBack = terminateCallBack;

	m_handle = (HANDLE)_beginthreadex(NULL, 0, ExeTrackerThreadFunction, this, 0, (unsigned*)&m_threadId);

	return true;
}


// ���α׷��� �����ϰ�, ����� ������ ����Ѵ�.
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

