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
#ifdef USE_STL_TYPE_TRAIT
#else
	#if _MSC_VER >= 1400
	#define HAS_TRIVIAL_CONSTRUCTOR(T) __has_trivial_constructor(T)
	#define HAS_TRIVIAL_DESTRUCTOR(T) __has_trivial_destructor(T)
	#define HAS_TRIVIAL_ASSIGN(T) __has_trivial_assign(T)
	#define HAS_TRIVIAL_COPY(T) __has_trivial_copy(T)
	#define IS_POD(T) __is_pod(T)
	#define IS_ENUM(T) __is_enum(T)
	#define IS_EMPTY(T) __is_empty(T)
	#else
	#define HAS_TRIVIAL_CONSTRUCTOR(T) false
	#define HAS_TRIVIAL_DESTRUCTOR(T) false
	#define HAS_TRIVIAL_ASSIGN(T) false
	#define HAS_TRIVIAL_COPY(T) false
	#define IS_POD(T) false
	#define IS_ENUM(T) false
	#define IS_EMPTY(T) false
	#endif

#endif

	template< class T > inline T Align(const T Ptr, USIZE_TYPE Alignment)
	{
		return (T)(((USIZE_TYPE)Ptr + Alignment - 1) & ~(Alignment - 1));

	}

	class SYSTEAPI MemManager
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
	class SYSTEAPI DebugMem : public MemManager
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
				pPrev = nullptr;
				pNext = nullptr;
			}
			void* pAddr[CALLSTACK_NUM];  // call stack information when allocating memory
			USIZE_TYPE uiStackInfoNum;  // call stack layers
			USIZE_TYPE uiSize;  // size of allocated memory
			bool bIsArray;
			bool bAlignment;
			Block* pPrev;
			Block* pNext;
		};

		Block* pHead;
		Block* pTail;

		USIZE_TYPE uiNumNewCalls; // the number of times "New" was called
		USIZE_TYPE uiNumDeleteCalls;  // the number of times "Delete" was called
		USIZE_TYPE uiNumBlocks;  // the number of current blocks 
		USIZE_TYPE uiNumBytes;  // the number of current bytes
		USIZE_TYPE uiMaxNumBytes;  // Maximum number of bytes requested
		USIZE_TYPE uiMaxNumBlocks;  // Maximum number of blocks requested

		USIZE_TYPE uiSizeRecord[RECORD_NUM];  // the number of memory requests recorded in the n power of 2 range
		void InsertBlock(Block* pBlock);
		void RemoveBlock(Block* pBlock);
		bool InitDbgHelpLib();
		void FreeDbgHelpLib();
		bool GetFileAndLine(const void* pAddress, TCHAR szFile[MAX_PATH], int& line);
		void PrintInfo();
		void FreeLeakMem();
	};
#elif _WIN64
	class SYSTEAPI MemWin64 : public MemManager
	{
	public:
		MemWin64();
		~MemWin64();
		virtual void* Allocate(USIZE_TYPE uiSize, USIZE_TYPE uiAlignment, bool bIsArray);
		virtual void Deallocate(char* pcAddr, USIZE_TYPE uiAlignment, bool bIsArray);
	};
#else
	class SYSTEAPI MemWin32 : public MemManager
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
			PoolInfo* indirect = (PoolInfo*)VirtualAlloc(nullptr, 2048 * sizeof(PoolInfo), MECOMMIT, PAGE_READWRITE);
			if (!indirect)
			{
				return nullptr;
			}
			return indirect;
		}
	};
#endif
#endif
	class SYSTEAPI StackMem: public MemManager
	{
		template<class T>
		friend class StackMemAlloc;
	public:
		StackMem(USIZE_TYPE uiDefaultChunkSize = 65536); 
		~StackMem();
		virtual void* Allocate(USIZE_TYPE uiSize, USIZE_TYPE uiAlignment, bool bIsArray);
		virtual void Deallocate(char* pcAddr, USIZE_TYPE uiAlignment, bool bIsArray);
	private:
		struct TaggedMemory
		{
			TaggedMemory* next;
			USIZE_TYPE size;
			USIZE_TYPE data[1];
		};

		USIZE_TYPE defaultChunkSize;
		BYTE* top;  // top of current Chunk
		BYTE* end;  // end of current Chunk
		TaggedMemory* topChunk;
		TaggedMemory* unusedChunk;

		/** The number of marks on this stack. */
		INT numMarks;

		/**
		* Allocate a new chunk of memory of at least minSize size,
		* and return it aligned to Align. Updates the memory stack's
		* Chunks table and ActiveChunks counter.
		*/
		BYTE* AllocateNewChunk(USIZE_TYPE minSize);
		
		/*
		* Remove this Chunk and Chunks before it 
		*/
		void FreeChunks(TaggedMemory* newTopChunk);
	};
	
	template<class T>
	class SYSTEAPI StackMemAlloc: public MemObject
	{
	public:
		StackMemAlloc(USIZE_TYPE uiSize = 0, USIZE_TYPE uiAlignment = 0)
		{
			NOEL_ASSERT(uiSize > 0);
			if (uiSize > 0)
			{
				/*
				* Preserve context
				*/
				StackMem& stackMem = GetStackMemManager();
				uiSize = uiSize;
				top = stackMem.top;
				savedChunk = stackMem.topChunk;
				stackMem.numMarks++;

				/*
				* Allocate memory, call constructor
				*/
				pPtr = (T*)stackMem.Allocate(uiSize, uiAlignment, false);
				NOEL_ASSERT(pPtr);
				if ()
			}
		}
		~StackMemAlloc();
	private:
		BYTE* top;
		StackMem::TaggedMemory* savedChunk;
		T* pPtr;
		USIZE_TYPE uiSize; 
	};

	class SYSTEAPI CMem : public MemManager
	{
	public:
		CMem();
		~CMem();
		virtual void* Allocate(USIZE_TYPE uiSize, USIZE_TYPE uiAlignment, bool bIsArray);
		virtual void Deallocate(char* pcAddr, USIZE_TYPE uiAlignment, bool bIsArray);
	};

	class SYSTEAPI MemObject
	{
	public:
		MemObject();
		~MemObject();
		static StackMem& GetStackMemManager();
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