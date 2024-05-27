#pragma once

#include "TestFramework.h"

#include "Window.h"

namespace {
	int width = 640;
	int height = 480; 
	const char* title = "Window Test";
}

TEST_CASE("Window Can be Created", 
	[&]() -> bool
	{
		Boundless::I::IWindow* pWindow = nullptr;

		bool pass = Boundless::Window::Create(width, height, title, &pWindow) == SUCCESS;

		delete pWindow;

		return pass;
	}
);

TEST_CASE("Create Window can be Destroyed", 
	[&]() -> bool
	{
		Boundless::I::IWindow* pWindow = nullptr;

		bool pass = Boundless::Window::Create(width, height, title, &pWindow) == SUCCESS;
	
		pass = Boundless::Window::Destroy(&pWindow) == SUCCESS;

		return pass;
	}
)

