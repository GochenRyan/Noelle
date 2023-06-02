#pragma once

#include "ImGuiBaseLayer.h"

namespace NoelleEditor
{
	class MainMenu : public ImGuiBaseLayer
	{
	public:
		using ImGuiBaseLayer::ImGuiBaseLayer;
		virtual ~MainMenu();

		virtual void Init() override;
		virtual void Update() override;

		void FileMenu();
		void EditMenu();
		void WindowMenu();
		void BuildMenu();
		void AboutMenu();
	};
}

