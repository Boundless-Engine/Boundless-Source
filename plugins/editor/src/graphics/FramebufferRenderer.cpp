#include "FramebufferRenderer.h"
#include "VulkanSurface.h"

#include "backends/imgui_impl_vulkan.h"

static Graphics::VulkanSurface* vk;

FramebufferRenderer::FramebufferRenderer(I::IRasterSurface* rasterSurface)
{
	vk = reinterpret_cast<Graphics::VulkanSurface*>(rasterSurface);
	//sint width = vk->Width(), height = vk->Height();
	
	int width = 2, height = 2;
	uint32_t pixels[4] = { 0xFFFFFFFF, 0xFF00FF00, 0xFF0000FF, 0x000000FF };

	// create the offscreen image, image view and memory
	vk->CreateTexture2D((void*)pixels, width, height, 8, 4, renderTexture);
	
	// create frambuffer for the offscreen image
	// create a command pool.


}

FramebufferRenderer::~FramebufferRenderer()
{
	//ImGui_ImplVulkan_RemoveTexture(texture);

	vk->DestroyTexture2D(renderTexture);
}

void FramebufferRenderer::OnEvent(I::IEvent* event)
{
}

void FramebufferRenderer::Initillize()
{
}

void FramebufferRenderer::Update(float ts)
{
}

// render happens between Begin/End Frame calls 
void FramebufferRenderer::Render()
{
	/*VkCommandBuffer cmd;
	VkCommandPool pool; vk->GetCommandPool(&pool);
	VkQueue graphicsQueue; vk->GetQueue(&graphicsQueue);

	vk->BeginSingleTimeCommand(pool, cmd);

	vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, );

	vk->EndSingleTimeCommand();*/

}
