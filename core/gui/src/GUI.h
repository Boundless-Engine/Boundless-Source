#pragma once

#include "pch.h"
#include <vector>
#include <functional>

#include "IGUI.h"

#include "IRasterSurface.h"
#include "VulkanSurface.h"

#include "VulkanGUI.h"
#include <GUITheme.h>



//#define IMGUI_UNLIMITED_FRAME_RATE
namespace Boundless {
	namespace Graphics {

		namespace Gui
		{
			static BReturn Create(I::IRasterSurface* rasterSurface, I::IGUI** pGUI, GuiTheme* theme = nullptr, std::function<void()> menu = nullptr)
			{
#if			defined(NATIVE_WINDOW_HANDLES)
#elif		defined(ENABLE_VULKAN)
				*pGUI = new Graphics::VulkanGUI(static_cast<Graphics::VulkanSurface*>(rasterSurface));
#elif		defined(ENABLE_DIRECTX)
#endif

				if (menu != nullptr)
					(*pGUI)->SetMenuCallback(menu);
				
				if (theme != nullptr)
					theme->Apply();
				
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
