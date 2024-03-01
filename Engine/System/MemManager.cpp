// Copyright 2024 Gaocheng Ran <rangaocheng@outlook.com>

// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at

//     http://www.apache.org/licenses/LICENSE-2.0

// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "MemManager.h"
using namespace Noelle;

std::recursive_mutex MemManager::m_mtx;

MemManager::MemManager()
{

}
MemManager::~MemManager()
{

}


#ifdef _DEBUG
#if WINDOWS_PLATFORM
#include <DbgHelp.h>
typedef BOOL
(WINAPI
	* tFSymInitializeW)(
		_In_ HANDLE hProcess,
		_In_opt_ PCWSTR UserSearchPath,
		_In_ BOOL fInvadeProcess
		);

typedef BOOL
(WINAPI
	* tFSymGetLineFromAddr64)
	(
		IN  HANDLE                  hProcess,
		IN  DWORD64                 qwAddr,
		OUT PDWORD                  pdwDisplacement,
		OUT PIMAGEHLP_LINE64        Line64
		);

typedef DWORD
(WINAPI
	* tFSymGetOptions)
	(
		VOID
		);

typedef DWORD
(WINAPI
	* tFSymSetOptions)
	(
		IN DWORD   SymOptions
		);

static tFSymGetLineFromAddr64 fnSymGetLineFromAddr64 = NULL;
static tFSymGetOptions fnSymGetOptions = NULL;
static tFSymSetOptions fnSymSetOptions = NULL;
static tFSymInitializeW fnSymInitializeW = NULL;
static HMODULE s_DbgHelpLib = NULL;

bool DebugMem::InitDbgHelpLib()
{
	TCHAR szDbgName[MAX_PATH];
	GetModuleFileName(NULL, szDbgName, MAX_PATH);
	TCHAR* p = (TCHAR*)NoelCsrchr(szDbgName, _T('\\'));
	if (p)
		*p = 0;
	NoelStrcat(szDbgName, MAX_PATH, _T("\\dbghelp.dll"));

	s_DbgHelpLib = LoadLibrary(szDbgName);

	fnSymGetLineFromAddr64 = (tFSymGetLineFromAddr64)GetProcAddress(s_DbgHelpLib, "SymGetLineFromAddr64");
	fnSymGetOptions = (tFSymGetOptions)GetProcAddress(s_DbgHelpLib, "SymGetOptions");
	fnSymSetOptions = (tFSymSetOptions)GetProcAddress(s_DbgHelpLib, "SymSetOptions");
	fnSymInitializeW = (tFSymInitializeW)GetProcAddress(s_DbgHelpLib, "SymInitializeW");
	DWORD SymOpts = fnSymGetOptions();

	SymOpts |= SYMOPT_LOAD_LINES;
	SymOpts |= SYMOPT_FAIL_CRITICAL_ERRORS;
	SymOpts |= SYMOPT_DEFERRED_LOADS;
	SymOpts |= SYMOPT_EXACT_SYMBOLS;

	// This option allows for undecorated names to be handled by the symbol engine.
	SymOpts |= SYMOPT_UNDNAME;

	// Disable by default as it can be very spammy/slow.  Turn it on if you are debugging symbol look-up!
	//		SymOpts |= SYMOPT_DEBUG;

	// Not sure these are important or desirable
	//		SymOpts |= SYMOPT_ALLOW_ABSOLUTE_SYMBOLS;
	//		SymOpts |= SYMOPT_CASE_INSENSITIVE;

	fnSymSetOptions(SymOpts);
	bool Temp = fnSymInitializeW(GetCurrentProcess(), NULL, true);
	return Temp;
}

