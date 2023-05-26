#pragma once

#include "VertexAttribute.h"

#include <bgfx/bgfx.h>

#include <vector>

namespace NoelleEditor
{
	class VertexLayoutUtility
	{
	public:
		static void CreateVertexLayout(bgfx::VertexLayout& outVertexLayout, const std::vector<VertexAttributeLayout>& vertexAttributes, bool debugPrint = false);
		static void CreateVertexLayout(bgfx::VertexLayout& outVertexLayout, const VertexAttributeLayout& vertexAttribute, bool debugPrint = false);
	};
}