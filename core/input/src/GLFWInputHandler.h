#pragma once

#include "IInputHandler.h"

namespace Boundless
{
	class GLFWInputHandler : public I::IInputHandler {
	public:
		GLFWInputHandler() {};
		~GLFWInputHandler() {};

		// Inherited via IInputHandler
		virtual void OnEvent(I::IEvent* event) override;
	};
}