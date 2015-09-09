//
// 실행파일을 실행하고, 종료될 때 메세지를 보낸다.
//
// 프로그램 하나만 실행할 수 있다. 두개 이상의 프로그램은 실행할 수 없다.
//
// Execute() 함수를 호출한 후, cExeTracker 인스턴스가 계속 유지되어야 한다.
//
#pragma once


class cExeTracker
{
public:
	cExeTracker();
	virtual ~cExeTracker();


	bool Execute(const string &fileName, const int id, void(*terminateCallBack)(int,void*), void*arg);


public:
	int m_id;
	string m_exeFileName;
	void (*m_terminateCallBack)(int,void*); // 프로그램이 종료될 때 호출되는 함수 포인터

	HANDLE m_handle;
	DWORD m_threadId;
	void *m_arg;
};

