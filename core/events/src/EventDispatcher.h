#pragma once
#include <vector>

namespace Boundless {

	namespace I {
		struct IEvent;
		class IEventListener;
	}
	class EventDispatcher {

	public:
		static void AddListener(I::IEventListener* listener);
		static void RemoveListener(I::IEventListener* listener);
		static void Dispatch(struct I::IEvent* event);

	private:
		std::vector<I::IEventListener*> listeners;

		static EventDispatcher _instance;

		EventDispatcher() = default;
	};

}