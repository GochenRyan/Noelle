// Copyright 2022 Gaocheng Ran <rangaocheng@outlook.com>

// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at

//     http://www.apache.org/licenses/LICENSE-2.0

// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
#include <System.h>
#include <Timer.h>
#include "ImGui/ImGuiLayer.h"
#include "GLFW/glfw3.h"

using namespace NoelleEditor;

int main()
{
    uint16_t width = 1024;
    uint16_t height = 768;
    std::unique_ptr<Window> pMainWindow = std::make_unique<Window>("TesT Bench", width, height);
    std::unique_ptr<ImGuiContextInstance> pImGuiContext = std::make_unique<ImGuiContextInstance>(width, height);
    pMainWindow->OnResize.Bind<ImGuiContextInstance, &ImGuiContextInstance::OnResize>(pImGuiContext.get());
    std::vector<std::string> ttfFileNames = { "FanWunMing-SB.ttf" };
	pImGuiContext->LoadFontFiles(ttfFileNames, Language::ChineseSimplied);

    pImGuiContext->AddStaticLayer(std::make_unique<MainMenu>("MainMenu"));

    if (!Timer::m_pTimer)
	{
		Timer * pTimer = new Timer();
		if (!pTimer)
		{
			return false;
		}
	}

    std::vector<std::unique_ptr<Renderer>> pEngineRenderers;

    std::unique_ptr<RenderContext> pRenderContext = std::make_unique<RenderContext>();
    pRenderContext->Init(pMainWindow);
    pMainWindow->OnResize.Bind<RenderContext, &RenderContext::OnResize>(pRenderContext.get());

    StringCrc editorSwapChainName("EditorUISwapChain");

    // The init size doesn't make sense. It will resize by SceneView.
	RenderTarget* pSceneRenderTarget = pRenderContext->CreateRenderTarget(editorSwapChainName, width, height, pMainWindow->GetNativeHandle());

    // Note that if you don't want to use ImGuiRenderer for engine, you should also disable EngineImGuiContext.
	std::unique_ptr<Renderer> pRenderer = std::make_unique<ImGuiRenderer>(pRenderContext.get(), pRenderContext->CreateView(), pSceneRenderTarget);

    pRenderer->Init();
	pEngineRenderers.emplace_back(std::move(pRenderer));

    while (!pMainWindow->ShouldClose())
    {
        Timer::m_pTimer->UpdateFPS();

        float deltaTime = Timer::m_pTimer->GetDetTime();

        pMainWindow->Update();
        pImGuiContext->Update(deltaTime);
        pRenderContext->BeginFrame();
        for (std::unique_ptr<Renderer>& pRenderer : pEngineRenderers)
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

        pRenderContext->EndFrame();
    }


    return 0;
}