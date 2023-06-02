#pragma once

#include "Application/IApplication.h"
#include "Window.h"
#include "ImGuiContextInstance.h"
#include "ImGuiRenderer.h"
#include "RenderContext.h"

#include <memory>

namespace NoelleEditor
{
    class Application final: public Noelle::IApplication
    {
    public:
        Application();
        Application(const Application&) = delete;
        Application(Application&&) = default;
        Application& operator=(const Application&) = delete;
        Application& operator=(Application&&) = default; 
        virtual ~Application();

        virtual void Init(Noelle::EngineInitArgs initArgs) override;
        virtual bool Update(float deltaTime) override;
        virtual void Shutdown() override;

        void AddWindow(std::unique_ptr<Window> pWindow);
        void InitRenderContext();
        void AddEditorRenderer(std::unique_ptr<Renderer> pRenderer);

        void InitEditorImGuiContext(Noelle::Language language);
        void RegisterImGuiUserData(ImGuiContextInstance* pImGuiContext);

        Window* GetMainWindow() const;
    private:
        std::vector<std::unique_ptr<Window>> m_pAllWindows;
        std::unique_ptr<ImGuiContextInstance> m_pImGuiContext;
        std::vector<std::unique_ptr<Renderer>> m_pEditorRenderers;
        std::unique_ptr<RenderContext> m_pRenderContext;
    };
}
