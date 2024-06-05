#include "VulkanFramebufferRenderer.h"

#include "VulkanAPI.h"

#include "backends/imgui_impl_vulkan.h"

#include "Logger.h"
#include "Profiler.h"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>



namespace Boundless {
	namespace Graphics {
		namespace Renderers {

			namespace {

				uint32_t width;
				uint32_t height;
				VkRenderPass renderPass;
				VkFramebuffer framebuffer;
				Texture2D renderTexture;
				Texture2D depthTexture;
				VulkanAPI* vk;

				
				// geometry info
			

				/*static float max_axis_distance = 10000.0f;
				static float min_axis_distance = -max_axis_distance;*/

				//std::vector<Vertex::Axis> axis_vertices =
				//{
				//	{{0, 0, 0},			{1, 0, 0}}, // the origin 
				//	{{max_axis_distance, 0, 0},	{1, 0, 0}}, // x max 
				//	{{0, 0, 0},			{1, 0, 0}}, // the origin 
				//	{{min_axis_distance, 0, 0},	{1, 0, 0}}, // x min

				//	{{0, 0, 0},		{0, 1, 0}}, // the origin 
				//	{{0, max_axis_distance, 0},	{0, 1, 0}}, // y max 
				//	{{0, 0, 0},		{0, 1, 0}}, // the origin 
				//	{{0, min_axis_distance, 0},	{0, 1, 0}}, // y min 

				//	{{0, 0, 0},		{0, 0, 1}}, // the origin 
				//	{{0, 0, max_axis_distance},	{0, 0, 1}}, // z max
				//	{{0, 0, 0},		{0, 0, 1}}, // the origin
				//	{{0, 0, min_axis_distance},	{0, 0, 1}}, // z min
				//};

	

				//static glm::vec3 cameraPosition = { 0, 0, 5 };
				//glm::mat4 icoSphere = glm::mat4(1.0f);


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
	

			void* VulkanFramebufferRenderer::RenderPass() {
				return renderPass; 
			}

			void VulkanFramebufferRenderer::Initillize()
			{
				fn_profiler("FramebufferRenderer_Initillize");

				width = 1;
				height = 1;
				
				// create the offscreen image, image view and memory
				vk->CreateRenderTexture(width, height, Texture2D::MSAA::None, Texture2D::BitsPerPixel::BITS_8, Texture2D::ColorChannels::RGBA, renderTexture);
				vk->CreateDepthTexture(width, height, depthTexture);

				std::vector<Texture2D> colorAttachments = {
					renderTexture,
				};

				vk->CreateRenderPass(colorAttachments, depthTexture, &renderPass);

				std::vector<Texture2D> framebufferAttachments
				{
					renderTexture,
					depthTexture
				};


				vk->CreateFramebuffer(width, height, framebufferAttachments, renderPass, &framebuffer);

				//----------------------------------------------------
				// Create Render Pipeline 
				//----------------------------------------------------

				
				// create vertex buffer.

				/*VkDeviceSize axisBufferSize = sizeof(Vertex::Axis) * axis_vertices.size();
				vk->AllocateBuffer<BufferType::Vertex>(axisBufferSize, axisBuffer);
				vk->CopyToBuffer((void*)axis_vertices.data(), axisBufferSize, axisBuffer);*/

			

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
				vk->DestroyTexture2D(depthTexture);
			}


			VkCommandBuffer cmd = VK_NULL_HANDLE;
			VkQueue graphicsQueue = VK_NULL_HANDLE;
			VkCommandPool graphicsPool = VK_NULL_HANDLE;
			
			void* VulkanFramebufferRenderer::Begin() {
				
				if(graphicsQueue == VK_NULL_HANDLE)
					vk->GetQueue(&graphicsQueue);
				
				if(graphicsPool == VK_NULL_HANDLE)
					vk->GetCommandPool(&graphicsPool);
				
				
				vk->BeginSingleTimeCommand(graphicsPool, cmd);

				VkRenderPassBeginInfo renderPassInfo{};
				renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
				renderPassInfo.renderPass = renderPass;
				renderPassInfo.framebuffer = framebuffer;
				renderPassInfo.renderArea.offset = { 0, 0 };
				renderPassInfo.renderArea.extent = { (uint32_t)width, (uint32_t)height };

				std::vector<VkClearValue> clearValues =
				{
					{
						.color = {
							.float32 = {0.16f, 0.16f, 0.16f, 1.0f}
						}
					},
					{
						.depthStencil = {.depth = 1.0f }
					}
				};

				renderPassInfo.clearValueCount = (uint32_t)clearValues.size();
				renderPassInfo.pClearValues = clearValues.data();

				vkCmdBeginRenderPass(cmd, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

				return cmd;
			}

			void VulkanFramebufferRenderer::End() {


				vkCmdEndRenderPass(cmd);

				vk->EndSingleTimeCommand(graphicsPool, graphicsQueue, cmd);
			}

			int VulkanFramebufferRenderer::Width()
			{
				return width;
			}

			int VulkanFramebufferRenderer::Height()
			{
				return height;
			}
			
			
			void VulkanFramebufferRenderer::OnResize(int nwidth, int nheight)
			{
				width = nwidth;
				height = nheight;



				// Wait for the GPU to finish any operations on the old resources
				VkQueue graphicsQueue;
				vk->GetQueue(&graphicsQueue);
				vkQueueWaitIdle(graphicsQueue);

				Debug::Log("Resize VulkanFramebuffer: (%i, %i)", width, height);

				// Destroy the old framebuffer and render pass
				vk->DestroyFramebuffer(&framebuffer);
				vk->DestroyRenderPass(&renderPass);
				vk->DestroyTexture2D(renderTexture);
				vk->DestroyTexture2D(depthTexture);


				// create the offscreen image, image view and memory
				vk->CreateRenderTexture(width, height, Texture2D::MSAA::None, Texture2D::BitsPerPixel::BITS_8, Texture2D::ColorChannels::RGBA, renderTexture);
				vk->CreateDepthTexture(width, height, depthTexture);

				std::vector<Texture2D> colorAttachments = {
					renderTexture,
				};

				vk->CreateRenderPass(colorAttachments, depthTexture, &renderPass);

				std::vector<Texture2D> framebufferAttachments
				{
					renderTexture,
					depthTexture
				};


				vk->CreateFramebuffer(width, height, framebufferAttachments, renderPass, &framebuffer);

				VkCommandPool pool;
				vk->GetCommandPool(&pool);

				/*VkCommandBuffer cmd;
				vk->BeginSingleTimeCommand(pool, cmd);

				vk->EndSingleTimeCommand(pool, graphicsQueue, cmd);*/

			}

			void* VulkanFramebufferRenderer::GetRenderTexture()
			{
				return (void*)renderTexture.descriptorSet;
			}
		}

	}
}