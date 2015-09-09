
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
bool cExeTracker::Execute(const string &fileName, const int id, void (*terminateCallBack)(int,void*), void*arg)
{
	if (m_handle)
		return false; // �̹� ���� ���̸�, �ٽ� ���� �� �� ����.

	m_id = id;
	m_exeFileName = fileName;
	m_arg = arg;
	m_terminateCallBack = terminateCallBack;

	m_handle = (HANDLE)_beginthreadex(NULL, 0, ExeTrackerThreadFunction, this, 0, (unsigned*)&m_threadId);

	return true;
}


// ���α׷��� �����ϰ�, ����� ������ ����Ѵ�.
unsigned WINAPI ExeTrackerThreadFunction(void* arg)
{
	cExeTracker *tracker = (cExeTracker*)arg;

	const string exeDir = common::GetFilePathExceptFileName(tracker->m_exeFileName) + "\\";
	const string exeName = common::GetFileName(tracker->m_exeFileName);

	STARTUPINFOA StartupInfo = { 0 };
	StartupInfo.cb = sizeof(STARTUPINFOA);
	PROCESS_INFORMATION ProcessInfo;

	CONST BOOL result = CreateProcessA(exeName.c_str(), NULL,
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

