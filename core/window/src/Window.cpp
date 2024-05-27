#include "Window.h"


#include "GLFWWindow.h"

namespace Boundless {
    namespace Window {
        BReturn Create(int width, int height, const char* title, I::IWindow** ppWindow)
        {
            BReturn _ = SUCCESS;

            *ppWindow = nullptr;

            #ifdef NATIVE_WINDOW_HANDLES
                #if _WIN32
                #elif _UNIX and not defined(_APPLE)
                #elif _APPLE
                #endif
            #else
                auto win = new GLFWWindow();
                _ = win->Create(width, height, title);
                *ppWindow = win;
                return _;
        #endif

            return SUCCESS;
        }
        BReturn Destroy(I::IWindow** ppWindow)
        {
            auto _ptr = *ppWindow;
         
            if (_ptr == nullptr)
                return ATTEMPT_DELETE_NULLPTR;
        
            delete _ptr;
            return SUCCESS;
        }
    }
}
