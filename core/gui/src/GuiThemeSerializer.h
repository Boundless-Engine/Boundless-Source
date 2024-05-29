#pragma once

#include "pch.h"

#include "imgui.h"

namespace Boundless {

	namespace Graphics{

		struct GuiTheme;

		namespace Gui {

			namespace Theme {
		
				BReturn Save(const std::string& filepath, GuiTheme* pTheme);
				
				BReturn Save(const std::string& filepath, ImGuiStyle& style);
				
				BReturn Load(const std::string& filepath, GuiTheme* pTheme);

				BReturn Apply(const GuiTheme* theme);

			}
		}
	}
}