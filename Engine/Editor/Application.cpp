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
#include "Application.h"
#include "Input.h"
#include "MainMenu.h"

#include <imgui.h>

using namespace NoelleEditor;

Application::Application()
{
}

Application::~Application()
{
}

void Application::Init(Noelle::EngineInitArgs initArgs)
{
	uint16_t width = initArgs.width;
	uint16_t height = initArgs.height;
	AddWindow(std::make_unique<Window>(initArgs.pTitle, width, height));
	GetMainWindow()->SetWindowIcon(initArgs.pIconFilePath);

	InitRenderContext();

	// Init ImGuiContext in the editor side which used to draw editor ui.
	InitEditorImGuiContext(initArgs.language);

}

bool Application::Update(float deltaTime)
{
    GetMainWindow()->Update();
    m_pImGuiContext->Update(deltaTime);
    m_pRenderContext->BeginFrame();
    for (std::unique_ptr<Renderer>& pRenderer : m_pEditorRenderers)
    {
        if (!pRenderer->IsEnable())
        {
            continue;
        }

        const float* pViewMatrix = nullptr;
        const float* pProjectionMatrix = nullptr;

        pRenderer->UpdateView(pViewMatrix, pProjectionMatrix);
        pRenderer->Render(deltaTime);
    }

    m_pRenderContext->EndFrame();
    Input::Get().FlushInputs();
    return !GetMainWindow()->ShouldClose();
}

void Application::Application::Shutdown()
{

}

void Application::AddWindow(std::unique_ptr<Window> pWindow)
{
    m_pAllWindows.emplace_back(std::move(pWindow));
}

void Application::InitRenderContext()
{
    Window* pMainWindow = GetMainWindow();

    m_pRenderContext = std::make_unique<RenderContext>();
    m_pRenderContext->Init();
    pMainWindow->OnResize.Bind<RenderContext, &RenderContext::OnResize>(m_pRenderContext.get());

    StringCrc editorSwapChainName("EditorUISwapChain");

    // The init size doesn't make sense. It will resize by SceneView.
	RenderTarget* pSceneRenderTarget = m_pRenderContext->CreateRenderTarget(editorSwapChainName, pMainWindow->GetWidth(), pMainWindow->GetHeight(), pMainWindow->GetNativeHandle());

    // Note that if you don't want to use ImGuiRenderer for engine, you should also disable EngineImGuiContext.
	std::unique_ptr<Renderer> pRenderer = std::make_unique<ImGuiRenderer>(m_pRenderContext.get(), m_pRenderContext->CreateView(), pSceneRenderTarget);

    AddEditorRenderer(std::move(pRenderer));
}

void Application::AddEditorRenderer(std::unique_ptr<Renderer> pRenderer)
{
    pRenderer->Init();
    m_pEditorRenderers.emplace_back(std::move(pRenderer));
}

void Application::InitEditorImGuiContext(Noelle::Language language)
{
    Window* pMainWindow = GetMainWindow();
    uint16_t width = pMainWindow->GetWidth();
    uint16_t height = pMainWindow->GetHeight();

    m_pImGuiContext = std::make_unique<ImGuiContextInstance>(width, height, true);

    RegisterImGuiUserData(m_pImGuiContext.get());

    pMainWindow->OnResize.Bind<ImGuiContextInstance, &ImGuiContextInstance::OnResize>(m_pImGuiContext.get());
    std::vector<std::string> ttfFileNames = { "FanWunMing-SB.ttf" };
	m_pImGuiContext->LoadFontFiles(ttfFileNames, Noelle::Language::ChineseSimplied);
    m_pImGuiContext->AddStaticLayer(std::make_unique<MainMenu>("MainMenu"));
}

void Application::RegisterImGuiUserData(ImGuiContextInstance* pImGuiContext)
{
    assert(GetMainWindow() && m_pRenderContext);

	ImGuiIO& io = ImGui::GetIO();
	assert(io.UserData == pImGuiContext);

	io.BackendPlatformUserData = GetMainWindow();
	io.BackendRendererUserData = m_pRenderContext.get();
}

Window* Application::GetMainWindow() const
{
    return m_pAllWindows[0].get();
}