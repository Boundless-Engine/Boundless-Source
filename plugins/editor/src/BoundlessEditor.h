#pragma once

#include "pch.h"

#define ENABLE_VULKAN
#include "boundless/core.h"

//! this is fine..
using namespace Boundless;
// using namespace I no!


struct BoundlessData : public I::IMetaData 
{
	std::string theme;
};


class BoundlessEditor : public I::IApplication, public I::IMetaDataManager<BoundlessData> {
public:
	BoundlessEditor() : 
		I::IApplication("config/editor.txt"), 
		I::IMetaDataManager<BoundlessData>("boundless.meta") 
	{ 
		if (_instance != nullptr) return;  
		_instance = this; Initillize(); 
	}
	~BoundlessEditor() { Shutdown(); };
	
	BReturn Initillize() override;
	BReturn Shutdown() override;
	BReturn Run() override;

	BReturn Serialize() override;
	BReturn Deserialize() override;

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

	const std::string metaFileLocation = "boundless.txt";

private:
	static BoundlessEditor* _instance;

};