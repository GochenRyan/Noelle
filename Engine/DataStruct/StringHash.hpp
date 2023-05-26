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