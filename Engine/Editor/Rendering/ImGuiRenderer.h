#pragma once

#include "Renderer.h"

namespace NoelleEditor
{
	class ImGuiRenderer final : public Renderer
	{
	public:
		using Renderer::Renderer;
		virtual ~ImGuiRenderer();

		virtual void Init() override;
		virtual void UpdateView(const float* pViewMatrix, const float* pProjectionMatrix) override;
		virtual void Render(float deltaTime) override;
	};
}