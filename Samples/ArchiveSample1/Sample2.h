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
#include "Core/Object.h" 
#include "Core/Context.h"
#include "Core/ClassInfo.marc"
#include "Core/Initial.marc"
#include "Core/Property.marc"
#include "Core/Priority.marc"
#include "Core/ClassInfo.h"
namespace NoelleGraphic
{
    class Sample2: public NoelleGraphic::Object
    {
    public:
        Sample2(){}
        virtual ~Sample2(){}
        DECLARE_CLASSINFO
        DECLARE_INITIAL

        static bool TestInitFunction();
        static bool TestTerminalFunction();
    };
    TYPE_MARCO(Sample2);
}