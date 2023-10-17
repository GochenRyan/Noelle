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

#include "Context.h"

#include <algorithm>

using namespace NoelleGraphic;

std::vector<std::unique_ptr<Function>> Context::m_TerminalFunctionArray;
std::vector<std::unique_ptr<FunctionProperty>> Context::m_InitialFunctionPropertyArray;
std::vector<std::unique_ptr<Context::FuncElement>> Context::m_InitialFuncElementArray;
std::vector<std::unique_ptr<Context::FuncElement>> Context::m_TerminalFuncElementArray;

void Context::AddInitialPropertyFunction(FunctionProperty func)
{
    if (!func)
        return;
    m_InitialFunctionPropertyArray.push_back(std::make_unique<FunctionProperty>(func));
}

void Context::AddTerminalPropertyFunction(Function func)
{
    if (!func)
        return;
    m_TerminalFunctionArray.push_back(std::make_unique<Function>(func));
}

void Context::AddInitialFunction(Function func)
{
    if (!func)
        return;
    m_InitialFuncElementArray.push_back(std::make_unique<FuncElement>(func, nullptr));
}

void Context::AddInitialFunction(Function func, Priority* p)
{
    if (!func)
        return;
    m_InitialFuncElementArray.push_back(std::make_unique<FuncElement>(func, p));
}

void Context::AddTerminalFunction(Function func)
{
    if (!func)
        return;
    m_TerminalFuncElementArray.push_back(std::make_unique<FuncElement>(func, nullptr));
}

void Context::AddTerminalFunction(Function func, Priority* p)
{
    if (!func)
        return;
    m_TerminalFuncElementArray.push_back(std::make_unique<FuncElement>(func, p));
}

bool Context::Initialize()
{
    for (std::unique_ptr<FunctionProperty>& func : m_InitialFunctionPropertyArray)
    {
        if (!(*func)(nullptr))
        {
            assert(false);
            return false;
        }
    }

    std::sort(m_InitialFuncElementArray.begin(), m_InitialFuncElementArray.end(),
        [](std::unique_ptr<Context::FuncElement>& l, std::unique_ptr<Context::FuncElement>& r)
        {
            return *l < *r;
        });
    
    for (std::unique_ptr<Context::FuncElement>& func : m_InitialFuncElementArray)
    {
        if (!(*func).Func())
        {
            assert(false);
            return false;
        }
    }

    return true;
}

bool Context::Terminal()
{
    std::sort(m_TerminalFuncElementArray.begin(), m_TerminalFuncElementArray.end(),
        [](std::unique_ptr<Context::FuncElement>& l, std::unique_ptr<Context::FuncElement>& r)
        {
            return *l > *r;
        });

    for (std::unique_ptr<Context::FuncElement>& func : m_TerminalFuncElementArray)
    {
        if (!(*func).Func())
        {
            assert(false);
            return false;
        }
    }

    for (std::unique_ptr<Function>& func : m_TerminalFunctionArray)
    {
        if (!(*func)())
        {
            assert(false);
            return false;
        }
    }

    return true;
}