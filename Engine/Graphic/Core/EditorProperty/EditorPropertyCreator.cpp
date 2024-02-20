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

#include "EditorPropertyCreator.h"

using namespace NoelleGraphic;

void EditorPropertyCreator::Set(CreateEditorPropertyFunc func)
{
    m_Creator = func;
}

std::unique_ptr<EditorControl> EditorPropertyCreator::CreateUIProperty(EditorControl::CONTROL_TYPE type, const std::string& name)
{
    if (m_Creator != nullptr)
        return m_Creator(type, name);
    return nullptr;
}