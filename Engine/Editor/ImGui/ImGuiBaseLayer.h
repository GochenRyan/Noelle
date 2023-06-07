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
namespace NoelleEditor
{
	class ImGuiBaseLayer
	{
	public:
		ImGuiBaseLayer(const char* pName) : m_pName(pName) { }
		ImGuiBaseLayer(const ImGuiBaseLayer&) = delete;
		ImGuiBaseLayer& operator=(const ImGuiBaseLayer&) = delete;
		ImGuiBaseLayer(ImGuiBaseLayer&&) = default;
		ImGuiBaseLayer& operator=(ImGuiBaseLayer&&) = default;
		virtual ~ImGuiBaseLayer() = default;

		virtual void Init() = 0;
		virtual void Update() = 0;

		const char* GetName() const { return m_pName; }
		float GetWindowPosX() const { return m_windowPosX; }
		float GetWindowPosY() const { return m_windowPosY; }
		void SetWindowPos(float x, float y) { m_windowPosX = x; m_windowPosY = y; }

		void SetEnable(bool enable) { m_isEnable = enable; }
		bool IsEnable() const { return m_isEnable; }

	protected:
		const char* m_pName = nullptr;
		float m_windowPosX = 0.0f;
		float m_windowPosY = 0.0f;
		bool m_isEnable = true;
	};
}
