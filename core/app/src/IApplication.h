#pragma once

#include"pch.h"

#include <string>
#include "EventDispatcher.h"
#include "EventListener.h"
#include "ILayerable.h"

namespace Boundless {
	struct ApplicationPreferences {
		struct {
			int x;		//!<	pos x of the window 
			int y;		//!<	pos y of the window
			int width;	//!<	width of the window 
			int height;	//!<	height of the window;
			bool vsync;	//!<	should this application window support vsync?
		}Window;
	};

	namespace I {
		class IApplication: public IEventListener, public ILayerable {
		public:
			IApplication(const std::string& filepath) : prefs_filepath{filepath} { 
				EventDispatcher::AddListener(this); 
				LoadPrefs();
			};
			virtual ~IApplication() {
				EventDispatcher::RemoveListener(this);
			};

			virtual BReturn Initillize() = 0;
			virtual BReturn Shutdown() = 0;

			virtual BReturn Run() = 0;
			

		protected:
			BReturn LoadPrefs();
			BReturn SavePrefs(ApplicationPreferences* pPrefs = nullptr);

		
		protected:
			ApplicationPreferences prefs{};
			std::string prefs_filepath;
		
		};
	}
}
