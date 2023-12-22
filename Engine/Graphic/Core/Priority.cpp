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

#include "Priority.h"

using namespace NoelleGraphic;

Priority::Priority()
{
    m_uiPriorityNum = 0;
}

Priority::Priority(unsigned int uiPriorityNum)
{
    m_uiPriorityNum = uiPriorityNum;
}

Priority::~Priority()
{
    m_DependencyPriorityList.clear();
}

bool Priority::AddDependencyPriorities(std::initializer_list<Priority*> args)
{
    bool allSuccess = true;
    for (auto arg : args)
    {
        allSuccess &= AddDependencyPriority(arg);
    }
    return allSuccess;
}

bool Priority::AddDependencyPriority(Priority* pOther)
{
    if (!pOther)
        return false;

    if (CheckIsCircle(pOther))
        return false;

    m_uiPriorityNum = 0;  // Set it to 0 to be able to update later
    m_DependencyPriorityList.push_back(pOther);
    return true;
}

bool Priority::operator < (Priority& other)
{
    other.CheckPriorityNum();
    CheckPriorityNum();
    return m_uiPriorityNum < other.m_uiPriorityNum;
}

bool Priority::operator <= (Priority& other)
{
    other.CheckPriorityNum();
    CheckPriorityNum();
    return m_uiPriorityNum <= other.m_uiPriorityNum;
}

bool Priority::operator > (Priority& other)
{
    other.CheckPriorityNum();
    CheckPriorityNum();
    return m_uiPriorityNum > other.m_uiPriorityNum;
}

bool Priority::operator >= (Priority& other)
{
    other.CheckPriorityNum();
    CheckPriorityNum();
    return m_uiPriorityNum >= other.m_uiPriorityNum;
}

bool Priority::operator == (Priority& other)
{
    other.CheckPriorityNum();
    CheckPriorityNum();
    return m_uiPriorityNum == other.m_uiPriorityNum;
}

bool Priority::operator != (Priority& other)
{
    other.CheckPriorityNum();
    CheckPriorityNum();
    return m_uiPriorityNum != other.m_uiPriorityNum;
}

bool Priority::CheckIsCircle(const Priority* pCheck) const
{
    if (!pCheck)
        return false;

    for (auto priority : m_DependencyPriorityList)
    {
        if (pCheck == priority)
            return true;
        else if (priority->CheckIsCircle(pCheck))
            return true;
    }
    return false;
}

unsigned int Priority::CheckPriorityNum()
{
    if (m_uiPriorityNum != 0 || m_DependencyPriorityList.empty())
        return m_uiPriorityNum;
    
    unsigned int uiMax = 0;
    for (auto priority : m_DependencyPriorityList)
    {
        if (uiMax < priority->CheckPriorityNum())
        {
            uiMax = priority->m_uiPriorityNum;
        }
    }

    m_uiPriorityNum = uiMax + 1;

    return m_uiPriorityNum;
}