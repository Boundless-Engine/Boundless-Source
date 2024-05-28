#pragma once

#include "pch.h"


#include <functional>

#include "Event.h"

namespace Boundless {
	namespace I {
		class IWindow : public IEventListener {
		public:
			IWindow() : handle{nullptr} 
								{ EventDispatcher::AddListener(this); }
			virtual ~IWindow()	{ EventDispatcher::RemoveListener(this); };
			
			virtual BReturn Create(int, int, const char*) = 0;
			virtual BReturn Destroy() = 0;
			virtual BReturn Update() = 0;

			virtual BReturn Get(void**) = 0;
		
			int Width() 
			{
				std::pair<int, int> dim = std::pair<int, int>(0, 0);
				Dimensions(dim);
				return dim.first;
			}

			int Height() {
				std::pair<int, int> dim = std::pair<int, int>(0, 0);
				Dimensions(dim);
				return dim.second;
			}

			virtual  BReturn Dimensions(std::pair<int, int>&) = 0;
		

		protected:
			void* handle;
		};
	}
}