#pragma once

/*!****************************************************************************
*	\file		ILayerable.h
*	\brief		Abstract Application Layer Management Interface.
*	\author		Austin Lynes
*	\date		5/28/2024
******************************************************************************/

#include "pch.h"
#include "ILayer.h"

namespace Boundless {

	namespace I {
		class ILayerable {
		public:
			virtual ~ILayerable() {}

			template<typename T, typename... Args>
			BReturn PushLayer(Args&&... args) {
				static_assert(std::is_base_of<I::ILayer, T>::value, "T must be derived from I::ILayer");
				try {
					auto layer = new T(std::forward<Args>(args)...);
					layer->OnAttach();
					layers.insert(layers.begin(), layer);
					return SUCCESS; // Assuming BReturn has a value SUCCESS
				}
				catch (...) {
					return FAILURE; // Assuming BReturn has a value FAILURE
				}
			}

			template<typename T>
			BReturn RemoveLayer() {
				static_assert(std::is_base_of<I::ILayer, T>::value, "T must be derived from I::ILayer");
				auto it = std::find_if(layers.begin(), layers.end(), [](I::ILayer* layer) {
					return dynamic_cast<T*>(layer) != nullptr;
					});
				if (it != layers.end()) {
					(*it)->OnDetach();
					delete* it;
					layers.erase(it);
					return SUCCESS; // Assuming BReturn has a value Success
				}
				else {
					return FAILURE; // Assuming BReturn has a value Failure
				}

			}

		protected:
			std::vector<I::ILayer*> layers;
		};
	}
}