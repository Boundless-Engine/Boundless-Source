#pragma once

#include "pch.h"

#include "Event.h"

namespace Boundless {

	namespace I {

		class ILayer  {
		public:
			ILayer()			{ 	 }
			virtual ~ILayer()	{	 };

			virtual BReturn OnAttach() = 0;
			virtual BReturn OnDetach() = 0;

			//! optional Update loop function.
			virtual void OnUpdate(){}

			//! optional GUI draw loop function.
			virtual void OnGUI(){}

			//! optional event loop function.
			virtual BReturn OnEvent(const IEvent* event) { return SUCCESS; };

		};
	}
}