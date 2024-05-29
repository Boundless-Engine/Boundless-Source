#pragma once

#include "pch.h"

#include "IGUI.h"

#include "IGraphicsAPI.h"
#include "VulkanAPI.h"

#include "VulkanGUI.h"

#include "GUITheme.h"
#include "GUIThemeSerializer.h"


//#define IMGUI_UNLIMITED_FRAME_RATE
namespace Boundless {
	namespace Graphics {

		namespace Gui
		{
		
			static BReturn Create(I::IGraphicsAPI* rasterSurface, I::IGUI** pGUI, const std::string& themeFilepath, std::function<void()> menu = nullptr)
			{
#if			defined(NATIVE_WINDOW_HANDLES)
#elif		defined(ENABLE_VULKAN)
				*pGUI = new Graphics::VulkanGUI(static_cast<Graphics::VulkanAPI*>(rasterSurface));
#elif		defined(ENABLE_DIRECTX)
#endif

				if (menu != nullptr)
					(*pGUI)->SetMenuCallback(menu);
				

				GuiTheme theme;

				auto r = Theme::Load(themeFilepath, &theme);
				
				if (r == SUCCESS)
					Theme::Apply(&theme);
				
				return SUCCESS;
			}


			static BReturn Destroy(I::IGUI** pGUI)
			{
				if (*pGUI == nullptr)
					return FAILURE;

				delete* pGUI;

				return SUCCESS;
			}

	
			

		}
	}
}
