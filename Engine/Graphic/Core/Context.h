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
#include "Priority.h"
#include "ClassInfo.h"

#include <memory>
#include <functional>
#include <vector>

namespace NoelleGraphic
{
    using Function = std::function<bool()>;
    using FunctionProperty = std::function<bool(ClassInfo*)>;

    class GRAPHIC_API Context
    {
    public:
        static void AddInitialPropertyFunction(FunctionProperty func);
        static void AddTerminalPropertyFunction(Function func);
        static void AddInitialFunction(Function func);
        static void AddInitialFunction(Function func, Priority* p);
        static void AddTerminalFunction(Function func);
        static void AddTerminalFunction(Function func, Priority* p);

        static bool Initialize();
        static bool Terminal();

        class FuncElement
        {
        public:
            Function Func;
            Priority* pPriority;

            FuncElement(Function func, Priority* p)
            {
                Func = func;
                pPriority = p;
            }

            bool operator == (const FuncElement other) const
            {
                static Priority lPriority;
                static Priority rPriority;

                Priority* p1 = nullptr;
                Priority* p2 = nullptr;

                if (pPriority)
                    p1 = pPriority;
                else
                    p1 = &lPriority;
                
                if (other.pPriority)
                    p2 = other.pPriority;
                else
                    p2 = &rPriority;
                
                return *p1 == *p2;
            }

            bool operator < (const FuncElement other) const
            {
                static Priority lPriority;
                static Priority rPriority;

                Priority* p1 = nullptr;
                Priority* p2 = nullptr;

                if (pPriority)
                    p1 = pPriority;
                else
                    p1 = &lPriority;
                
                if (other.pPriority)
                    p2 = other.pPriority;
                else
                    p2 = &rPriority;
                
                return *p1 < *p2;
            }

            bool operator > (const FuncElement other) const
            {
                static Priority lPriority;
                static Priority rPriority;

                Priority* p1 = nullptr;
                Priority* p2 = nullptr;

                if (pPriority)
                    p1 = pPriority;
                else
                    p1 = &lPriority;
                
                if (other.pPriority)
                    p2 = other.pPriority;
                else
                    p2 = &rPriority;
                
                return *p1 > *p2;
            }
        };

    private:
        static std::vector<std::unique_ptr<Function>> m_TerminalFunctionArray;
        static std::vector<std::unique_ptr<FunctionProperty>> m_InitialFunctionPropertyArray;
        static std::vector<std::unique_ptr<FuncElement>> m_InitialFuncElementArray;
        static std::vector<std::unique_ptr<FuncElement>> m_TerminalFuncElementArray;
    };
}