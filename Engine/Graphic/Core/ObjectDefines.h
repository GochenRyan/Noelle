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

#include "functional"

namespace NoelleGraphic
{
    class GRAPHIC_API TypeRegister
    {
    public:
        TypeRegister(uint32_t uiTypeID, ClassInfo* pType);
        ~TypeRegister();
        static void Register();
    private:
        void VerifyAndRegisterType(uint32_t uiTypeID, ClassInfo* pType);
        static TypeRegister* s_HeadRegister;
        TypeRegister* m_Prev;
        TypeRegister* m_Next;
        std::function<void()> m_RegisteFunction;
    };

    template<uint32_t TypeID>
    struct ObjectRegisteredHelper
    {
        static TypeRegister s_Init;
    };

    #define IMPLEMENT_REGISTER_TYPE_ID(TypeID, ClassName) \
    template<> NoelleGraphic::TypeRegister NoelleGraphic::ObjectRegisteredHelper<TypeID>::s_Init(TypeID, &ClassName::ms_Type);
}