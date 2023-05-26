#include "Window.h"

#include "Input.h"

#include <SDL3/sdl.h>
#include <SDL3/SDL_syswm.h>
#include <SDL3/SDL_video.h>
#include <SDL3/SDL_joystick.h>
#include <SDL3/SDL_gamepad.h>

#include <stb/stb_image.h>

#include <string>

using namespace NoelleEditor;

Window::Window(const char* pTitle, uint16_t width, uint16_t height)
	: m_width(width)
	, m_height(height)
{
	// JoyStick : SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER
	SDL_Init(SDL_INIT_EVENTS);
	SDL_SetHintWithPriority("SDL_BORDERLESS_RESIZABLE_STYLE", "1", SDL_HINT_OVERRIDE);
	SDL_SetHintWithPriority("SDL_BORDERLESS_WINDOWED_STYLE", "1", SDL_HINT_OVERRIDE);

	// If you want to implement window like Visual Studio without titlebar provided by system OS, open SDL_WINDOW_BORDERLESS.
	// But the issue is that you can't drag it unless you provide an implementation about hit test.
	// Then you also need to simulate minimize and maxmize buttons.
	m_pSDLWindow = SDL_CreateWindow(pTitle, width, height, SDL_WINDOW_RESIZABLE);
	SDL_WarpMouseInWindow(m_pSDLWindow, static_cast<int>(width * 0.5f), static_cast<int>(height * 0.5f));

	SDL_SysWMinfo wmi;
	SDL_GetWindowWMInfo(m_pSDLWindow, &wmi, SDL_SYSWM_CURRENT_VERSION);

#if CD_PLATFORM_OSX || CD_PLATFORM_IOS
	m_pNativeWindowHandle = wmi.info.cocoa.window;
#elif CD_PLATFORM_WINDOWS
	m_pNativeWindowHandle = wmi.info.win.window;
#elif CD_PLATFORM_ANDRIOD
	m_pNativeWindowHandle = wmi.info.android.window;
#endif
}

Window::~Window()
{
	SDL_Quit();
	SDL_DestroyWindow(m_pSDLWindow);
}

void Window::Closed(bool bPushSdlEvent)
{
	m_IsClosed = true;
	if (!bPushSdlEvent) { return; }

	SDL_Event sdlEvent;
	SDL_QuitEvent& quitEvent = sdlEvent.quit;
	quitEvent.type = SDL_EVENT_QUIT;
	SDL_PushEvent(&sdlEvent);
}

