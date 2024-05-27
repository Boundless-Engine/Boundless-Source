#pragma once


#include "pch.h"

#include "Boundless/Core.h"


using namespace Boundless;

class Console : public Boundless::I::ILayer
{
public:
	Console() {}
	~Console() {}

	virtual BReturn OnAttach() override {
		return SUCCESS;
	}
	virtual BReturn OnDetach() override {
		return SUCCESS;
	}

	virtual void OnGUI() override
	{
		ImGui::Begin("Console");


		ImGui::End();
	}
};
