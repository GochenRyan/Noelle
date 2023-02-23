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
