#include "Timer.h"
#include <MMSystem.h>
using namespace Noelle;
Timer * Timer::ms_pTimer = NULL;
Timer::Timer()
{
	InitGameTime();
	ms_pTimer = this;
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
	if(QueryPerformanceFrequency((LARGE_INTEGER*) &m_int64OneSecondTicks))
	{
		m_bUseLargeTime=true;
		QueryPerformanceCounter((LARGE_INTEGER*) &m_int64TimeTickStartCounts);
	}
	else
	{
		m_bUseLargeTime = false;
		m_ulTimeStart = timeGetTime();
	}
}
int Timer::GetRandSeed()
{
	return ((LARGE_INTEGER*)&m_int64TimeTickStartCounts)->LowPart;
}
double Timer::GetGamePlayTime()
{  //·µ»ØÒÑ½øÐÐµÄÊ±¼ä,µ¥Î»ºÁÃë
	__int64 int64TimeCurrentCounts;
	if(m_bUseLargeTime)
	{
		QueryPerformanceCounter((LARGE_INTEGER*) &int64TimeCurrentCounts);
		return ((int64TimeCurrentCounts - m_int64TimeTickStartCounts)*(1.0 / m_int64OneSecondTicks)*1000.0);
	}
	else
	{
		return ((timeGetTime() - m_ulTimeStart));  //timeGetTimeº¯Êý·µ»ØµÄÊ±¼äµ¥Î»ÎªºÁÃë
	}
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














