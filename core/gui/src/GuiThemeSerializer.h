#pragma once

#include "pch.h"

namespace Boundless {

	namespace Graphics{

		struct GuiTheme;

		namespace Gui {

			namespace Theme {
		
				BReturn Save(const std::string& filepath, GuiTheme* pTheme);

				BReturn Load(const std::string& filepath, GuiTheme* pTheme);

				BReturn Apply(const GuiTheme* theme);

			}
		}
	}
}