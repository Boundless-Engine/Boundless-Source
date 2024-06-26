#pragma once

/*!****************************************************************************
*	\file		IRenderer.h
*	\brief		Abstract Renderer Interface.
*	\author		Austin Lynes
*	\date		5/28/2024
******************************************************************************/

#include "pch.h"

#include "Event.h"
#include "EventListener.h"
#include "EventDispatcher.h"

namespace Boundless {
	namespace I {
		class IRenderer : public IEventListener {
		public:
			IRenderer() { EventDispatcher::AddListener(this); }
			virtual ~IRenderer() { EventDispatcher::RemoveListener(this); };

			virtual void Initillize() = 0;
			virtual void Shutdown() = 0;

			virtual void Update(float ts) = 0;
			virtual void Render() = 0;
			
		};
	}
}
