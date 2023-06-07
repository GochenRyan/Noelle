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

#include "xxhash.hpp"
#include <string_view>


namespace details
{

	template<typename T>
	struct xxhTraits;

	template<>
	struct xxhTraits<std::uint32_t>
	{
		enum {SIZE = 32};
	};

	template<>
	struct xxhTraits<std::uint64_t>
	{
		enum {SIZE = 64};
	};

}


template<typename T>
T StringHash(const char* str, std::size_t n)
{
	using Traits = details::xxhTraits<T>;
	return xxh::xxhash<Traits::SIZE>(str, n);
}

template<typename T>
T StringHash(std::string_view sv)
{
	return StringHash<T>(sv.data(), sv.size());
}