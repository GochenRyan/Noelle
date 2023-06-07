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
#include <cstdint>
#include <type_traits>

namespace NoelleEditor
{
	enum class VertexAttributeType : uint8_t
	{
		Position,
		Normal,
		Tangent,
		Bitangent,
		UV,
		Color,
		BoneWeight,
		BoneIndex,
	};

	enum class AttributeValueType : uint8_t
	{
		Uint8,
		Float,
		Int16,
	};

	template<typename T>
	static constexpr AttributeValueType GetAttributeValueType()
	{
		if constexpr (std::is_same<T, uint8_t>())
		{
			return AttributeValueType::Uint8;
		}
		else if constexpr (std::is_same<T, uint16_t>())
		{
			return AttributeValueType::Int16;
		}
		else if constexpr (std::is_same<T, float>())
		{
			return AttributeValueType::Float;
		}
		else
		{
			return AttributeValueType::Float;
		}
	}

	struct VertexAttributeLayout
	{
		VertexAttributeType vertexAttributeType;
		AttributeValueType attributeValueType;
		uint8_t attributeCount;
		uint8_t padding;
	};

}

