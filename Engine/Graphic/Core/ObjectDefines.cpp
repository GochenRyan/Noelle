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

#include "ObjectDefines.h"
#include "System.h"
#include "Context.h"

using namespace NoelleGraphic;

TypeRegister* TypeRegister::s_HeadRegister = nullptr;

TypeRegister::TypeRegister(uint32_t uiTypeID, ClassInfo* pType)
{
    if (s_HeadRegister != nullptr)
    {
        s_HeadRegister->m_Prev = this;
    }
    m_Next = s_HeadRegister;
    m_Prev = nullptr;
    s_HeadRegister = this; 

    m_RegisteFunction = std::bind(&TypeRegister::VerifyAndRegisterType, this, uiTypeID, pType);
}

TypeRegister::~TypeRegister()
{
    if (m_Prev)
        m_Prev->m_Next = m_Next;
    else
        s_HeadRegister = m_Next;

    if (m_Next)
        m_Next->m_Prev = m_Prev;
}

void TypeRegister::VerifyAndRegisterType(uint32_t uiTypeID, ClassInfo* pType)
{
    NOEL_ASSERTM(!Context::m_TypeIDMap.contains(uiTypeID), "Type ID conflict!");

    if (!Context::m_TypeIDMap.contains(uiTypeID))
    {
        Context::m_TypeIDMap[uiTypeID] = pType;
    }
}

void TypeRegister::Register()
{
    TypeRegister* pCurrent = s_HeadRegister;
    while (pCurrent)
    {
        pCurrent->m_RegisteFunction();
        pCurrent = pCurrent->m_Next;
    }
    
}