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
#pragma once

#include "StringHash.hpp"


template<typename T>
class TStringCrc final
{
public:
	TStringCrc() = default;
	explicit TStringCrc(std::string_view sv) : m_hashValue(StringHash<T>(sv)) {}
	explicit TStringCrc(const char* str, std::size_t n) : m_hashValue(StringHash<T>(str, n)) {}
	TStringCrc(const TStringCrc&) = default;
	TStringCrc& operator=(const TStringCrc&) = default;
	TStringCrc(TStringCrc&&) = default;
	TStringCrc& operator=(TStringCrc&&) = default;
	~TStringCrc() = default;

	T Value() const { return m_hashValue; }
	bool operator==(const TStringCrc& other) const { return m_hashValue == other.m_hashValue; }
	bool operator!=(const TStringCrc& other) const { return m_hashValue != other.m_hashValue; }

private:
	T m_hashValue;
};

using StringCrc = TStringCrc<uint32_t>;