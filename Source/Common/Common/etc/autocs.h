//
// criticalsection ��ü�� �ڵ����� �����ϴ� Ŭ������.
//

#pragma once


class cAutoCS
{
public:
	cAutoCS(CRITICAL_SECTION &cs) : 
		m_cs(cs)
		, m_isLeave(false) 
	{ 
		EnterCriticalSection(&cs); 
	}

	virtual ~cAutoCS() 
	{ 
		if (!m_isLeave)
			LeaveCriticalSection(&m_cs); 
		m_isLeave = true;
	}

	void Enter()
	{
		if (m_isLeave)
			EnterCriticalSection(&m_cs);
		m_isLeave = false;
	}

	void Leave() 
	{ 
		LeaveCriticalSection(&m_cs); 
		m_isLeave = true;  
	}

	CRITICAL_SECTION &m_cs;
	bool m_isLeave;
};

