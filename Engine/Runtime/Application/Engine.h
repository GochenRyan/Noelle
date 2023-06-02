#pragma once

#include "Runtime.h"
#include "IApplication.h"

#include <memory>

namespace Noelle
{
    struct EngineInitArgs;
    class IApplication;

    class RUNTIME_API Engine
    {
    public:
        Engine() = delete;
        explicit Engine(std::unique_ptr<IApplication>);
        Engine(Engine&) = delete;
        Engine(Engine&&) = delete;
        Engine& operator=(Engine&) = delete;
        Engine& operator=(Engine&&) = delete;
        virtual ~Engine();

        static Engine* Create(std::unique_ptr<IApplication>);
        static void Destroy(Engine*);

        void Init(EngineInitArgs args);
        void Run();
        void Shutdown();

        static Engine* ms_pEngine;
    private:
        std::unique_ptr<IApplication> m_pApplication;
    };
}
