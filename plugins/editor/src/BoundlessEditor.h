#pragma once

#include <string>

#define ENABLE_VULKAN
#include "boundless/core.h"

//! this is fine..
using namespace Boundless;
// using namespace I no!


class BoundlessEditor : public I::IApplication {
public:
	BoundlessEditor() : I::IApplication("config/editor.txt") { if (_instance != nullptr) return;  _instance = this; Initillize(); }
	~BoundlessEditor() { Shutdown(); };
	
	BReturn Initillize() override;
	BReturn Shutdown() override;
	BReturn Run() override;

	void Close();
	std::function<void()> Menubar() override;

	virtual void OnEvent(I::IEvent* event) override;
	
private:
	void SaveDialogue(std::function<void(bool)> closeFunc);
	void ShowPreferences();
	void ShowStyleEditor();

private:
	bool show_preferences = false;
	bool application_should_run = true;
	bool show_save_popup = false;
	Graphics::GuiTheme* theme;

	static BoundlessEditor* _instance;
	//static std::function<void()> menu_thunk();

};