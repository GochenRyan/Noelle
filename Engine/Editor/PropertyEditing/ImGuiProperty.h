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
#include "Core/EditorProperty/EditorProperty.h"
#include "NVector.h"

#include <imgui.h>
#include <string>
#include <misc/cpp/imgui_stdlib.h>

namespace NoelleGraphic
{
    class IImGuiUpdatable
    {
    public:
        IImGuiUpdatable() = default;
        virtual void Update() = 0;
        virtual ~IImGuiUpdatable() = default;
    };

    class ImGuiLable: public ELabel, public IImGuiUpdatable
    {
    public:
        ImGuiLable(std::string& name): ELabel(name)
        {
        }
        virtual ~ImGuiLable() = default;
        void Update() override
        {
            ImGui::Text(m_Content.data());
        }
    };

    class ImGuiCheckBox: public ECheckBox, public IImGuiUpdatable
    {
    public:
        ImGuiCheckBox(std::string& name): ECheckBox(name)
        {
        }
        virtual ~ImGuiCheckBox() = default;
        void Update() override
        {
            if (ImGui::Checkbox(GetName().data(), &m_Content))
            {
                CallBackValue(m_Content);
            }
        }
    };

    class GRAPHIC_API ImGuiSlider: public ESlider, public IImGuiUpdatable
    {
    public:
        ImGuiSlider(std::string& name, uint32_t uiMin, uint32_t uiMax, uint32_t uiStep): ESlider(name, uiMin, uiMax, uiStep)
        {
        }

        virtual ~ImGuiSlider() = default;

        void Update() override
        {
            if (ImGui::SliderInt(GetName().data(), &m_iContent, m_uiMin, m_uiMax))
            {
                m_uiContent = m_iContent;
                CallBackValue(m_uiContent);
            }
        }
    protected:
        int m_iContent;
    };

    class ImGuiViewWindow: public EViewWindow, public IImGuiUpdatable
    {
        void Update() override
        {
        }
    };

    class ImGuiCombo: public ECombo, public IImGuiUpdatable
    {
    public:
        ImGuiCombo(std::string& name): ECombo(name)
        {

        }

        virtual ~ImGuiCombo() = default;

        void Update() override
        {
            if (ImGui::Combo(GetName().data(), &m_uiSelection, m_Options.data(), m_Options.size()))
            {
                std::string cur = m_Options[m_uiSelection];
                CallBackValue(cur);
            }
        }
        void AddOption(std::string option);
        void AddOptions(std::vector<std::string> options);
        virtual void CallBackValue(std::string& value);
    private:
        int m_uiSelection = 0;
        std::vector<const char*> m_Options;
    };

    class ImGuiColorTable: public EColorTable, public IImGuiUpdatable
    {
    public:
        ImGuiColorTable(std::string& name): EColorTable(name)
        {
            m_Color = NoelleMath::NVector<float, 4>(0.0f, 0.0f, 0.0f, 0.0f); 
        }

        virtual ~ImGuiColorTable() = default;

        void Update() override
        {
            if (ImGui::ColorEdit4(GetName().data(), &m_Color[0]))
            {
                CallBackValue(m_Color);
            }
        }
    private:
        NoelleMath::NVector<float, 4> m_Color;
    };

    class ImGuiTextBox: public ETextBox, public IImGuiUpdatable
    {
    public:
        ImGuiTextBox(std::string& name): ETextBox(name)
        {
        }

        virtual ~ImGuiTextBox() = default;

        void Update() override
        {
            if (ImGui::InputText(GetName().data(), &m_Content))
            {
                CallBackValue(m_Content);
            }
        }
    };

}