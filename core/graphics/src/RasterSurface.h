#pragma once

#include "pch.h"

namespace Boundless {
	namespace I {
		class IWindow;
		class IRasterSurface;
	}
	namespace Graphics {
		namespace RasterSurface 
		{
			BReturn	Create(I::IWindow* pWindow, I::IRasterSurface** pRasterSurface);
			BReturn	Destroy(I::IRasterSurface** pRasterSurface);
		};
	}
}