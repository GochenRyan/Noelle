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
#include "Graphic.h"
#include "EditorProperty.h"

#include <functional>
#include <memory>

namespace NoelleGraphic
{
    using CreateEditorPropertyFunc = std::function<std::unique_ptr<EditorControl>(EditorControl::CONTROL_TYPE, std::string&)>;

    class GRAPHIC_API EditorPropertyCreator
    {
    private:
        EditorPropertyCreator() = default;
        ~EditorPropertyCreator() = default;
    public:
        EditorPropertyCreator(const EditorPropertyCreator&) = delete;
        EditorPropertyCreator& operator=(const EditorPropertyCreator&) = delete;
        EditorPropertyCreator(EditorPropertyCreator&&) = delete;
        EditorPropertyCreator& operator=(EditorPropertyCreator&&) = delete;

        void Set(CreateEditorPropertyFunc func);
        std::unique_ptr<EditorControl> CreateUIProperty(EditorControl::CONTROL_TYPE type, std::string& name);

        auto& GetInstance()
        {
            static EditorPropertyCreator ms_Instance;
            return ms_Instance;
        }
    private:
        CreateEditorPropertyFunc m_Creator = nullptr;
    };
}
