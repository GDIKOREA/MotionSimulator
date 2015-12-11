
#include "stdafx.h"
#include "sharedmemcommunication.h"



cSharedMemoryCommunication::cSharedMemoryCommunication()
{

}

cSharedMemoryCommunication::~cSharedMemoryCommunication()
{

}


// Ŭ���� �ʱ�ȭ.
bool cSharedMemoryCommunication::Init(const string &sharedMemoryName)
{
	try
	{
		//shared_memory_object::remove(sharedMemoryName.c_str());
		m_sharedmem = shared_memory_object(open_or_create, sharedMemoryName.c_str(), read_write);

		// setting the size of the shared memory
		m_sharedmem.truncate(512);

		// map the shared memory to current process 
		m_mmap = mapped_region(m_sharedmem, read_write, 0, 256);

		// access the mapped region using get_address
		//strcpy(static_cast<char*>(mmap.get_address()), "Hello World!\n");

	}
	catch (interprocess_exception&e)
	{
		std::cout << e.what() << std::endl;
		std::cout << "Shared Memory Error!!";
		return false;
	}

	return true;
}


// ���� �޸� �ּҸ� �����Ѵ�.
void* cSharedMemoryCommunication::GetMemoryPtr()
{
	return m_mmap.get_address();
}
