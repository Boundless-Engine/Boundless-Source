#include "InputHandler.h"

#include "GLFWInputHandler.h"

namespace Boundless {
	namespace Input {
		BReturn Create(I::IInputHandler** ppInput)
		{
			if (*ppInput != nullptr)
				return FAILURE;

		#ifdef NATIVE_WINDOW_HANDLES
		#else
			auto iHandler = new GLFWInputHandler();
			*ppInput = iHandler;
		#endif

			return SUCCESS;
		}

		BReturn Destroy(I::IInputHandler** ppInput)
		{
			if (ppInput == nullptr)
				return FAILURE;

			auto _ptr = *ppInput;
			
			delete _ptr;

			return SUCCESS;
		}
	}
}