void DebugMem::FreeDbgHelpLib()
{
	if (s_DbgHelpLib != NULL)
	{
		FreeLibrary(s_DbgHelpLib);
		s_DbgHelpLib = NULL;
	}


	fnSymGetLineFromAddr64 = NULL;
	fnSymGetOptions = NULL;
	fnSymSetOptions = NULL;
	fnSymInitializeW = NULL;
}
bool DebugMem::GetFileAndLine(const void* pAddress, TCHAR szFile[MAX_PATH], int& line)
{

	IMAGEHLP_LINE64 Line;
	Line.SizeOfStruct = sizeof(Line);
	NoelMemset(&Line, 0, sizeof(Line));
	DWORD Offset = 0;

	if (fnSymGetLineFromAddr64(GetCurrentProcess(), (DWORD64)pAddress, &Offset, &Line))
	{
#ifdef  _UNICODE
		NoelMbsToWcs(szFile, MAX_PATH, Line.FileName, MAX_PATH);
#else
		NeolStrCopy(szFile, MAX_PATH, Line.FileName);
#endif
		line = Line.LineNumber;

		return true;
	}
	else
	{
		DWORD error = GetLastError();
		// 		if (error == 487)
		// 		{
		// 			NoelOutputDebugString(_T("No debug info in current module\n"));
		// 		}
		// 		else if (error == 126)
		// 		{
		// 			NoelOutputDebugString(_T("Debug info in current module has not loaded\n"));
		// 		}
		// 		else 
		// 		{
		// 			NoelOutputDebugString(_T("SymGetLineFromAddr64 failed\n"));
		// 		}
		return false;
	}
}
#endif

void DebugMem::FreeLeakMem()
{
	Block* pBlock = m_pHead;
	while (pBlock)
	{
		Block* Temp = pBlock;
		pBlock = pBlock->m_pNext;
		free((void*)Temp);
	}
}

void DebugMem::PrintInfo()
{
	NoelOutputDebugString(_T("#########################  begin print leak mem  ######################\n"));

	NoelOutputDebugString(_T("Max Byte Num: %d\n"), m_uiMaxNumBytes);

	NoelOutputDebugString(_T("Max Block Num: %d\n"), m_uiMaxNumBlocks);

	NoelOutputDebugString(_T("Total Size: %d\n"), m_uiNumBytes);

	NoelOutputDebugString(_T("Block Num: %d\n"), m_uiNumBlocks);

	NoelOutputDebugString(_T("New Call: %d\n"), m_uiNumNewCalls);

	NoelOutputDebugString(_T("Delete Call: %d\n"), m_uiNumDeleteCalls);

	if (m_pHead)
	{
		NoelOutputDebugString(_T("Memory Leak:\n"));
	}
	else
	{
		NoelOutputDebugString(_T("No Memory Leak\n"));
	}
	Block* pBlock = m_pHead;
	static USIZE_TYPE uiLeakNum = 0;
	while (pBlock)
	{
		uiLeakNum++;
		NoelOutputDebugString(_T("$$$$$$$$$$$$$$$$  Leak %d  $$$$$$$$$$$$$$$$$\n"), uiLeakNum);
		NoelOutputDebugString(_T("Size: %d\n"), pBlock->m_uiSize);
		NoelOutputDebugString(_T("Is Array:%d\n"), pBlock->m_bIsArray);
#if WINDOWS_PLATFORM
		TCHAR szFile[MAX_PATH];
		int	  line;
		for (USIZE_TYPE i = 0; i < pBlock->m_uiStackInfoNum; i++)
		{

			if (!GetFileAndLine(pBlock->pAddr[i], szFile, line))
			{
				break;
			}
			NoelOutputDebugString(_T("%s(%d)\n"), szFile, line);

		}
#endif
		NoelOutputDebugString(_T("$$$$$$$$$$$$$$$$$ Leak %d  $$$$$$$$$$$$$$$$$$$\n"), uiLeakNum);
		pBlock = pBlock->m_pNext;
	}
	NoelOutputDebugString(_T("leak block total num : %d\n"), uiLeakNum);

	NoelOutputDebugString(_T("#########################  end print leak mem  ######################\n"));
}


DebugMem::DebugMem()
{
	m_uiNumNewCalls = 0;
	m_uiNumDeleteCalls = 0;

	m_uiNumBlocks = 0;
	m_uiNumBytes = 0;
	m_uiMaxNumBytes = 0;
	m_uiMaxNumBlocks = 0;

	m_pHead = 0;
	m_pTail = 0;

	for (USIZE_TYPE i = 0; i < RECORD_NUM; i++)
	{
		m_uiSizeRecord[i] = 0;
	}

}

