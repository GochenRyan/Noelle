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

#pragma once
#include "Graphic.h"
#include "StringCrc.h"
#include "Property.h"
#include "System.h"

#include <memory>
#include <functional>

namespace NoelleGraphic
{
    class Object;
    using CreateObjectFunc = std::function<Object*()>;

    class GRAPHIC_API ClassInfo
    {
    public:
        ClassInfo() = delete;
        ClassInfo(const StringCrc& nameCrc, ClassInfo* pBase, CreateObjectFunc COF);
        ClassInfo(const ClassInfo&) = delete;
        ClassInfo& operator=(const ClassInfo&) = delete;
        ClassInfo(ClassInfo&&) = default;
        ClassInfo& operator=(ClassInfo&&) = default;
        ~ClassInfo();

        inline StringCrc GetName() const;
        inline bool IsSameType(const ClassInfo& Type) const;
        inline bool IsDerived(const ClassInfo& Type) const;
        inline ClassInfo* GetBase() const
        {
            return m_pBase;
        }

        Property* GetProperty(unsigned int uiIndex) const;
        unsigned int GetPropertyNum() const;
        void AddProperty(std::unique_ptr<Property> property);
        void AddProperty(ClassInfo& classinfo);
        void ClearProperty();

    private:
        StringCrc m_crcName;
        ClassInfo* m_pBase;
        CreateObjectFunc m_createFunc;
        std::vector<std::unique_ptr<Property>> m_PropertyArray;
    };

    #include "ClassInfo.inl"
}
