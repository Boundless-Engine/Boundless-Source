#include "GLFWWindow.h"

#include <iostream>

#include <stdexcept>

#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"



namespace Boundless {
	
	static bool isGLFWInitillized = false;

	static void key_callback_thunk(GLFWwindow* w, int k, int sc, int a, int m) 
	{
		if (a == GLFW_PRESS) {				// the client just pressed a key.
			Events::KeyPressEvent keyPress(k);
			EventDispatcher::Dispatch(&keyPress);
		}
		else if (a == GLFW_RELEASE) {		// the client just released a key
			Events::KeyReleaseEvent keyRelease(k);
			EventDispatcher::Dispatch(&keyRelease);
		}
		else {								// the client is holding a key
			Events::KeyHeldEvent keyHeld(k);
			EventDispatcher::Dispatch(&keyHeld);
		}
	}

	static void window_close_thunk(GLFWwindow* win) {
		Events::WindowClosedEvent closeWindow((void* )win);
		EventDispatcher::Dispatch(&closeWindow);
	}

	static void window_resize_thunk(GLFWwindow* win, int w, int h) {
		Events::WindowResizedEvent resize(w, h);
		EventDispatcher::Dispatch(&resize);

		glfwSetWindowSize(win, w, h);
	}

	static void window_moved_thunk(GLFWwindow* win, int x, int y) {
		Events::WindowMovedEvent move(x, y);
		EventDispatcher::Dispatch(&move);
	}

	GLFWWindow::GLFWWindow()
	{
		if (!isGLFWInitillized)
		{
			int success = glfwInit();
		}

		
		isGLFWInitillized = true;
	}

	GLFWWindow::~GLFWWindow()
	{
		if (isGLFWInitillized) {
			isGLFWInitillized = false;
			glfwTerminate();
		}
	}


	BReturn GLFWWindow::Get(void** outHandle)
	{
		if (handle == nullptr)
			return HANDLE_IS_NULL;

		*outHandle = handle;

		return SUCCESS;
	}

	BReturn GLFWWindow::Dimensions(std::pair<int, int>& dims)
	{
		glfwGetFramebufferSize((GLFWwindow*)handle, &dims.first, &dims.second);

		return SUCCESS;
	}

	void GLFWWindow::OnEvent(I::IEvent* event)
	{

	}

	BReturn GLFWWindow::Create(int w, int h, const char* title)
	{
	
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

		handle = (void*)glfwCreateWindow(w, h, title, nullptr, nullptr);

		if (handle == nullptr)
			throw std::runtime_error("Could Not Create Requested BWindow");

		// link in all event handlers
		glfwSetKeyCallback((GLFWwindow*)handle, key_callback_thunk);
		glfwSetWindowCloseCallback((GLFWwindow*)handle, window_close_thunk);
		glfwSetFramebufferSizeCallback((GLFWwindow*)handle, window_resize_thunk);
		glfwSetWindowPosCallback((GLFWwindow*)handle, window_moved_thunk);

		return SUCCESS;
	}

	BReturn GLFWWindow::Destroy()
	{
		glfwDestroyWindow((GLFWwindow*)handle);
		return SUCCESS;
	}

	BReturn GLFWWindow::Update()
	{
		glfwPollEvents();

		return SUCCESS;
	}


}