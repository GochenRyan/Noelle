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

#include "MulticastDelegate.hpp"

#include <bgfx/bgfx.h>

#include <inttypes.h>
#include <memory>
#include <vector>

namespace NoelleEditor
{
	enum class TextureFormat
	{
		RGBA32F,
		D32F
	};

	struct AttachmentDescriptor
	{
		TextureFormat textureFormat;
	};

	class RenderTarget
	{
	public:
		RenderTarget() = delete;
		explicit RenderTarget(uint16_t width, uint16_t height, void* hwnd);
		explicit RenderTarget(uint16_t width, uint16_t height, std::vector<AttachmentDescriptor> attachmentDescs);
		RenderTarget(const RenderTarget&) = delete;
		RenderTarget& operator=(const RenderTarget&) = delete;
		RenderTarget(RenderTarget&&) = delete;
		RenderTarget& operator=(RenderTarget&&) = delete;
		~RenderTarget() = default;

		bool IsSwapChainTarget() const { return m_hwnd != nullptr && m_attachmentDescriptors.empty(); }
		uint16_t GetWidth() const { return m_width; }
		uint16_t GetHeight() const { return m_height; }
		void Resize(uint16_t width, uint16_t height);
		float GetAspect() const { return static_cast<float>(m_width) / static_cast<float>(m_height); }

		const bgfx::FrameBufferHandle* GetFrameBufferHandle() const { return m_pFrameBufferHandle.get(); }
		bgfx::TextureHandle GetTextureHandle(int index) const;

	public:
		MulticastDelegate<void(uint16_t, uint16_t)> OnResize;

	private:
		uint16_t m_width = 1;
		uint16_t m_height = 1;
		void* m_hwnd = nullptr;
		std::vector<AttachmentDescriptor> m_attachmentDescriptors;

		std::unique_ptr<bgfx::FrameBufferHandle> m_pFrameBufferHandle;
	};
}