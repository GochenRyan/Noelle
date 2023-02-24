#include <System.h>
#include <Timer.h>
#include "ImGui/ImGuiLayer.h"
#include "GLFW/glfw3.h"

int main()
{
    std::unique_ptr<Noelle::ImGuiLayer> imGuiLayer{new Noelle::ImGuiLayer()};
    while (true)
    {
        imGuiLayer->OnAttach();
        imGuiLayer->Begin();
        imGuiLayer->End();
        imGuiLayer->OnDetach();
    }

    return 0;
}