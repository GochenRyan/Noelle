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
#include "Command.h"
#include <sstream>

using namespace Noelle;

Command::Command(int count, char* arguments[])
{
    for (int i = 0; i < count; ++i)
    {
        std::stringstream iss(arguments[i]);
        std::string key;
        std::string value;
        std::getline(iss, key, '=');
        std::getline(iss, value);
        m_commandMap[key] = value;
    }
}

Command::~Command()
{

}

bool Command::ContainsName(const std::string name)
{
    return m_commandMap.contains(name);
}

bool Command::GetInteger(const std::string name, int& value)
{
    if (ContainsName(name))
    {
        value = std::stoi(m_commandMap[name]);
        return true;
    }
    return false;
}

bool Command::GetReal(const std::string name, float& value)
{
    if (ContainsName(name))
    {
        value = std::stof(m_commandMap[name]);
        return true;
    }
    return false;
}

bool Command::GetString(const std::string name, std::string& value)
{
    if (ContainsName(name))
    {
        value = m_commandMap[name];
        return true;
    }
    return false;
}