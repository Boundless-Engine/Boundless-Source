#pragma once

namespace Boundless {
	namespace I {
		class IEventListener {
		public:
			IEventListener() = default;
			virtual ~IEventListener() {};

			virtual void OnEvent(struct IEvent* event) = 0;
		};
	}
}