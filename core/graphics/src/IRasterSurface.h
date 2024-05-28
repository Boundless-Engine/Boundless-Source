#pragma once

#include "pch.h"
#include "IWindow.h"

namespace Boundless {
	namespace I {
		class IRasterSurface {
		public:
			IRasterSurface(IWindow* win) : win{ win } {}
			
			virtual ~IRasterSurface() {}

			virtual void BeginFrame() = 0;
			virtual void EndFrame() = 0;

			int Width() { return win->Width(); }
			int Height() { return win->Height(); }

			BReturn GetWindowHandle(void** pHandle)
			{
				if (win == nullptr)
					return FAILURE;

				win->Get(pHandle);

				return SUCCESS;
			}

		private:
			IWindow* win	= nullptr;


		};
	}
}
