// Copyright 2022 Gaocheng Ran <rangaocheng@outlook.com>

// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at

//     http://www.apache.org/licenses/LICENSE-2.0

// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once
#include "System.h"
//#include "Synchronize.h"
#ifdef WINDOWS_PLATFORM
#include <Windows.h>
#endif // WINDOWS_PLATFORM
#include <new.h>
#define NOELLE_NEW new
#define NOELLE_DELETE delete
#define USE_STL_TYPE_TRAIT
#ifdef USE_STL_TYPE_TRAIT
#include <type_traits>
#endif // USE_STL_TYPE_TRAIT

namespace Noelle
{
	template< class T > inline T Align(const T Ptr, USIZE_TYPE Alignment)
	{
		return (T)(((USIZE_TYPE)Ptr + Alignment - 1) & ~(Alignment - 1));

	}

	class SYSTEM_API MemManager
	{
	public:
		MemManager();
		virtual ~MemManager();

		// Allocate memory
		virtual void* Allocate(USIZE_TYPE uiSize, USIZE_TYPE uiAlignment, bool bIsArray) = 0;
		// Deallocate memory
		virtual void Deallocate(char* pcAddr, USIZE_TYPE uiAlignment, bool bIsArray) = 0;
	};

#if WINDOWS_PLATFORM
#if _DEBUG
	class SYSTEM_API DebugMem : public MemManager
	{
	public:
		DebugMem();
		~DebugMem();

		virtual void* Allocate(USIZE_TYPE uiSize, USIZE_TYPE uiAlignment, bool bIsArray);
		virtual void Deallocate(char* pcAddr, USIZE_TYPE uiAlignment, bool bIsArray);

	private:
		enum
		{
			BEGIN_MASK = 0xDEADC0DE,
			END_MASK = 0xDEADC0DE,
			RECORD_NUM = 32,
			CALLSTACK_NUM = 32
		};

		class Block
		{
		public:
			Block()
			{
				for (USIZE_TYPE i = 0; i < CALLSTACK_NUM; i++)
				{
					pAddr[i] = nullptr;
				}
				m_pPrev = nullptr;
				m_pNext = nullptr;
			}
			void* pAddr[CALLSTACK_NUM];  // call stack information when allocating memory
			USIZE_TYPE m_uiStackInfoNum;  // call stack layers
			USIZE_TYPE m_uiSize;  // size of allocated memory
			bool m_bIsArray;
			bool m_bAlignment;
			Block* m_pPrev;
			Block* m_pNext;
		};

		Block* m_pHead;
		Block* m_pTail;

		USIZE_TYPE m_uiNumNewCalls; // the number of times "New" was called
		USIZE_TYPE m_uiNumDeleteCalls;  // the number of times "Delete" was called
		USIZE_TYPE m_uiNumBlocks;  // the number of current blocks 
		USIZE_TYPE m_uiNumBytes;  // the number of current bytes
		USIZE_TYPE m_uiMaxNumBytes;  // Maximum number of bytes requested
		USIZE_TYPE m_uiMaxNumBlocks;  // Maximum number of blocks requested

		USIZE_TYPE m_uiSizeRecord[RECORD_NUM];  // the number of memory requests recorded in the n power of 2 range
		void InsertBlock(Block* pBlock);
		void RemoveBlock(Block* pBlock);
		bool InitDbgHelpLib();
		void FreeDbgHelpLib();
		bool GetFileAndLine(const void* pAddress, TCHAR szFile[MAX_PATH], int& line);
		void PrintInfo();
		void FreeLeakMem();
	};
#elif _WIN64
	class SYSTEM_API MemWin64 : public MemManager
	{
	public:
		MemWin64();
		~MemWin64();
		virtual void* Allocate(USIZE_TYPE uiSize, USIZE_TYPE uiAlignment, bool bIsArray);
		virtual void Deallocate(char* pcAddr, USIZE_TYPE uiAlignment, bool bIsArray);
	};
#else
	class SYSTEM_API MemWin32 : public MemManager
	{
	public:
		MemWin32();
		~MemWin32();
		virtual void* Allocate(USIZE_TYPE uiSize, USIZE_TYPE uiAlignment, bool bIsArray);
		virtual void Deallocate(char* pcAddr, USIZE_TYPE uiAlignment, bool bIsArray);
	private:
		enum { POOL_CNT = 42 };  // magic numble from ue3
		enum { POOL_MAX = 32768 + 1 };

		struct PoolInfo;
		struct FreeMem;

		struct PoolTable
		{
			DWORD blockSize;

			PoolInfo* exaustedPool;  // ...
			PoolInfo* firstPool;  // ...
		};

		struct PoolInfo
		{
			DWORD bytes;  // Bytes allocated for pool.
			DWORD osBytes;  // Bytes aligned to page size.
			DWORD taken;  // Number of allocated elements in this pool, when counts down to 0 can free the entire pool.

			PoolTable* table;

			PoolInfo* next;
			PoolInfo** preLink;

			BYTE* mem;  // ...
			FreeMem* firstMem;  // ... 

			void Link(PoolInfo*& before)
			{
				if (before)
				{
					before->preLink = &next;
				}
				next = before;
				preLink = &before;
				before = this;
			}

			void Unlink()
			{
				if (next)
				{
					next->preLink = preLink;
				}
				*preLink = next;
			}
		};

		struct FreeMem
		{
			DWORD blocks;

			FreeMem* next;
			PoolInfo* GetPool();
		};

		PoolTable poolTable[POOL_CNT];
		PoolTable osTable;
		PoolTable* memSize2PoolTable[POOL_MAX];

		PoolInfo* poolIndirect[32];  // Used to find the PoolInfo pointer based on the given address (2^5, 2^11)
		DWORD pageSize;

		PoolInfo* CreateIndirect()
		{
			PoolInfo* indirect = (PoolInfo*)VirtualAlloc(nullptr, 2048 * sizeof(PoolInfo), MEM_COMMIT, PAGE_READWRITE);
			if (!indirect)
			{
				return nullptr;
			}
			return indirect;
		}
	};
#endif
#endif
	class SYSTEM_API CMem : public MemManager
	{
	public:
		CMem();
		~CMem();
		virtual void* Allocate(USIZE_TYPE uiSize, USIZE_TYPE uiAlignment, bool bIsArray);
		virtual void Deallocate(char* pcAddr, USIZE_TYPE uiAlignment, bool bIsArray);
	};

	class SYSTEM_API MemObject
	{
	public:
		MemObject();
		~MemObject();
		static MemManager& GetMemManager();
		static MemManager& GetCMemManager();
	};
}

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