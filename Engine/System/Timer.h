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
#include <chrono>
namespace Noelle
{
	class SYSTEM_API Timer
	{
	private:
		unsigned long long m_ulTimeStart;
		int m_iFrameCount;
		double m_fFPS;
		double m_fTime,m_fLastFPSTime,m_fTimeSlice;
		double m_fDetTime, m_fLastTime;
		void InitGameTime();
	public:
		Timer();
		~Timer();
	
		double GetGamePlayTime();
		void UpdateFPS();
		inline double GetFPS(){return m_fFPS;}
		static Timer * m_pTimer;
		double GetDetTime(){ return m_fDetTime; }
		int GetRandSeed();
	};
}
