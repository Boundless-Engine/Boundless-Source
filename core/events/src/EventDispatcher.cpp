#include "EventDispatcher.h"

#include "./Event.h"
#include "./EventListener.h"

namespace Boundless {
	EventDispatcher EventDispatcher::_instance{};

	void EventDispatcher::AddListener(I::IEventListener* listener)
	{
		_instance.listeners.push_back(listener);
	}

	void EventDispatcher::RemoveListener(I::IEventListener* listener)
	{
		_instance.listeners.erase(std::remove(_instance.listeners.begin(), _instance.listeners.end(), listener));
	}

	void EventDispatcher::Dispatch(I::IEvent* event)
	{
		for (int i = 0; i < _instance.listeners.size(); i++)
		{
			auto l = _instance.listeners[i];
			if(l != nullptr)
				l->OnEvent(event);
		}
		
	}
}
