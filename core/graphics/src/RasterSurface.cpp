#include "RasterSurface.h"

#include "IRasterSurface.h"

#define ENABLE_VULKAN

#ifdef ENABLE_VULKAN
	#include "VulkanSurface.h"
#endif

namespace Boundless {
	namespace Graphics {
		namespace RasterSurface {

			BReturn Create(I::IWindow* pWindow, I::IRasterSurface** ppRasterSurface)
			{
				//@TODO: Create Native RasterHandle.
#if		defined(NATIVE_WINDOW_HANDLES)
				*pRasterSurface = new NativeRasterSurface(); 
#elif	defined(ENABLE_VULKAN)
				*ppRasterSurface = new VulkanSurface(pWindow);
#elif	defined(ENABLE_DIRECTX)
				*pRasterSurface new DirectXSurface();
#else 
#error "one of [NATIVE_WINDOW_HANDLES, ENABLE_VULKAN, ENABLE_DIRECTX] MUST be defined for graphics to work properly..."
#endif 
				return SUCCESS;
			}

			BReturn Destroy(I::IRasterSurface** ppRasterSurface) {
				if (*ppRasterSurface == nullptr)
					return FAILURE;

				delete* ppRasterSurface;
				
				return SUCCESS;

			}
		}
	}
}
