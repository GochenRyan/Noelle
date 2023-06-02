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
