#pragma once


namespace BoundlessEngine {
	class BWindow {
	public:
		BWindow() = default;
		BWindow(int width, int height);
		~BWindow();

		void* GetWindowHandle();


	private:
		void* handle;

	};
}