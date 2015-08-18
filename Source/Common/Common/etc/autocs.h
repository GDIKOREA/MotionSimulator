//
// criticalsection 객체를 자동으로 제거하는 클래스다.
//

#pragma once


class cAutoCS
{
public:
	cAutoCS(CRITICAL_SECTION &cs) : m_cs(cs) { EnterCriticalSection(&cs); }
	virtual ~cAutoCS() { LeaveCriticalSection(&m_cs); }
	CRITICAL_SECTION &m_cs;
};

