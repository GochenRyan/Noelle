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

int main()
{
    std::unique_ptr<Noelle::ImGuiLayer> imGuiLayer{new Noelle::ImGuiLayer()};
    if (imGuiLayer->CreateglfwWindow() == -1)
        return -1;
    
    imGuiLayer->OnAttach();
    while (true)
    {
        imGuiLayer->Begin();
        imGuiLayer->OnImGuiRender();
        imGuiLayer->End();
    }

    imGuiLayer->OnDetach();

    return 0;
}