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
#include "Platform.h"
#include "System.h"
#if WINDOWS_PLATFORM
#include <tchar.h>
#include <memory.h>
#include <atlsimpstr.h>
#endif

#ifdef SYSTEM_EXPORTS
	#define SYSTEM_API __declspec(dllexport) 
#else 
	#define SYSTEM_API __declspec(dllimport) 
#endif

namespace Noelle
{
#ifdef _DEBUG
#define NOEL_ASSERT(Expression)\
	{\
		 assert(Expression);\
	}
#else
#define NOEL_ASSERT(Expression)
#endif


#define LOG_BUFFER_SIZE 65536	 
	class SYSTEM_API System
	{
	public:
		static TCHAR ms_sLogBuffer[LOG_BUFFER_SIZE];
		static DWORD ms_dwMainThreadID;
	};

	inline const TCHAR* NoelCsrchr(const TCHAR* pString, int c)
	{
#ifdef WINDOWS_PLATFORM
		return _tcsrchr(pString, c);
#else
		static_assert(0, "No Implement!");;
		return NULL;
#endif
	}

	inline void NoelStrcat(TCHAR* pDest, USIZE_TYPE uiCount, const TCHAR* pSource)
	{
#ifdef WINDOWS_PLATFORM
		_tcscat_s(pDest, uiCount, pSource);
#else
		static_assert(0, "No Implement!");;
		return;
#endif
	}

	inline void NoelMbsToWcs(wchar_t* Dest, USIZE_TYPE uiSizeInWord, const char* Source, USIZE_TYPE uiSizeInByte)
	{
#ifdef WINDOWS_PLATFORM
		mbstowcs_s(0, Dest, uiSizeInWord, Source, uiSizeInByte);
#else
		static_assert(0, "No Implement!");;
		return;
#endif
	}

	inline void NoelMemset(void* pDest, int iC, USIZE_TYPE uiCount)
	{
#ifdef WINDOWS_PLATFORM
		memset(pDest, iC, uiCount);
#else
		static_assert(0, "No Implement!");;
		return;
#endif
	}

	inline void NoelOutputDebugString(const TCHAR* pcString, ...)
	{
#ifdef WINDOWS_PLATFORM
		char* pArgs;
		pArgs = (char*)&pcString + sizeof(pcString);
		_vstprintf_s(System::ms_sLogBuffer, LOG_BUFFER_SIZE, pcString, pArgs);
		OutputDebugString(System::ms_sLogBuffer);
#else
		static_assert(0, "No Implement!");;
		return;
#endif
	}
}