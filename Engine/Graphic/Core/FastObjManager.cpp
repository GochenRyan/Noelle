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

#include "FastObjManager.h"

using namespace NoelleGraphic;

FastObjManager::FastObjManager()
{
}

FastObjManager::~FastObjManager()
{
}

void FastObjManager::AddObject(Object* obj)
{
    m_objectSet.insert(obj);
}

void FastObjManager::DeleteObject(Object* obj)
{
    m_objectSet.erase(obj);
}

bool FastObjManager::IsClear()
{
    return m_objectSet.size() == 0;
}

unsigned int FastObjManager::GetObjectNum()
{
    return m_objectSet.size();
}