// Copyright 2024 Gaocheng Ran <rangaocheng@outlook.com>

// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at

//     http://www.apache.org/licenses/LICENSE-2.0

// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
#include "ImGui/ThemeColor.h"
#include "MainMenu.h"
#include "ImGuiContextInstance.h"
#include "Input.h"
#include "KeyCode.h"
#include "Window.h"

#include <imgui.h>

#include <filesystem>

using namespace NoelleEditor;

MainMenu::~MainMenu()
{
}

void MainMenu::FileMenu()
{
	if (ImGui::BeginMenu("File"))
	{
		if (ImGui::MenuItem("New", "Ctrl N"))
		{
		}
		if (ImGui::MenuItem("Open", "Ctrl O"))
		{
		}
		if (ImGui::MenuItem("Open Recent"))
		{
		}

		ImGui::Separator();

		if (ImGui::MenuItem("Save", "Ctrl S"))
		{
		}
		if (ImGui::MenuItem("Save As", "Shift Ctrl S"))
		{
		}

		ImGui::Separator();

		if (ImGui::MenuItem("Quit", "Ctrl Q"))
		{
			if (auto* pMainWindow = reinterpret_cast<Window*>(ImGui::GetIO().BackendPlatformUserData))
			{
				pMainWindow->Closed();
			}
		}

		ImGui::EndMenu();
	}
}

void MainMenu::EditMenu()
{
	ImGuiIO& io = ImGui::GetIO();

	if (ImGui::BeginMenu("Edit"))
	{
		if (ImGui::MenuItem("Undo", "Ctrl Z"))
		{
		}
		if (ImGui::MenuItem("Redo", "Shift Ctrl Z"))
		{
		}

		ImGui::Separator();

		if (ImGui::MenuItem("Preference"))
		{
		}

		if (ImGui::BeginMenu("Style"))
		{
			// It is not convenient in C++ to loop enum except define an extra array to wrap them.
			// C++ 20/23 ranges may look better but still needs std::iota inside its implementation.
			for (Noelle::ThemeColor theme = Noelle::ThemeColor::Black; theme < Noelle::ThemeColor::Count;
				theme = static_cast<Noelle::ThemeColor>(static_cast<int>(theme) + 1))
			{
				ImGuiContextInstance* pImGuiContextInstance = reinterpret_cast<ImGuiContextInstance*>(io.UserData);
				if (ImGui::MenuItem(GetThemeColorName(theme), "", pImGuiContextInstance->GetImGuiThemeColor() == theme))
				{
					pImGuiContextInstance->SetImGuiThemeColor(theme);
				}
			}

			ImGui::EndMenu();
		}

		ImGui::EndMenu();
	}
}

void MainMenu::WindowMenu()
{
	ImGuiIO& io = ImGui::GetIO();

	if (ImGui::BeginMenu("Window"))
	{
		ImGuiContextInstance* pCurrentImguiContextInstance = reinterpret_cast<ImGuiContextInstance*>(io.UserData);
		for (const auto& pDockableLayer : pCurrentImguiContextInstance->GetDockableLayers())
		{
			if (ImGui::MenuItem(pDockableLayer->GetName(), "", pDockableLayer->IsEnable()))
			{
				pDockableLayer->SetEnable(!pDockableLayer->IsEnable());
			}
		}

		ImGui::EndMenu();
	}
}

void MainMenu::BuildMenu()
{
	if (ImGui::BeginMenu("Build"))
	{
		if (ImGui::MenuItem("Rebuild Shaders"))
		{
		}

		ImGui::EndMenu();
	}
}

void MainMenu::AboutMenu()
{
	if (ImGui::BeginMenu("About"))
	{
		if (ImGui::MenuItem("Documents"))
		{
		}

		ImGui::EndMenu();
	}
}

void MainMenu::Init()
{

}

void MainMenu::Update()
{
	if (ImGui::BeginMainMenuBar())
	{
		FileMenu();
		EditMenu();
		WindowMenu();
		BuildMenu();
		AboutMenu();
		ImGui::EndMainMenuBar();
	}

	if (Input::Get().ContainsModifier(KeyMod::KMOD_CTRL) 
		&& Input::Get().IsKeyPressed(KeyCode::q))
	{
		if (auto* pMainWindow = reinterpret_cast<Window*>(ImGui::GetIO().BackendPlatformUserData))
		{ 
			pMainWindow->Closed();
		}
	}
}