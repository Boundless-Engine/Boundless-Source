#pragma once

#include "pch.h"

#include "IWindow.h"
#include "IGraphicsAPI.h"

#ifdef ENABLE_VULKAN
#include "VulkanAPI.h"
#endif

namespace Boundless {
	
	namespace Graphics {
		namespace GraphicsAPI 
		{
			static BReturn Create(I::IWindow* pWindow, I::IGraphicsAPI** ppAPI)
			{
				//@TODO: Create Native RasterHandle.
#if		defined(NATIVE_WINDOW_HANDLES)
				*ppAPI = new NativeRasterSurface();
#elif	defined(ENABLE_VULKAN)
				*ppAPI = new VulkanAPI(pWindow);
#elif	defined(ENABLE_DIRECTX)
				*ppAPI new DirectXSurface();
#endif 
				return SUCCESS;
			}

			static BReturn Destroy(I::IGraphicsAPI** ppAPI)
			{
				if (*ppAPI == nullptr)
					return FAILURE;

				delete* ppAPI;

				return SUCCESS;

			}
		};
	}
}