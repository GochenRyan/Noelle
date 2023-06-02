#include "Engine.h"
#include <Timer.h>

using namespace Noelle;

Engine* Engine ::ms_pEngine = nullptr;

Engine::Engine(std::unique_ptr<IApplication> pApplication)
	: m_pApplication(std::move(pApplication))
{
}

Engine::~Engine()
{
	m_pApplication->Shutdown();
	Shutdown();
}

void Engine::Init(EngineInitArgs args)
{
	m_pApplication->Init(args);
}


void Engine::Run()
{
	if (!Timer::ms_pTimer)
	{
		Timer * pTimer = new Timer();
		if (!pTimer)
		{
			return;
		}
	}

	while (true)
	{
		Timer::ms_pTimer->UpdateFPS();

		if (!m_pApplication->Update(Timer::ms_pTimer->GetDetTime()))
		{
			break;
		}
	}
}

void Engine::Shutdown()
{

}

Engine* Engine::Create(std::unique_ptr<IApplication> pApplication)
{
	IApplication* pWhatApp = pApplication.get();
	ms_pEngine = new Engine(std::move(pApplication));
	pWhatApp->SetEngine(ms_pEngine);
	return ms_pEngine;
}

void Engine::Destroy(Engine* pEngine)
{
	assert(ms_pEngine == pEngine);
	if (ms_pEngine)
	{
		ms_pEngine->Shutdown();
		ms_pEngine = nullptr;
	}
}