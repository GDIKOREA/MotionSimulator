
#include "stdafx.h"
#include "ExeTracker.h"


unsigned WINAPI ExeTrackerThreadFunction(void* arg);
BOOL SafeTerminateProcess(HANDLE hProcess, UINT uExitCode);


cExeTracker::cExeTracker() 
	: m_terminateCallBack(NULL)
	, m_handle(NULL)
	, m_arg(NULL)
	, m_process(NULL)
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


void cExeTracker::Termaniate()
{
	//TerminateProcess(m_process, 0);
	if (m_process)
	{
		SafeTerminateProcess(m_process, 0);
		WaitForSingleObject(m_handle, INFINITE); // �����尡 ����� ������ ����Ѵ�.
	}
}


//http://www.digipine.com/index.php?mid=programming&sort_index=last_update&order_type=desc&page=3&listStyle=gallery&document_srl=570
BOOL SafeTerminateProcess(HANDLE hProcess, UINT uExitCode)
{
	DWORD dwTID, dwCode, dwErr = 0;
	HANDLE hProcessDup = INVALID_HANDLE_VALUE;
	HANDLE hRT = NULL;
	HINSTANCE hKernel = GetModuleHandle(L"Kernel32");

	BOOL bSuccess = FALSE;
	BOOL bDup = DuplicateHandle(GetCurrentProcess(),
		hProcess,
		GetCurrentProcess(),
		&hProcessDup,
		PROCESS_ALL_ACCESS,
		FALSE,
		0);
	if (GetExitCodeProcess((bDup) ? hProcessDup : hProcess, &dwCode)
		&& (dwCode == STILL_ACTIVE))
	{
		FARPROC pfnExitProc;
		pfnExitProc = GetProcAddress(hKernel, "ExitProcess");
		hRT = CreateRemoteThread((bDup) ? hProcessDup : hProcess,
			NULL,
			0,
			(LPTHREAD_START_ROUTINE)pfnExitProc,
			(PVOID)uExitCode, 0, &dwTID);
		if (hRT == NULL) dwErr = GetLastError();
	}
	else
	{
		dwErr = ERROR_PROCESS_ABORTED;
	}
	if (hRT)
	{
		WaitForSingleObject((bDup) ? hProcessDup : hProcess, INFINITE);
		CloseHandle(hRT);
		bSuccess = TRUE;
	}
	if (bDup)
		CloseHandle(hProcessDup);
	if (!bSuccess)
		SetLastError(dwErr);

	return bSuccess;
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

	tracker->m_process = ProcessInfo.hProcess;

	if (result)
	{
		WaitForSingleObject(ProcessInfo.hProcess, INFINITE);
	}

	tracker->m_terminateCallBack(tracker->m_id, tracker->m_arg);
	tracker->m_handle = NULL;

	return 0;
}
