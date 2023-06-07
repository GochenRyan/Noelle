// Copyright 2023 Gaocheng Ran <rangaocheng@outlook.com>

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
#if _WIN32
#define WINDOWS_PLATFORM 1
#endif
enum EPLATFORM
{
	EP_Windows,
	EP_Android,
	EP_IOS,
	EP_MAX,
};
#if WINDOWS_PLATFORM
#if _WIN64
#define SYSTEM_BIT_WIDTH 64
#define USIZE_TYPE unsigned long long
#define SIZE_TYPE long long
#else
#define SYSTEM_BIT_WIDTH 32
#define USIZE_TYPE unsigned int
#define SIZE_TYPE int
#endif
#endif