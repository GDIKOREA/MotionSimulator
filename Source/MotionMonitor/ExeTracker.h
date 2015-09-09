//
// ���������� �����ϰ�, ����� �� �޼����� ������.
//
// ���α׷� �ϳ��� ������ �� �ִ�. �ΰ� �̻��� ���α׷��� ������ �� ����.
//
// Execute() �Լ��� ȣ���� ��, cExeTracker �ν��Ͻ��� ��� �����Ǿ�� �Ѵ�.
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
	void (*m_terminateCallBack)(int,void*); // ���α׷��� ����� �� ȣ��Ǵ� �Լ� ������

	HANDLE m_handle;
	DWORD m_threadId;
	void *m_arg;
};

