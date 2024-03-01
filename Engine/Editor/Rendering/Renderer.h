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

#include <string>

namespace NoelleEditor
{
	class RenderContext;
	class RenderTarget;

	class Renderer
	{
	public:
		Renderer() = delete;
		explicit Renderer(RenderContext* pRenderContext, uint16_t viewID, RenderTarget* pRenderTarget);
		Renderer(const Renderer&) = delete;
		Renderer& operator=(const Renderer&) = delete;
		Renderer(Renderer&&) = delete;
		Renderer& operator=(Renderer&&) = delete;
		virtual ~Renderer() = default;

		virtual void Init() = 0;
		virtual void UpdateView(const float* pViewMatrix, const float* pProjectionMatrix) = 0;
		virtual void Render(float deltaTime) = 0;

		uint16_t GetViewID() const { return m_viewID; }
		const RenderTarget* GetRenderTarget() const { return m_pRenderTarget; }

		void SetEnabled(bool enable) { m_isEnable = enable; }
		void SetDisabled(bool diable) { m_isEnable = !diable; }
		bool IsEnable() const { return m_isEnable; }

	public:
		static void ScreenSpaceQuad(float _textureWidth, float _textureHeight, bool _originBottomLeft = false, float _width = 1.0f, float _height = 1.0f);

	protected:
		uint16_t		m_viewID = 0;
		RenderContext*	m_pRenderContext = nullptr;
		RenderTarget*	m_pRenderTarget = nullptr;
		bool			m_isEnable = true;
	};
}