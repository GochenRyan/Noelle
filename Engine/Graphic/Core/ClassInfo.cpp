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

#include "ClassInfo.h"

using namespace NoelleGraphic;

ClassInfo::ClassInfo(const StringCrc& nameCrc, ClassInfo* pBase, CreateObjectFunc COF)
{
    m_uiName = nameCrc.Value();
    m_pBase = pBase;
    m_createFunc = COF;
}

ClassInfo::~ClassInfo()
{
    m_pBase = nullptr;
}

Property* ClassInfo::GetProperty(unsigned int uiIndex) const
{
    return m_PropertyArray[uiIndex].get();
}

unsigned int ClassInfo::GetPropertyNum() const
{
    return m_PropertyArray.size();
}

void ClassInfo::AddProperty(std::unique_ptr<Property> property)
{
    m_PropertyArray.emplace_back(std::move(property));
}

void ClassInfo::AddProperty(ClassInfo& classinfo)
{
    for (const auto& property : classinfo.m_PropertyArray) 
    {
        Property* newProperty = property->GetInstance();
        newProperty->Clone(property.get());
        m_PropertyArray.emplace_back(std::make_unique<Property>(std::move(*newProperty)));
        newProperty = nullptr;
    }
}