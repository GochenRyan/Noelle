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