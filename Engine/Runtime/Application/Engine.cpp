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