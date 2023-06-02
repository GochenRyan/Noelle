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