DebugMem::~DebugMem()
{
#if WINDOWS_PLATFORM
	InitDbgHelpLib();
#endif
	PrintInfo();
#if WINDOWS_PLATFORM
	FreeDbgHelpLib();
#endif
	FreeLeakMem();
}

void* DebugMem::Allocate(USIZE_TYPE uiSize, USIZE_TYPE uiAlignment, bool bIsArray)
{
	std::lock_guard<std::recursive_mutex> lock(m_mtx);
	NOEL_ASSERT(uiSize);
	m_uiNumNewCalls++;

	// 
	USIZE_TYPE uiExtendedSize = sizeof(Block) + sizeof(USIZE_TYPE) + uiSize + sizeof(USIZE_TYPE);
	char* pcAddr = (char*)MemObject::GetCMemManager().Allocate(uiExtendedSize, uiAlignment, bIsArray);
	NOEL_ASSERT(pcAddr);
	if (!pcAddr)
		return nullptr;

	// 
	Block* pBlock = (Block*)pcAddr;
	pBlock->m_uiSize = uiSize;
	pBlock->m_bIsArray = bIsArray;

	bool bAlignment = (uiAlignment > 0) ? true : false;
	pBlock->m_bAlignment = bAlignment;
	pBlock->m_uiStackInfoNum = 0;

	//
	InsertBlock(pBlock);

	pcAddr += sizeof(Block);
	// 
	USIZE_TYPE* pBeginMask = (USIZE_TYPE*)(pcAddr);
	*pBeginMask = BEGIN_MASK;
	pcAddr += sizeof(USIZE_TYPE);
	// 
	USIZE_TYPE* pEndMask = (USIZE_TYPE*)(pcAddr + uiSize);
	*pEndMask = END_MASK;

	m_uiNumBlocks++;
	m_uiNumBytes += uiSize;

	if (m_uiNumBlocks > m_uiMaxNumBlocks)
	{
		m_uiMaxNumBlocks = m_uiNumBlocks;
	}

	if (m_uiNumBytes > m_uiMaxNumBytes)
	{
		m_uiMaxNumBytes = m_uiNumBytes;
	}

	USIZE_TYPE uiTwoPowerI = 1;
	int i;
	for (i = 0; i <= RECORD_NUM - 2; i++, uiTwoPowerI <<= 1)
	{
		if (uiSize <= uiTwoPowerI)
		{
			m_uiSizeRecord[i]++;
			break;
		}
	}
	if (i == RECORD_NUM - 1)
	{
		m_uiSizeRecord[i]++;
	}

	return (void*)pcAddr;
}

void DebugMem::Deallocate(char* pcAddr, USIZE_TYPE uiAlignment, bool bIsArray)
{
	std::lock_guard<std::recursive_mutex> lock(m_mtx);
	NOEL_ASSERT(pcAddr);
	m_uiNumDeleteCalls++;
	pcAddr -= sizeof(USIZE_TYPE);

	USIZE_TYPE* pBeginMask = (USIZE_TYPE*)(pcAddr);
	NOEL_ASSERT(*pBeginMask == BEGIN_MASK);

	pcAddr -= sizeof(Block);

	Block* pBlock = (Block*)pcAddr;
	RemoveBlock(pBlock);

	NOEL_ASSERT(m_uiNumBlocks > 0 && m_uiNumBytes >= pBlock->m_uiSize);
	bool bAlignment = (uiAlignment > 0) ? true : false;
	USIZE_TYPE* pEndMask = (USIZE_TYPE*)(pcAddr + sizeof(Block) + sizeof(USIZE_TYPE) + pBlock->m_uiSize);
	NOEL_ASSERT(*pEndMask == END_MASK);

	m_uiNumBlocks--;
	m_uiNumBytes -= pBlock->m_uiSize;

	MemObject::GetCMemManager().Deallocate(pcAddr, uiAlignment, bIsArray);
}

void DebugMem::InsertBlock(Block* pBlock)
{
	if (m_pHead == nullptr)
	{
		pBlock->m_pPrev = nullptr;
		pBlock->m_pNext = nullptr;
		m_pHead = pBlock;
		m_pTail = pBlock;
	}
	else
	{
		pBlock->m_pNext = nullptr;
		pBlock->m_pPrev = m_pTail;
		m_pTail->m_pNext = pBlock;
		m_pTail = pBlock;
	}
}

