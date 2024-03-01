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

#include "Sample2.h"

#include <spdlog/spdlog.h>

IMPLEMENT_CLASSINFO(Sample2, NoelleGraphic::Object)
NO_PROPERTY(Sample2)
IMPLEMENT_INITIAL_BEGIN(Sample2)
ADD_INITIAL_FUNCTION_WITH_PRIORITY(Sample2::TestInitFunction)
ADD_TERMINAL_FUNCTION(Sample2::TestTerminalFunction)
IMPLEMENT_INITIAL_END


bool Sample2::TestInitFunction()
{
    spdlog::info("Sample2::TestInitFunction");
    return true;
}

bool Sample2::TestTerminalFunction()
{
    spdlog::info("Sample2::TestTerminalFunction");
    return true;
}
