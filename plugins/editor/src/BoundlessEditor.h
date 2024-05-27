#pragma once

#include <string>

#define ENABLE_VULKAN
#include "boundless/core.h"

//! this is fine..
using namespace Boundless;
// using namespace I no!


class BoundlessEditor : public I::IApplication {
public:
	BoundlessEditor() : I::IApplication("config/editor.txt") { Initillize(); }
	~BoundlessEditor() { Shutdown(); };
	
	BReturn Initillize() override;
	BReturn Shutdown() override;
	BReturn Run() override;

	void Close();

	virtual void OnEvent(I::IEvent* event) override;
	
	void SaveDialogue(std::function<void(bool)> closeFunc);

private:
	bool application_should_run = true;
	

};