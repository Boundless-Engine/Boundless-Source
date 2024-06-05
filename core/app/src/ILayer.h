#pragma once

/*!****************************************************************************
*	\file		ILayer.h
*	\brief		Abstract Application Layer Interface.
*	\author		Austin Lynes
*	\date		5/28/2024
******************************************************************************/

#include "pch.h"

#include "Event.h"

namespace Boundless {

	namespace I {

		class ILayer : public I::IEventListener  {
		public:
			ILayer() { EventDispatcher::AddListener(this); }
			virtual ~ILayer() { EventDispatcher::RemoveListener(this); };

			virtual BReturn OnAttach() = 0;
			virtual BReturn OnDetach() = 0;

			//! optional Update loop function.
			virtual void OnUpdate(float ts){}

			//! otional Render Loop Function
			virtual void OnRender(){}

			//! optional GUI draw loop function.
			virtual void OnGUI(){}


		};
	}
}