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

#include "Runtime.h"
#include <unordered_map>
#include <string>

namespace Noelle
{
    class RUNTIME_API Command final
    {
    public:
        Command(int count, char* arguments[]);
        ~Command();

        bool ContainsName(const std::string name);
        bool GetInteger(const std::string name, int& value);
        bool GetReal(const std::string name, float& value);
        bool GetString(const std::string name, std::string& value);
    private:
        std::unordered_map<std::string, std::string> m_commandMap;
    };
}