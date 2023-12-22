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

#include "Application/Command.h"
#include "Core/Context.h"
#include "Core/ClassInfo.h"
#include "Sample1.h"
#include "Sample2.h"

#include <spdlog/spdlog.h>

#include <memory>


int main(int argc, char* argv[])
{
    spdlog::info("Property Sample 1");
    std::unique_ptr<Noelle::Command> commands = std::make_unique<Noelle::Command>(argc, argv);

    NoelleGraphic::Context::Initialize();

    std::unique_ptr<Sample1> sample1 = std::make_unique<Sample1>();
    sample1->m_TestInt = 10086;
    std::unique_ptr<Sample2> sample2 = std::make_unique<Sample2>();

    NoelleGraphic::ClassInfo* type1 = &(sample1->GetType());
    NoelleGraphic::ClassInfo* type2 = &(sample2->GetType());

    int* reflectIntPointer = (int*)type1->GetProperty(1)->GetValueAddress((void*)&(*sample1));

    NoelleGraphic::Context::Terminal();
}