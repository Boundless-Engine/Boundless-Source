#pragma once

#include "pch.h"

#include "Boundless/Core.h"
using namespace Boundless;
using namespace Graphics;

#include "backends/imgui_impl_vulkan.h"

class SceneView : public Boundless::I::ILayer
{
public:
	SceneView(I::IGraphicsAPI* api) : api{api} {}
	~SceneView() {}

	virtual BReturn OnAttach() override {
		Rendering::CreateFrambufferRenderer(api, &renderer);
		renderer->Initillize();
		return SUCCESS;
	}
	virtual BReturn OnDetach() override {
		renderer->Shutdown();
		Rendering::DestroyFramebufferRenderer(&renderer);
		return SUCCESS;
	}

	virtual void OnRender() override {
		renderer->Render();
	}

	virtual void OnGUI() override
	{
		ImGui::Begin("Scene");
		ImTextureID sceneTexture = reinterpret_cast<ImTextureID>(renderer->GetRenderTexture());
		
		ImVec2 region = ImGui::GetContentRegionAvail();
		ImGui::Image(sceneTexture, region, { 0, 1 }, {1,  0});

		ImGui::End();
	}

private:
	I::IGraphicsAPI* api;
	I::IFramebufferRenderer* renderer;

};
