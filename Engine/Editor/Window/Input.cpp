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
#include "Input.h"

#include <imgui.h>
#include <unordered_map>
#include <utility>

using namespace NoelleEditor;

Input::Input()
{
	memset(m_keyPressed, static_cast<int>(false), sizeof(bool) * MaxKeyCode);
}

bool Input::ContainsModifier(NoelleEditor::KeyMod mod) const
{
	if (m_keyModifiers == NoelleEditor::KeyMod::KMOD_NULL) {
		return false;
	}
	return static_cast<std::underlying_type_t<NoelleEditor::KeyMod>>(m_keyModifiers) & static_cast<std::underlying_type_t<NoelleEditor::KeyMod>>(mod);
}

void Input::SetModifier(NoelleEditor::KeyMod mod)
{
	if (!ContainsModifier(mod))
	{
		m_keyModifiers = static_cast<NoelleEditor::KeyMod>(static_cast<std::underlying_type_t<NoelleEditor::KeyMod>>(m_keyModifiers) | static_cast<std::underlying_type_t<NoelleEditor::KeyMod>>(mod));
	}
}

void Input::ClearModifier(NoelleEditor::KeyMod mod)
{
	if (ContainsModifier(mod))
	{
		m_keyModifiers = static_cast<NoelleEditor::KeyMod>(static_cast<std::underlying_type_t<NoelleEditor::KeyMod>>(m_keyModifiers) & ~static_cast<std::underlying_type_t<NoelleEditor::KeyMod>>(mod));
	}
}

void Input::SetKeyPressed(NoelleEditor::KeyCode code, bool pressed)
{ 
	m_keyPressed[static_cast<std::underlying_type_t<NoelleEditor::KeyCode>>(code)] = pressed;
}

void Input::AppendKeyEvent(NoelleEditor::KeyCode code, NoelleEditor::KeyMod mod, bool pressed)
{
	KeyEvent newEvent;
	newEvent.code = code;
	newEvent.mod = mod;
	newEvent.isPressed = pressed;
	m_keyEventList.push_back(newEvent);
}

void Input::AppendInputCharacter(const char* c, size_t len)
{
	if ((m_inputCharBufferIndex + len) >= MaxInputCharBuffer)
	{
		// overflown for this frame, ignore this input
		return;
	}
	for (size_t i = 0; i < len; ++i)
	{
		m_inputCharBuffer[m_inputCharBufferIndex + i] = c[i];
	}
	m_inputCharBuffer[m_inputCharBufferIndex + len] = '\0';
	m_inputCharBufferIndex += len;
}

void Input::FlushInputs()
{ 
	m_keyEventList.clear(); 
	m_inputCharBufferIndex = 0;
	m_inputCharBuffer[0] = '\0';
}