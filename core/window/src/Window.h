#pragma once

#include "IWindow.h"

namespace Boundless{
	namespace Window {
		BReturn Create(int width, int height, const char* title, I::IWindow** ppWindow);
		BReturn Destroy(I::IWindow** window);
	}
}