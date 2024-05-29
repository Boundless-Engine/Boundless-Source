#pragma once

#include "pch.h"

#include "Boundless/Core.h"
using namespace Boundless;

#include "graphics/FramebufferRenderer.h"
#include "backends/imgui_impl_vulkan.h"

class SceneView : public Boundless::I::ILayer
{
public:
	SceneView(I::IRasterSurface* pRasterSurface) : rasterSurface{pRasterSurface} {}
	~SceneView() {}

	virtual BReturn OnAttach() override {
		renderer = new FramebufferRenderer(rasterSurface);
		return SUCCESS;
	}
	virtual BReturn OnDetach() override {
		delete renderer;
		return SUCCESS;
	}

	virtual void OnRender() override {
		renderer->Render();
	}

	virtual void OnGUI() override
	{
		ImGui::Begin("Scene");
		ImTextureID sceneTexture = reinterpret_cast<ImTextureID>(renderer->GetRenderTexture());

		ImVec2 region = ImGui::GetContentRegionMax();
		ImGui::Image(sceneTexture, region, { 0, 0 }, {1, 0});

		ImGui::End();
	}

private:
	I::IRasterSurface* rasterSurface;
	FramebufferRenderer* renderer;

};
