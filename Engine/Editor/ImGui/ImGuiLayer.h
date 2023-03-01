#pragma once
#include "../Layer.h"
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

// TEMPORARY
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define IMGUI_IMPL_OPENGL_LOADER_GLAD

namespace Noelle {
    class ImGuiLayer : public Layer
    {
    public:
        ImGuiLayer();
        ~ImGuiLayer();
        int CreateglfwWindow();

        virtual void OnAttach() override;
        virtual void OnDetach() override;
        virtual void OnImGuiRender() override;

        void Begin();
        void End();

        void BlockEvents(bool block) { m_BlockEvents = block; }

        void SetDarkThemeColors();

        uint32_t GetActiveWidgetID() const;
    private:
        bool m_BlockEvents = true;
        GLFWwindow* m_window;
    };
}