#pragma once

#include "pch.h"
#include "IWindow.h"

namespace Boundless {
	namespace I {
		class IGraphicsAPI {
		public:
			IGraphicsAPI(IWindow* win) : win{ win } {}
			
			virtual ~IGraphicsAPI() {}

			uint32_t Width()  {	return (uint32_t) win->Width(); }
			uint32_t Height() { return (uint32_t) win->Height(); }

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
