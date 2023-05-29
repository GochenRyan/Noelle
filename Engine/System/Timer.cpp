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
Timer* Timer::m_pTimer = nullptr;

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
	m_fLastFPSTime = {};
	m_fTimeSlice = 0;
	m_tpTimeStart = std::chrono::steady_clock::now();
	m_ulTimeStart = std::chrono::duration_cast<std::chrono::milliseconds>(m_tpTimeStart.time_since_epoch()).count();
	m_fLastTime = {};
	m_fTime = {};
	m_fDetTime = 0;
	
}

int Timer::GetRandSeed()
{
	return static_cast<unsigned long>(m_ulTimeStart & 0xFFFFFFFF);
}

std::chrono::duration<float> Timer::GetGamePlayTime()
{
	return std::chrono::steady_clock::now() - m_tpTimeStart;
}

void Timer::UpdateFPS()
{
	m_fTime = GetGamePlayTime();
	m_fDetTime = (m_fTime - m_fLastTime).count();
	m_fLastTime = m_fTime;
	if ((m_fTime - m_fLastFPSTime).count() > 1.0f)
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
