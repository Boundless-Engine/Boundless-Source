#pragma once

#include "pch.h"
#include "ILayer.h"

namespace Boundless {

	namespace I {
		class ILayerable {
		public:
			virtual ~ILayerable() {}

			template<typename T>
			BReturn PushLayer() {
				static_assert(std::is_base_of<I::ILayer, T>::value, "T must be derived from I::ILayer");
				try {
					auto layer = new T();
					layer->OnAttach();
					layers.insert(layers.begin(), layer);
					return SUCCESS; // Assuming BReturn has a value Success
				}
				catch (...) {
					return FAILURE; // Assuming BReturn has a value Failure
				}
			}

			template<typename T>
			BReturn RemoveLayer() {
				static_assert(std::is_base_of<I::ILayer, T>::value, "T must be derived from I::ILayer");
				auto it = std::find_if(layers.begin(), layers.end(), [](I::ILayer* layer) {
					return dynamic_cast<T*>(layer) != nullptr;
					});
				if (it != layers.end()) {
					it->OnDetach();
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