void DebugMem::RemoveBlock(Block* pBlock)
{
	if (pBlock->m_pPrev)
	{
		pBlock->m_pPrev->m_pNext = pBlock->m_pNext;
	}
	else
	{
		m_pHead = pBlock->m_pNext;
	}

	if (pBlock->m_pNext)
	{
		pBlock->m_pNext->m_pPrev = pBlock->m_pPrev;
	}
	else
	{
		m_pTail = pBlock->m_pPrev;
	}
}

#elif _WIN64
#include <scalable_allocator.h>
MemWin64::MemWin64()
{

}

MemWin64::~MemWin64()
{

}

void* MemWin64::Allocate(USIZE_TYPE uiSize, USIZE_TYPE uiAlignment, bool bIsArray)
{
	if (uiAlignment != 0)
	{
		uiAlignment = max(uiSize >= 16 ? (USIZE_TYPE)16 : (USIZE_TYPE)8, uiAlignment);
		return scalable_aligned_malloc(uiSize, uiAlignment);
	}
	else
	{
		return scalable_malloc(uiSize);
	}
}

void MemWin64::Deallocate(char* pcAddr, USIZE_TYPE uiAlignment, bool bIsArray)
{
	if (!pcAddr)
		return;

	if (uiAlignment != 0)
	{
		scalable_aligned_free(pcAddr);
	}
	else
	{
		scalable_free(pcAddr);
	}
}

#else
MemWin32::MemWin32()
{
	pageSize = 0;

	// Get OS page size
	SYSTEM_INFO SI;
	GetSystemInfo(&SI);
	pageSize = SI.dwPageSize;
	NOEL_ASSERT(!(pageSize & (pageSize - 1)));

	// Init PoolTable
	/* block size
	// 0: 8
	// 1~5: (8<<((i+1)>>2)) + (2<<i)
	// 6~41: (4+((i+7)&3)) << (1+((i+7)>>2))
	*/
	poolTable[0].firstPool = nullptr;
	poolTable[0].exaustedPool = nullptr;
	poolTable[0].blockSize = 8;

	for (DWORD i = 1; i <= 5; ++i)
	{
		poolTable[i].firstPool = nullptr;
		poolTable[i].exaustedPool = nullptr;
		poolTable[i].blockSize = (8 << ((i + 1) >> 2)) + (2 << i);
	}

	for (DWORD i = 6; i < POOL_CNT; ++i)
	{
		poolTable[i].firstPool = nullptr;
		poolTable[i].exaustedPool = nullptr;
		poolTable[i].blockSize = (4 + ((i + 7) & 3)) << (1 + ((i + 7) >> 2));
	}

	NOEL_ASSERT(poolTable[POOL_CNT - 1].blockSize == POOL_MAX - 1)

		DWORD index = 0;
	for (DWORD j = 0; j < POOL_CNT; ++j)
	{
		while (index <= poolTable[j].blockSize)
		{
			memSize2PoolTable[index] = &poolTable[j];
			++index;
		}
	}

	// Init PoolIndirect
	for (DWORD i = 0; i < 32; ++i)
	{
		poolIndirect[i] = nullptr;
	}
}

MemWin32::~MemWin32()
{
	for (int i = 0; i < 32; ++i)
	{
		for (int j = 0; j < 2048; ++j)
		{
			if (poolIndirect[i] == nullptr)
				break;

			if (poolIndirect[i][i].mem)
			{
				VirtualFree(poolIndirect[i][i].mem, 0, MEM_RELEASE);
				poolIndirect[i][i].mem = nullptr;
			}

			VirtualFree(poolIndirect[i], 0, MEM_RELEASE);
			poolIndirect[i] = nullptr;
		}
	}
}

