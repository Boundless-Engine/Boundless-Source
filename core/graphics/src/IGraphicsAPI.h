#pragma once

#include "pch.h"
#include "IWindow.h"

namespace Boundless {
	namespace I {
		class IGraphicsAPI {
		public:
			IGraphicsAPI(IWindow* win) : win{ win } {}
			
			virtual ~IGraphicsAPI() {}

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
