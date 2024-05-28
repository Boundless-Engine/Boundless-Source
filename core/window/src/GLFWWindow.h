#pragma once 

/*!****************************************************************************
*	\file		GLFWWindow.h
*	\brief		Concrete window for use with application creation.
*	\author		Austin Lynes
*	\date		5/20/2024
******************************************************************************/

#include "IWindow.h"
#include "Event.h"

namespace Boundless {



	class GLFWWindow : public I::IWindow {
	public:
		GLFWWindow();
		~GLFWWindow() override;

		// Inherited via IWindow
		virtual BReturn Create(int w, int h, const char* title) override;

		virtual BReturn Destroy() override;

		virtual BReturn Update() override;

		virtual BReturn Get(void**) override;
		
		virtual BReturn Dimensions(std::pair<int, int>& dims) override;


		// Inherited via IWindow
		virtual void OnEvent(I::IEvent* event) override;

	private:
		

	};
}