void* MemWin32::Allocate(USIZE_TYPE uiSize, USIZE_TYPE uiAlignment, bool bIsArray)
{
	std::lock_guard<std::recursive_mutex> lock(m_mtx);

	FreeMem* free;
	if (uiSize < POOL_MAX)
	{
		PoolTable* table = memSize2PoolTable[uiSize];
		PoolInfo* pool = table->firstPool;
		if (pool == nullptr)
		{
			DWORD blocks = 65536 / table->blockSize;
			DWORD bytes = blocks * table->blockSize;
			NOEL_ASSERT(blocks >= 1);

			// Allocate memory
			free = (FreeMem*)VirtualAlloc(nullptr, bytes, MEM_COMMIT, PAGE_READWRITE);
			if (!free)
			{
				return nullptr;
			}

			// Create pool in the indirect table
			PoolInfo*& indirect = poolIndirect[(DWORD)free >> 27];
			if (!indirect)
			{
				indirect = CreateIndirect();
			}
			pool = &indirect[((DWORD)free >> 16) & 2047];

			// Init pool
			pool->Link(table->firstPool);
			pool->firstMem = free;
			pool->mem = (BYTE*)free;
			pool->table = table;
			pool->bytes = bytes;
			pool->osBytes = Align(bytes, pageSize);
			pool->taken = 1;

			// Create first FreeMem
			free->blocks = blocks;
			free->next = nullptr;
		}

		// Pick first available block and unlink it.
		pool->taken++;
		NOEL_ASSERT(pool->firstMem);
		NOEL_ASSERT(pool->firstMem->blocks > 0);
		free = (FreeMem*)((BYTE*)pool->firstMem + (--pool->firstMem->blocks) * table->blockSize);

		if (pool->firstMem->blocks == 0)
		{
			pool->firstMem = pool->firstMem->next;
			if (pool->firstMem == nullptr)
			{
				// Move to exhausted list.
				pool->Unlink();
				pool->Link(table->exaustedPool);
			}
		}
	}
	else
	{
		// Use OS for large allocation
		DWORD alignedBytes = Align(uiSize, pageSize);
		free = (FreeMem*)VirtualAlloc(nullptr, alignedBytes, MEM_COMMIT, PAGE_READWRITE);

		// Create indirect
		PoolInfo*& indirect = poolIndirect[(DWORD)free >> 27];
		if (!indirect)
		{
			indirect = CreateIndirect();
		}
		PoolInfo* pool = &indirect[((DWORD)free >> 16) & 2047];

		pool->firstMem = free;
		pool->mem = (BYTE*)free;
		pool->table = &osTable;
		pool->bytes = uiSize;
		pool->osBytes = alignedBytes;
	}

	return free;
}

void MemWin32::Deallocate(char* pcAddr, USIZE_TYPE uiAlignment, bool bIsArray)
{
	std::lock_guard<std::recursive_mutex> lock(m_mtx);

	// TODO: mutithread
	NOEL_ASSERT(pcAddr);
	PoolInfo* pool = &poolIndirect[(DWORD)pcAddr >> 27][((DWORD)pcAddr >> 16) & 2047];
	NOEL_ASSERT(pool->bytes != 0);
	if (pool->table != &osTable)
	{
		if (pool->firstMem == nullptr)
		{
			// If this pool was exausted, move to available list.
			pool->Unlink();
			pool->Link(pool->table->firstPool);
		}

		FreeMem* free = (FreeMem*)pcAddr;
		free->blocks = 1;
		free->next = pool->firstMem;
		pool->firstMem = free;

		// Free this pool
		NOEL_ASSERT(pool->taken > 0);
		if (--pool->taken == 0)
		{
			// Free the OS memory
			pool->Unlink();
			VirtualFree(pool->mem, 0, MEM_RELEASE);
			pool->mem = nullptr;
		}
	}
	else
	{
		// Free an OS allocation
		VirtualFree(pcAddr, 0, MEM_RELEASE);
		pool->mem = nullptr;
	}
}
#endif

StackMem::StackMem(USIZE_TYPE uiDefaultChunkSize)
{
	NOEL_ASSERT(uiDefaultChunkSize > sizeof(TaggedMemory));
	m_uiDefaultChunkSize = uiDefaultChunkSize;
	m_pTop = nullptr;
	m_pEnd = nullptr;
	m_pTopChunk = nullptr;
	m_pUnusedChunk = nullptr;
	m_iNumMarks = 0;
}

