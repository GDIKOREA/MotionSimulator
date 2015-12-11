//
// �����޸𸮸� ����, ���Ӱ� ����� �ϱ����� Ŭ����.
//
#pragma once

#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/allocators/allocator.hpp>

using namespace boost::interprocess;


class cSharedMemoryCommunication
{
public:
	cSharedMemoryCommunication();
	virtual ~cSharedMemoryCommunication();

	bool Init(const string &sharedMemoryName);
	void* GetMemoryPtr();


protected:
	shared_memory_object m_sharedmem;
	mapped_region m_mmap;
};
