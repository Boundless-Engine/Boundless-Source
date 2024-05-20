#include "BWindow.h"

#include "GLFW/glfw3.h"
#include <stdexcept>

namespace BoundlessEngine {

	BWindow::BWindow(int width, int height) {
		
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

		GLFWwindow* win = glfwCreateWindow(width, height, "new window", nullptr, nullptr);
		handle = static_cast<void*>(win);

		if (handle == nullptr)
			throw std::runtime_error("Could Not Create Requested BWindow");

	}

	BWindow::~BWindow()
	{
	}

	void* BWindow::GetWindowHandle()
	{
		return handle;
	}

}