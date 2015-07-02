//
// UDP ��Ʈ��ũ�� ���� ��Ŷ�� ������,�̺�Ʈ�� �ޱ� ���� ������ ��ü��.
//
//
#pragma once


class iUDPObserver
{
public:
	virtual void UpdateUDP(const char *buffer, const int bufferLen) = 0;
};


class cUDPObservable
{
public:
	cUDPObservable();
	virtual ~cUDPObservable();

	void AddUDPObserver(iUDPObserver* observer);
	void RemoveUDPObserver(iUDPObserver* observer);
	void NotifyUDPObserver(const char *buffer, const int bufferLen);


protected:
	vector<iUDPObserver*> m_udpObservers; // reference
};




inline cUDPObservable::cUDPObservable()
{
	m_udpObservers.reserve(16);
}

inline cUDPObservable::~cUDPObservable()
{
	m_udpObservers.clear();
}

inline void cUDPObservable::AddUDPObserver(iUDPObserver* observer)
{
	RemoveUDPObserver(observer);
	m_udpObservers.push_back(observer);
}

inline void cUDPObservable::RemoveUDPObserver(iUDPObserver* observer)
{
	common::removevector(m_udpObservers, observer);
}

inline void cUDPObservable::NotifyUDPObserver(const char *buffer, const int bufferLen)
{
	BOOST_FOREACH(auto &observer, m_udpObservers)
		observer->UpdateUDP(buffer, bufferLen);
}
