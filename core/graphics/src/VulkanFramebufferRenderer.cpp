#include "VulkanFramebufferRenderer.h"



#include "VulkanAPI.h"

#include "backends/imgui_impl_vulkan.h"

#include "Logger.h"
#include "Profiler.h"


namespace Boundless {
	namespace Graphics {
		namespace Renderers {

			namespace {

				static int width;
				static int height;
				static VkRenderPass renderPass;
				static VkFramebuffer framebuffer;
				static Texture2D renderTexture;
				static VulkanAPI* vk;
			}

			VulkanFramebufferRenderer::VulkanFramebufferRenderer(I::IGraphicsAPI* pAPI)
			{
				fn_profiler("FramebufferRenderer_CTOR");
				vk = reinterpret_cast<Graphics::VulkanAPI*>(pAPI);
			}

			VulkanFramebufferRenderer::~VulkanFramebufferRenderer()
			{
				fn_profiler("FramebufferRenderer_DTOR");
			}

			void VulkanFramebufferRenderer::OnEvent(I::IEvent* event)
			{
				fn_profiler("FramebufferRenderer_OnEvent");
			}

			void VulkanFramebufferRenderer::Initillize()
			{
				fn_profiler("FramebufferRenderer_Initillize");

				width = vk->Width();
				height = vk->Height();

				uint32_t* dummy_data = new uint32_t[width * height]{ 0x000000FF };

				for (int i = 0; i < width * height; i++) dummy_data[i] = 0xFF000000;

				// create the offscreen image, image view and memory
				vk->CreateTexture2D((void*)dummy_data, width, height, Texture2D::MSAA::None, Texture2D::BitsPerPixel::BITS_8, Texture2D::ColorChannels::RGBA, renderTexture);

				delete[] dummy_data;


				vk->CreateRenderPass(renderTexture, &renderPass);

				vk->CreateFramebuffer(width, height, renderTexture, renderPass, &framebuffer);

			}

			void VulkanFramebufferRenderer::Shutdown()
			{
				fn_profiler("FramebufferRenderer_Shutdown");

				//ImGui_ImplVulkan_RemoveTexture(texture);
				VkQueue graphicsQueue;
				vk->GetQueue(&graphicsQueue);
				vkQueueWaitIdle(graphicsQueue);

				vk->DestroyRenderPass(&renderPass);
				vk->DestroyFramebuffer(&framebuffer);


				vk->DestroyTexture2D(renderTexture);
			}

			void VulkanFramebufferRenderer::Update(float ts)
			{
				//fn_profiler("FramebufferRenderer_Update");

			}

			// render happens between Begin/End Frame calls 
			void VulkanFramebufferRenderer::Render()
			{
				//fn_profiler("FramebufferRenderer_Render");

				VkQueue graphicsQueue;
				vk->GetQueue(&graphicsQueue);
				VkCommandPool graphicsPool;
				vk->GetCommandPool(&graphicsPool);
				VkCommandBuffer cmd;
				vk->BeginSingleTimeCommand(graphicsPool, cmd);

				VkRenderPassBeginInfo renderPassInfo{};
				renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
				renderPassInfo.renderPass = renderPass;
				renderPassInfo.framebuffer = framebuffer;
				renderPassInfo.renderArea.offset = { 0, 0 };
				renderPassInfo.renderArea.extent = { (uint32_t)width, (uint32_t)height };

				VkClearValue clearColor = { {{0.16f, 0.16f, 0.16f, 1.0f}} };
				renderPassInfo.clearValueCount = 1;
				renderPassInfo.pClearValues = &clearColor;

				vkCmdBeginRenderPass(cmd, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

				// Record drawing commands here


				vkCmdEndRenderPass(cmd);

				vk->EndSingleTimeCommand(graphicsPool, graphicsQueue, cmd);

			}

			void* VulkanFramebufferRenderer::GetRenderTexture()
			{
				return (void*)renderTexture.descriptorSet;
			}
		}

	}
}