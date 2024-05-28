#pragma once

/*!
* \file		IInputHandler.h 
* \brief	Boundless Generic Inteface for Input Handling
* \author	Austin Lynes
* \date		5/21/2024
*/

#include "Event.h"
#include "EventListener.h"
#include "EventDispatcher.h"

namespace Boundless {

	namespace I {
		class IInputHandler: public IEventListener {
		public:
			IInputHandler() { EventDispatcher::AddListener(this); }
			virtual ~IInputHandler() { EventDispatcher::RemoveListener(this); };

		};
	}
}