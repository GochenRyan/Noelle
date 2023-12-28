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

#include "MemManager.h"
#include "Application/Command.h"
#include "Core/Context.h"
#include "Core/ClassInfo.h"
#include "Core/Stream/Stream.h"
#include "Sample1.h"

#include <spdlog/spdlog.h>

#include <memory>

using namespace NoelleGraphic;

int main(int argc, char* argv[])
{
    spdlog::info("Archive Sample 1");
    std::unique_ptr<Noelle::Command> commands = std::make_unique<Noelle::Command>(argc, argv);

    NoelleGraphic::Context::Initialize();

    /* SERIALIZE BEGIN */

    std::unique_ptr<Sample1> pSample1 = std::make_unique<Sample1>();
    pSample1->m_TestInt = 10086;
    pSample1->m_TestFloat = 100.86f;
    
    pSample1->m_FloatDataNum = 4;
    pSample1->m_FloatData = new float[4];
    pSample1->m_FloatData[0] = 1.0f;
    pSample1->m_FloatData[1] = 2.0f;
    pSample1->m_FloatData[2] = 3.0f;
    pSample1->m_FloatData[3] = 4.0f;

    pSample1->m_FloatFixedData[0] = 1.0f;
    pSample1->m_FloatFixedData[1] = 2.0f;
    pSample1->m_FloatFixedData[2] = 3.0f;
    pSample1->m_FloatFixedData[3] = 4.0f;

    pSample1->m_Sample2UniquePtr = std::make_unique<Sample2>();
    pSample1->m_Sample2SharedPtr = std::make_shared<Sample2>();
    pSample1->m_VecData = {1, 0, 0, 8, 6};
    pSample1->m_StrData = "10086";
    pSample1->m_MapData = {
        {"One", 1},
        {"Zero", 0},
        {"Eight", 8},
        {"Six", 6}
    };

    pSample1->m_EnumTest = Sample1::TestEnum::TE_2;
    std::unique_ptr<NoelleGraphic::BinaryStream> binaryStream = std::make_unique<NoelleGraphic::BinaryStream>();
    binaryStream->SetStreamFlag(NoelleGraphic::BinaryStream::AT_REGISTER);
    binaryStream->ArchiveAll(pSample1.get());
    binaryStream->Save("archive_sample_1.bin");

    /* SERIALIZE END */

    /* DESERIALIZE BEGIN */

    binaryStream->Load("archive_sample_1.bin");
    Object* pObject = nullptr;
    if (binaryStream->GetObjectByType(Sample1::ms_Type, pObject))
    {
        std::unique_ptr<Sample1> pNewSample1(static_cast<Sample1*>(pObject));
    }

    /* DESERIALIZE END */

    NoelleGraphic::Context::Terminal();
}