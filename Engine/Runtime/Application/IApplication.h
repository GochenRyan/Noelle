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

#include "Runtime.h"
#include "Command.h"
#include "Engine.h"
#include "ImGui/Localization.h"

namespace Noelle
{
    class Engine;

    struct RUNTIME_API EngineInitArgs
    {
        const char* pTitle = "Noelle";
        const char* pIconFilePath = nullptr;
        uint16_t width = 800;
        uint16_t height = 600;

        Language language = Language::English;
    };

    class RUNTIME_API IApplication
    {
    public:
        IApplication() = default;
        IApplication(const IApplication&) = delete;
        IApplication(IApplication&&) = default;
        IApplication& operator=(const IApplication&) = delete;
        IApplication& operator=(IApplication&&) = default; 
        virtual ~IApplication(){}

        virtual void Init(EngineInitArgs initArgs) = 0;
        virtual bool Update(float deltaTime) = 0;
        virtual void Shutdown() = 0;

        void SetEngine(Engine* engine) { m_pEngine = engine;}
        Engine* GetEngine() { return m_pEngine;}
    protected:
        Engine* m_pEngine;
        Command* m_pCommand;
    };
}
