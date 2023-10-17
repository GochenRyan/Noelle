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

#include "Sample1.h"
#include "Sample2.h"

#include <spdlog/spdlog.h>

IMPLEMENT_CLASSINFO(Sample1, NoelleGraphic::Object)
IMPLEMENT_INITIAL_BEGIN(Sample1)
ADD_PRIORITY(Sample2)
ADD_INITIAL_FUNCTION_WITH_PRIORITY(Sample1::TestInitFunction)
ADD_TERMINAL_FUNCTION(Sample1::TestTerminalFunction)
IMPLEMENT_INITIAL_END
ADD_PROPERTY_BEGIN(Sample1, NoelleGraphic::Object)
REGISTER_PROPERTY(m_TestInt, TestInt, NoelleGraphic::Property::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY(m_TestFloat, TestFloat, NoelleGraphic::Property::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY_DATA(m_FloatData, m_FloatDataNum, NoelleGraphic::Property::F_SAVE_LOAD_CLONE)
REGISTER_PROPERTY_FIXED_DATA(m_FloatFixedData, 4, FloatFixedData, false)
REGISTER_ENUM_PROPERTY(m_EnumTest, TestEnum, EnumTest, NoelleGraphic::Property::F_SAVE_LOAD_CLONE)
ADD_PROPERTY_END

bool Sample1::TestInitFunction()
{
    spdlog::info("Sample1::TestInitFunction");
    return true;
}

bool Sample1::TestTerminalFunction()
{
    spdlog::info("Sample1::TestTerminalFunction");
    return true;
}