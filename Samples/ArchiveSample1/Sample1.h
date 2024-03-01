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
#include "Core/Object.h" 
#include "Core/Context.h"
#include "Core/ClassInfo.marc"
#include "Core/Initial.marc"
#include "Core/Property.marc"
#include "Core/Priority.marc"
#include "Core/ClassInfo.h"
#include "Sample2.h"

namespace NoelleGraphic
{
    class Sample1: public NoelleGraphic::Object
    {
    public:
        Sample1()
        {
            m_TestInt = 0;
            m_TestFloat = 0;
            m_FloatData = nullptr;
            m_FloatDataNum = 0;
            m_FloatFixedData = new float[4];
            m_StrData = "";
            m_Sample2UniquePtr = nullptr;
            m_Sample2SharedPtr = nullptr;
            m_VecData.clear();
            m_MapData.clear();
        }
        virtual ~Sample1()
        {
            delete[] m_FloatData;
            delete[] m_FloatFixedData;
            m_VecData.clear();
            m_MapData.clear();
        }
        DECLARE_CLASSINFO
        DECLARE_INITIAL
        int m_TestInt;
        float m_TestFloat;
        float* m_FloatData;
        unsigned int m_FloatDataNum;
        float* m_FloatFixedData;
        std::string m_StrData;
        std::unique_ptr<Sample2> m_Sample2UniquePtr;
        std::shared_ptr<Sample2> m_Sample2SharedPtr;
        std::vector<int> m_VecData;
        std::map<std::string, int> m_MapData;

        enum TestEnum
        {
            TE_1,
            TE_2,
            TE_3,
            TE_MAX
        };
        TestEnum m_EnumTest;

        static bool TestInitFunction();
        static bool TestTerminalFunction();
    };
    TYPE_MARCO(Sample1);
}

