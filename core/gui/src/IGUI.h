#pragma once

#include <functional>

namespace Boundless {
	namespace I {
		class IGUI {
		public:
			IGUI() {}
			virtual  ~IGUI() {}

			virtual void Begin() = 0;
			virtual void End() = 0;
			virtual void Present() = 0;

			void SetMenuCallback(std::function<void()> fn) {
				menubarCallback = fn;
			}

		protected:
			std::function<void()> menubarCallback;
		};
	}
}