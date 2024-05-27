#pragma once

#include "Window.h"
#include "InputHandler.h"

using namespace Boundless;

namespace EventSystemTest {


	namespace {
		int width = 640;
		int height = 480;
		const char* title = "Key Event(s) Test";
	}

	int KeyEventsTest() {
		I::IWindow* win;
		I::IInputHandler* inp;

		Window::Create(width, height, title, &win);
		Input::Create(&inp);


		while (true) {
			win->Update();
		}
	}
}
