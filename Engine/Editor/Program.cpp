// Copyright 2022 Gaocheng Ran <rangaocheng@outlook.com>

// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at

//     http://www.apache.org/licenses/LICENSE-2.0

// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
#define NOMINMAX
#include "MemManager.h"
#undef NOMINMAX

#include <Application/Engine.h>
#include <Application/Command.h>

#include "Application.h"


int main(int argc, char* argv[])
{
	Noelle::StackMemAlloc<Noelle::EngineInitArgs> initArgs(1);
	initArgs.GetPtr()->pTitle = "Noelle";
	initArgs.GetPtr()->pIconFilePath = "editor_icon.png";
	initArgs.GetPtr()->language = Noelle::Language::ChineseSimplied;
	initArgs.GetPtr()->width = 1920;
	initArgs.GetPtr()->height = 1080;

    Noelle::Engine* pEngine = Noelle::Engine::Create(std::make_unique<NoelleEditor::Application>());

    pEngine->Init(*(initArgs.GetPtr()));
	pEngine->Run();
	Noelle::Engine::Destroy(pEngine);

    return 0;
}
    