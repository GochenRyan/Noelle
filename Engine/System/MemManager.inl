#include "MemManager.h"

inline void* operator new (USIZE_TYPE uiSize)
{
	return Noelle::MemObject::GetMemManager().Allocate(uiSize, false, false);
}

inline void* operator new[](USIZE_TYPE uiSize)
{
	return Noelle::MemObject::GetMemManager().Allocate(uiSize, false, true);
}

inline void operator delete (void* pvAddr)
{
	Noelle::MemObject::GetMemManager().Deallocate((char*)pvAddr, false, false);
}

inline void operator delete[](void* pvAddr)
{
	Noelle::MemObject::GetMemManager().Deallocate((char*)pvAddr, false, true);
}