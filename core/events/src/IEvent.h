#pragma once 

#include <cstdint>

namespace Boundless {
	namespace I {
		struct IEvent {
			IEvent(uint32_t type) : type{ type } {};
			virtual ~IEvent() = default;

			uint32_t type;
		};
	}

}