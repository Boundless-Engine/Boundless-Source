#pragma once


#include "IRenderer.h"
#include "IRasterSurface.h"
#include "VulkanSurface.h"
using namespace Boundless;

class FramebufferRenderer : public I::IRenderer
{
public:
	FramebufferRenderer(I::IRasterSurface* rasterSurface);
	~FramebufferRenderer();

	// Inherited via IRenderer
	virtual void OnEvent(I::IEvent* event) override;

	virtual void Initillize() override;

	virtual void Update(float ts) override;

	virtual void Render() override;

	VkDescriptorSet GetRenderTexture() { return renderTexture.descriptorSet; }

private:
	Graphics::Texture2D renderTexture;

};
