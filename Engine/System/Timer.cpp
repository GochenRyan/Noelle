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

#include "Timer.h"
using namespace Noelle;
Timer * Timer::m_pTimer = nullptr;
Timer::Timer()
{
	InitGameTime();
	m_pTimer = this;
}

Timer::~Timer()
{

}

void Timer::InitGameTime()
{
	m_iFrameCount = 0;
	m_fFPS = 0;
	m_fTime = 0;
	m_fLastFPSTime = 0;
	m_fTimeSlice = 0;
	m_fLastTime = 0;
	m_fDetTime = 0;
	m_ulTimeStart = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

int Timer::GetRandSeed()
{
	return ((LARGE_INTEGER*)&m_ulTimeStart)->LowPart;
}

double Timer::GetGamePlayTime()
{
	return (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() - m_ulTimeStart);
}

void Timer::UpdateFPS()
{

	m_fTime = GetGamePlayTime() * 0.001;
	m_fDetTime = m_fTime - m_fLastTime;
	m_fLastTime = m_fTime;
	if (m_fTime - m_fLastFPSTime > 1.0f)
	{
		m_fLastFPSTime = m_fTime;
		m_fFPS = m_iFrameCount;
		m_iFrameCount = 0;
	}
	else
	{
		m_iFrameCount++;
	}
}














