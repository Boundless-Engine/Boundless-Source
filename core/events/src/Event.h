#pragma once

#include "IEvent.h"
#include "EventDispatcher.h"
#include "EventListener.h"


namespace Boundless {
	
	namespace Events {
		
		enum Type {
			
			ApplicationStartup,		//!< The Application is Starting up... 
			ApplicationShutdown,	//!< The Application is Shutting down...	
			
			// WINDOW EVENTS 
			WindowMoved,
			WindowResized,
			WindowClosed,

			// INPUT EVENTS
			KeyReleased,
			KeyPressed,
			KeyHeld,
		};


#pragma region APPLICATION EVENTS

		struct ApplicationStartupEvent : public I::IEvent {
		public:
			bool isFirstTime;

			ApplicationStartupEvent(bool isFirst) : isFirstTime{ isFirst }, I::IEvent(ApplicationStartup) {}

		};


		struct ApplicationClosedEvent : public I::IEvent {
		public:
			bool save;

			ApplicationClosedEvent(bool save) : save{save}, I::IEvent(ApplicationShutdown){}

		};

#pragma endregion

#pragma region WINDOW EVENTS

	
		struct WindowClosedEvent : public I::IEvent {
		public: 
			void* window;
		
			WindowClosedEvent(void* window) : window{window}, I::IEvent(WindowClosed) {}
		};

		struct WindowMovedEvent : public I::IEvent {
		public:
			int posX;
			int posY;

			WindowMovedEvent(int x, int y) : posX{ x }, posY{ y }, I::IEvent(WindowMoved) {}
		};

		struct WindowResizedEvent : public I::IEvent {
		public:
			int width;
			int height;

			WindowResizedEvent(int width, int height) : width{ width }, height{ height }, I::IEvent(WindowResized) {}
		};

#pragma endregion

#pragma region KEY EVENTS

		struct KeyReleaseEvent : public I::IEvent {
		public:
			int keycode;


			KeyReleaseEvent(int keycode) : keycode{ keycode }, I::IEvent(KeyReleased) { }

		};

		struct  KeyPressEvent : public I::IEvent {
		public:

			KeyPressEvent(int keycode) : keycode{ keycode }, I::IEvent(KeyPressed) {}

			int keycode;
		};

		struct  KeyHeldEvent : public I::IEvent {
		public:

			KeyHeldEvent(int keycode) : keycode{ keycode }, I::IEvent(KeyHeld) {}

			int keycode;
		};


#pragma endregion
	}
}