void Window::Update()
{
	Input::Get().Reset();

	SDL_Event sdlEvent;
	while (SDL_PollEvent(&sdlEvent))
	{
		switch (sdlEvent.type)
		{
		case SDL_EVENT_QUIT:
		{
			Closed(false);
		}
		break;
        case SDL_EVENT_WINDOW_RESIZED:
        case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
        {
            int currentWindowWidth;
            int currentWindowHeight;
            SDL_GetWindowSize(m_pSDLWindow, &currentWindowWidth, &currentWindowHeight);
            if (currentWindowWidth != m_width || currentWindowHeight != m_height)
            {
                m_width = currentWindowWidth;
                m_height = currentWindowHeight;
                SDL_SetWindowSize(m_pSDLWindow, m_width, m_height);

                OnResize.Invoke(m_width, m_height);
            }
        }
        break;
		case SDL_EVENT_MOUSE_MOTION:
		{
			// Top left is (0,0) for (x, y)
			// xrel is positive to the right, negative to the left
			// yrel is positive to the bottom, negative to the top
			const SDL_MouseMotionEvent& mouseMotionEvent = sdlEvent.motion;
			Input::Get().SetMousePositionX(mouseMotionEvent.x);
			Input::Get().SetMousePositionY(mouseMotionEvent.y);
			Input::Get().SetMousePositionOffsetX(mouseMotionEvent.xrel);
			Input::Get().SetMousePositionOffsetY(mouseMotionEvent.yrel);
		}
		break;

		case SDL_EVENT_MOUSE_BUTTON_DOWN:
		{
			switch (sdlEvent.button.button)
			{
			case SDL_BUTTON_LEFT:
				Input::Get().SetMouseLBPressed(true);
				break;
			case SDL_BUTTON_RIGHT:
				Input::Get().SetMouseRBPressed(true);
				break;
			case SDL_BUTTON_MIDDLE:
				Input::Get().SetMouseMBPressed(true);
				break;
			}
		}
		break;

		case SDL_EVENT_MOUSE_BUTTON_UP:
		{
			switch (sdlEvent.button.button)
			{
			case SDL_BUTTON_LEFT:
				Input::Get().SetMouseLBPressed(false);
				break;
			case SDL_BUTTON_RIGHT:
				Input::Get().SetMouseRBPressed(false);
				break;
			case SDL_BUTTON_MIDDLE:
				Input::Get().SetMouseMBPressed(false);
				break;
			}
		}
		break;

		case SDL_EVENT_MOUSE_WHEEL:
		{
			Input::Get().SetMouseScrollOffsetY(sdlEvent.wheel.mouseY);
		}
		break;

		case SDL_EVENT_TEXT_INPUT:
		{   
			const size_t inputLen = strlen(sdlEvent.text.text);
			Input::Get().AppendInputCharacter(sdlEvent.text.text, inputLen);
		}
		break;

		case SDL_EVENT_KEY_DOWN:
		{
			Sint32 sdlKeyCode = sdlEvent.key.keysym.sym;
			NoelleEditor::KeyMod keyMod = static_cast<NoelleEditor::KeyMod>(sdlEvent.key.keysym.mod);
			if (keyMod != NoelleEditor::KeyMod::KMOD_NULL)
			{
				Input::Get().SetModifier(keyMod);
			}
			if (sdlKeyCode >= Input::MaxKeyCode)
			{
				return;
			}
			KeyCode keyCode = static_cast<KeyCode>(static_cast<std::underlying_type_t<KeyCode>>(sdlKeyCode));
			Input::Get().SetKeyPressed(keyCode, true);
			Input::Get().AppendKeyEvent(keyCode, keyMod, true);
		}
		break;

		case SDL_EVENT_KEY_UP:
		{
			Sint32 sdlKeyCode = sdlEvent.key.keysym.sym;
			NoelleEditor::KeyMod keyMod = static_cast<NoelleEditor::KeyMod>(sdlEvent.key.keysym.mod);
			if (keyMod != NoelleEditor::KeyMod::KMOD_NULL)
			{
				Input::Get().ClearModifier(keyMod);
			}

			if (sdlKeyCode >= Input::MaxKeyCode)
			{
				return;
			}
			KeyCode keyCode = static_cast<KeyCode>(static_cast<std::underlying_type_t<KeyCode>>(sdlKeyCode));
			Input::Get().SetKeyPressed(keyCode, false);
			Input::Get().AppendKeyEvent(keyCode, keyMod, false);

		}
		break;

		case SDL_EVENT_DROP_FILE:
		{
			OnDropFile.Invoke(sdlEvent.drop.file);
		}
		break;

		default:
			break;
		}
	}
}

void Window::SetWindowIcon(const char* pFilePath) const
{
	std::string iconFilePath = CDEDITOR_RESOURCES_ROOT_PATH;
	iconFilePath += pFilePath;

	int width, height, originFormat;
	int depth = 32;
	int channels = STBI_rgb_alpha;
	void* pImageData = stbi_load(iconFilePath.c_str(), &width, &height, &originFormat, STBI_rgb_alpha);
	if (nullptr == pImageData)
	{
		return;
	}

	uint32_t maskR, maskG, maskB, maskA;
	if constexpr(SDL_BYTEORDER == SDL_BIG_ENDIAN)
	{
		maskR = 0xff000000;
		maskG = 0x00ff0000;
		maskB = 0x0000ff00;
		maskA = 0x000000ff;
	}
	else
	{
		maskR = 0x000000ff;
		maskG = 0x0000ff00;
		maskB = 0x00ff0000;
		maskA = 0xff000000;
	}
	
	SDL_Surface* pSDLSurface = SDL_CreateSurfaceFrom(pImageData, width, height, channels * width,
        SDL_GetPixelFormatEnumForMasks(depth,  maskR, maskG, maskB, maskA));

	SDL_SetWindowIcon(m_pSDLWindow, pSDLSurface);
	SDL_DestroySurface(pSDLSurface);
	stbi_image_free(pImageData);
}