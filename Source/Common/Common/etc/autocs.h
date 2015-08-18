//
// criticalsection ��ü�� �ڵ����� �����ϴ� Ŭ������.
//

#pragma once


class cAutoCS
{
public:
	cAutoCS(CRITICAL_SECTION &cs) : m_cs(cs) { EnterCriticalSection(&cs); }
	virtual ~cAutoCS() { LeaveCriticalSection(&m_cs); }
	CRITICAL_SECTION &m_cs;
};

