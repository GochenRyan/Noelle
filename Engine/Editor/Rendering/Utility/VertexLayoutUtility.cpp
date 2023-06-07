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
#include "VertexLayoutUtility.h"

#include <cassert>
#include <string>
#include <vector>

using namespace NoelleEditor;

namespace
{

constexpr bgfx::Attrib::Enum AllAttribColorTypes[] = {
	bgfx::Attrib::Enum::Color0,
	bgfx::Attrib::Enum::Color1,
	bgfx::Attrib::Enum::Color2,
	bgfx::Attrib::Enum::Color3
};
constexpr uint32_t MAX_COLOR_COUNT = sizeof(AllAttribColorTypes) / sizeof(bgfx::Attrib::Enum);

constexpr bgfx::Attrib::Enum AllAttribUVTypes[] = {
	bgfx::Attrib::Enum::TexCoord0,
	bgfx::Attrib::Enum::TexCoord1,
	bgfx::Attrib::Enum::TexCoord2,
	bgfx::Attrib::Enum::TexCoord3,
	bgfx::Attrib::Enum::TexCoord4,
	bgfx::Attrib::Enum::TexCoord5,
	bgfx::Attrib::Enum::TexCoord6,
	bgfx::Attrib::Enum::TexCoord7
};
constexpr uint32_t MAX_UV_COUNT = sizeof(AllAttribUVTypes) / sizeof(bgfx::Attrib::Enum);

std::string VertexAttributeTypeToString(VertexAttributeType attribType)
{
	switch (attribType)
	{
	case VertexAttributeType::Position:
		return "Position";
	case VertexAttributeType::Normal:
		return "Normal";
	case VertexAttributeType::Tangent:
		return "Tangent";
	case VertexAttributeType::Bitangent:
		return "Bitangent";
	case VertexAttributeType::UV:
		return "UV";
	case VertexAttributeType::Color:
		return "Color";
	case VertexAttributeType::BoneIndex:
		return "BoneIndex";
	case VertexAttributeType::BoneWeight:
		return "BoneWeight";
	default:
		return "Invalid Attribute Type!";
	}
}

std::string AttributeValueTypeToString(const AttributeValueType& attribValueType)
{
	switch (attribValueType)
	{
	case AttributeValueType::Float:
		return "Float";
	case AttributeValueType::Uint8:
		return "Uint8";
	case AttributeValueType::Int16:
		return "Uint16";
	default:
		return "Invalid Attribute Value Type!";
	}
}

void ConvertVertexLayout(const VertexAttributeLayout& vertexAttributeLayout, bgfx::VertexLayout& outVertexLayout)
{
	bgfx::Attrib::Enum vertexAttribute = bgfx::Attrib::Enum::Count;
	bgfx::AttribType::Enum vertexAttributeValue = bgfx::AttribType::Enum::Count;
	bool normalized = false;

	// Attribute Type
	switch (vertexAttributeLayout.vertexAttributeType)
	{
	case VertexAttributeType::Position:
		vertexAttribute = bgfx::Attrib::Enum::Position;
		break;
	case VertexAttributeType::Normal:
		vertexAttribute = bgfx::Attrib::Enum::Normal;
		normalized = true;
		break;
	case VertexAttributeType::Tangent:
		vertexAttribute = bgfx::Attrib::Enum::Tangent;
		break;
	case VertexAttributeType::Bitangent:
		vertexAttribute = bgfx::Attrib::Enum::Bitangent;
		break;
	case VertexAttributeType::UV:
		vertexAttribute = bgfx::Attrib::Enum::Count;
		for (const bgfx::Attrib::Enum& textCoord : AllAttribUVTypes)
		{
			if (!outVertexLayout.has(textCoord))
			{
				vertexAttribute = textCoord;
				break;
			}
		}
		break;
	case VertexAttributeType::Color:
		vertexAttribute = bgfx::Attrib::Enum::Count;
		for (const bgfx::Attrib::Enum& color : AllAttribColorTypes)
		{
			if (!outVertexLayout.has(color))
			{
				vertexAttribute = color;
				break;
			}
		}
		break;
	case VertexAttributeType::BoneIndex:
		vertexAttribute = bgfx::Attrib::Enum::Indices;
		break;
	case VertexAttributeType::BoneWeight:
		vertexAttribute = bgfx::Attrib::Enum::Weight;
		break;
	default:
		vertexAttribute = bgfx::Attrib::Enum::Count;
		break;
	}

	// Attribute Value
	switch (vertexAttributeLayout.attributeValueType)
	{
	case AttributeValueType::Uint8:
		vertexAttributeValue = bgfx::AttribType::Enum::Uint8;
		break;
	case AttributeValueType::Int16:
		vertexAttributeValue = bgfx::AttribType::Enum::Int16;
		break;
	case AttributeValueType::Float:
		vertexAttributeValue = bgfx::AttribType::Enum::Float;
		break;
	default:
		vertexAttributeValue = bgfx::AttribType::Enum::Count;
		break;
	}

	assert(vertexAttribute != bgfx::Attrib::Enum::Count);
	assert(vertexAttributeValue != bgfx::AttribType::Enum::Count);
	outVertexLayout.add(vertexAttribute, vertexAttributeLayout.attributeCount, vertexAttributeValue, normalized);
}

}

// static
void VertexLayoutUtility::CreateVertexLayout(bgfx::VertexLayout& outVertexLayout, const std::vector<VertexAttributeLayout>& vertexAttributes, bool debugPrint /* = false */)
{
	outVertexLayout.begin();
	for (const VertexAttributeLayout& vertexAttributeLayout : vertexAttributes)
	{
		ConvertVertexLayout(vertexAttributeLayout, outVertexLayout);
	}
	outVertexLayout.end();
}

// static
void VertexLayoutUtility::CreateVertexLayout(bgfx::VertexLayout& outVertexLayout, const VertexAttributeLayout& vertexAttribute, bool debugPrint /* = false */)
{
	outVertexLayout.begin();
	ConvertVertexLayout(vertexAttribute, outVertexLayout);
	outVertexLayout.end();
}