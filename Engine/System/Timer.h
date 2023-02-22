#pragma once
#include "System.h"
namespace VSEngine2
{
	class VSSYSTEM_API VSTimer
	{
	private:
		bool m_bUseLargeTime;
		__int64 m_int64OneSecondTicks;
		__int64 m_int64TimeTickStartCounts;
		unsigned long m_ulTimeStart;
		int m_iFrameCount;
		double m_fFPS;
		double m_fTime,m_fLastFPSTime,m_fTimeSlice;
		double m_fDetTime, m_fLastTime;
		void InitGameTime();
	public:
		VSTimer();
		~VSTimer();
	
		double GetGamePlayTime();
		void UpdateFPS();
		inline double GetFPS(){return m_fFPS;}
		static VSTimer * ms_pTimer;
		double GetDetTime(){ return m_fDetTime; }
		int GetRandSeed();
	};
}
