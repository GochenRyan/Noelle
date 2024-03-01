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
#include "FastObjManager.h"
#include "Priority.h"
#include "Type.marc"

#include "ClassInfo.marc"
#include "Initial.marc"

#include <unordered_map>
#include <functional>
#include <vector>

namespace NoelleGraphic
{
    class Object;
    using CreateObjectFunc = std::function<Object*()>;
    class VSStream;
    class ClassInfo;

    class GRAPHIC_API Object
    {
    public:
        friend class VSStream;

        Object();
        virtual ~Object() = 0;
        DECLARE_CLASSINFO
        
        inline bool IsSameType(const ClassInfo& Type) const;
        inline bool IsDerived(const ClassInfo& Type) const;
        inline bool IsSameType(const Object* pObject) const;
        inline bool IsDerived(const Object* pObject) const;

    public:
        static Object* GetInstance(const ClassInfo& Type);

        static bool ms_bRegisterFactory;
        static std::unordered_map<uint32_t, CreateObjectFunc> ms_ClassFactory;
        
        DECLARE_INITIAL_NO_CLASS_FACTORY
        
        friend class FastObjManager;
        static FastObjManager& GetFastObjManager()
        {
            static FastObjManager ms_objManager;
            return ms_objManager;
        }
        
        enum //Object Flag
        {
            OF_REACH = 0x01,
            OF_UNREACH = 0x02,
            OF_PendingKill = 0x04,
            OF_GCObject = 0x08,
            OF_RootObject = 0x10,
            OF_MAX
        };
        inline void SetFlag(unsigned int uiFlag);
        inline void ClearFlag(unsigned int uiFlag);
        inline bool IsHasFlag(unsigned int uiFlag);
    
        void ValueChanged(std::string name, void* pData);
    private:
        unsigned int m_uiFlag;
    };

    TYPE_MARCO(Object)

    #include "Object.inl"
}