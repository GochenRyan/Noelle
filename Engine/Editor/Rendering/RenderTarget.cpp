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
#include "RenderTarget.h"

using namespace NoelleEditor;

RenderTarget::RenderTarget(uint16_t width, uint16_t height, void* hwnd) :
	m_hwnd(hwnd)
{
	Resize(width, height);
}

RenderTarget::RenderTarget(uint16_t width, uint16_t height, std::vector<AttachmentDescriptor> attachmentDescs) :
	m_attachmentDescriptors(std::move(attachmentDescs))
{
	Resize(width, height);
}

bgfx::TextureHandle RenderTarget::GetTextureHandle(int index) const
{
	return bgfx::getTexture(*m_pFrameBufferHandle.get(), index);
}

void RenderTarget::Resize(uint16_t width, uint16_t height)
{
	if (width == m_width && height == m_height)
	{
		return;
	}

	m_width = width;
	m_height = height;

	if (!m_pFrameBufferHandle)
	{
		m_pFrameBufferHandle = std::make_unique<bgfx::FrameBufferHandle>();
	}
	else
	{
		// When creating the frame buffer, we specified the flag to auto release textures.
		bgfx::destroy(*m_pFrameBufferHandle.get());
	}

	if (IsSwapChainTarget())
	{
		*m_pFrameBufferHandle = bgfx::createFrameBuffer(m_hwnd, width, height);
	}
	else
	{
		std::vector<bgfx::TextureHandle> textureHandles;
		textureHandles.reserve(m_attachmentDescriptors.size());
		for (const auto& attachmentDescriptor : m_attachmentDescriptors)
		{
			const uint64_t tsFlags = BGFX_TEXTURE_RT | BGFX_SAMPLER_U_CLAMP | BGFX_SAMPLER_V_CLAMP;
			bgfx::TextureFormat::Enum textureFormat;
			switch (attachmentDescriptor.textureFormat)
			{
			case TextureFormat::D32F:
				textureFormat = bgfx::TextureFormat::D32F;
				break;
			case TextureFormat::RGBA32F:
			default:
				textureFormat = bgfx::TextureFormat::RGBA32F;
				break;
			};
			textureHandles.push_back(bgfx::createTexture2D(width, height, false, 1, textureFormat, tsFlags));
		}
		*m_pFrameBufferHandle = bgfx::createFrameBuffer(static_cast<uint8_t>(textureHandles.size()), textureHandles.data(), true);
	}

	OnResize.Invoke(m_width, m_height);
}