StackMem::~StackMem()
{
	FreeChunks(nullptr);
	while (m_pUnusedChunk)
	{
		TaggedMemory* temp = m_pUnusedChunk;
		m_pUnusedChunk = m_pUnusedChunk->m_pNext;
		MemObject::GetMemManager().Deallocate((char*)temp, 0, true);
	}
	NOEL_ASSERT(m_iNumMarks == 0);
}

void* StackMem::Allocate(USIZE_TYPE uiSize, USIZE_TYPE uiAlignment, bool bIsArray)
{
	// res is the old top
	BYTE* res = m_pTop;
	if (uiAlignment > 0)
		res = Align<BYTE*>(m_pTop, uiAlignment);

	m_pTop = res + uiSize;

	if (m_pTop > m_pEnd)
	{
		// Make sure to allocate memory that can be aligned
		AllocateNewChunk(uiSize + uiAlignment);
		res = m_pTop;
		if (uiAlignment > 0)
			res = Align<BYTE*>(m_pTop, uiAlignment);
		m_pTop = res + uiSize;
	}

	return res;
}

BYTE* StackMem::AllocateNewChunk(USIZE_TYPE minSize)
{
	TaggedMemory* pChunk = NULL;
	for (TaggedMemory** link = &m_pUnusedChunk; *link; link = &(*link)->m_pNext)
	{
		// Find existing chunk.
		if ((*link)->m_uiSize >= minSize)
		{
			pChunk = *link;
			*link = (*link)->m_pNext;
			break;
		}
	}

	if (!pChunk)
	{
		// Create new chunk.
		USIZE_TYPE uiDataSize = Max(minSize, m_uiDefaultChunkSize - sizeof(TaggedMemory));
		pChunk = (TaggedMemory*)MemObject::GetMemManager().Allocate(uiDataSize + sizeof(TaggedMemory), 0, true);
		pChunk->m_uiSize = uiDataSize;
	}

	pChunk->m_pNext = m_pTopChunk;
	m_pTopChunk = pChunk;
	m_pTop = pChunk->m_uiData;
	m_pEnd = m_pTop + pChunk->m_uiSize;
	return m_pTop;
}

void StackMem::FreeChunks(TaggedMemory* newTopChunk)
{
	while (m_pTopChunk != newTopChunk)
	{
		TaggedMemory* removeTrunk = m_pTopChunk;
		m_pTopChunk = m_pTopChunk->m_pNext;
		removeTrunk->m_pNext = m_pUnusedChunk;
		m_pUnusedChunk = removeTrunk;
	}

	m_pTop = nullptr;
	m_pEnd = nullptr;

	if (m_pTopChunk)
	{
		m_pTop = m_pTopChunk->m_uiData;
		m_pEnd = m_pTop + m_pTopChunk->m_uiSize;
	}
}


CMem::CMem()
{

}

CMem::~CMem()
{

}

void* CMem::Allocate(USIZE_TYPE uiSize, USIZE_TYPE uiAlignment, bool bIsArray)
{
	std::lock_guard<std::recursive_mutex> lock(m_mtx);

	if (uiAlignment == 0)
		return malloc(uiSize);
	else
		return _aligned_malloc(uiSize, uiAlignment);
	return nullptr;
}

void CMem::Deallocate(char* pcAddr, USIZE_TYPE uiAlignment, bool bIsArray)
{
	std::lock_guard<std::recursive_mutex> lock(m_mtx);

	if (uiAlignment == 0)
		return free(pcAddr);
	else
		return _aligned_free(pcAddr);
}

StackMem& MemObject::GetStackMemManager()
{
	// todo: multithread
	static StackMem g_StackMemManager;
	return g_StackMemManager;
}


MemManager& MemObject::GetMemManager()
{
#if WINDOWS_PLATFORM
#ifdef _DEBUG
	static DebugMem g_MemManager;
#elif _WIN64
	static MemWin64 g_MemManager;
#else
	static MemWin32 g_MemManager;
#endif
#endif
	return g_MemManager;
}

MemManager& MemObject::GetCMemManager()
{
	static CMem g_MemManager;
	return g_MemManager;
}

MemObject::MemObject()
{
	GetCMemManager();
	GetMemManager();
	GetStackMemManager();
}
MemObject::~MemObject()
{

}