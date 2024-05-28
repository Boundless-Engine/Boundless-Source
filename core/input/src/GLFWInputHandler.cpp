#include "GLFWInputHandler.h"

#include <iostream>
#include "Event.h"

namespace Boundless {

	void GLFWInputHandler::OnEvent(I::IEvent* event)
	{
		if (event->type == Events::KeyPressed) {
			auto keyPressed = static_cast<Events::KeyPressEvent*>(event);
			std::cout << "Key Pressed: " << keyPressed->keycode << std::endl;
		}
		else if (event->type == Events::KeyPressed) {

			auto keyReleased = static_cast<Events::KeyReleaseEvent*>(event);
			std::cout << "Key Released: " << keyReleased->keycode<< std::endl;
		}
	}

}