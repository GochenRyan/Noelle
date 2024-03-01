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

#include <list>

namespace NoelleGraphic
{
    class GRAPHIC_API Priority
    {
    public:
        Priority();
        Priority(unsigned int uiPriorityNum);
        ~Priority();

        bool AddDependencyPriorities(std::initializer_list<Priority*> args);
        bool AddDependencyPriority(Priority* pOther);

        bool operator < (Priority& other);
        bool operator <= (Priority& other);
        bool operator > (Priority& other);
        bool operator >= (Priority& other);
        bool operator == (Priority& other);
        bool operator != (Priority& other);
    private:
        bool CheckIsCircle(const Priority* pCheck) const;
        unsigned int CheckPriorityNum();
        unsigned int m_uiPriorityNum;
        std::list<Priority*> m_DependencyPriorityList;
    };
}