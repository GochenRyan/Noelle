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