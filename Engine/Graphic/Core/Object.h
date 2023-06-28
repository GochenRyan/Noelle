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
#include "ClassInfo.h"
#include "ClassInfo.marc"
#include "FastObjManager.h"

#include <unordered_map>
#include <functional>

namespace NoelleGraphic
{
    class Object;
    using CreateObjectFunc = std::function<Object*()>;

    class GRAPHIC_API Object
    {
    public:
        Object();
        virtual ~Object() = 0;
        DECLARE_CLASSINFO
        inline bool IsSameType(const ClassInfo& Type) const;
        inline bool IsDerived(const ClassInfo& Type) const;
        inline bool IsSameType(const Object* pObject) const;
        inline bool IsDerived(const Object* pObject) const;
        static bool ms_bRegisterFactory;
        static std::unordered_map<uint32_t, CreateObjectFunc> ms_classFactory;
        
        friend class FastObjManager;
        static FastObjManager& GetFastObjManager()
        {
            static FastObjManager ms_objManager;
            return ms_objManager;
        }
    };

    #include "Object.inl"
}