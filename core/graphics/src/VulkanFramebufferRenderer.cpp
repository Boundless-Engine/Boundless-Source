#include "VulkanFramebufferRenderer.h"



#include "VulkanAPI.h"

#include "backends/imgui_impl_vulkan.h"

#include "Logger.h"
#include "Profiler.h"

#include "glm/glm.hpp"

namespace Boundless {
	namespace Graphics {
		namespace Renderers {

			namespace {

				static uint32_t width;
				static uint32_t height;
				static VkRenderPass renderPass;
				static VkFramebuffer framebuffer;
				static Texture2D renderTexture;
				static VulkanAPI* vk;


				Buffer vbuffer{};
				Buffer ibuffer{};
				VkPipeline graphicsPipeline;
				VkPipeline wireframePipeline;

				constexpr ShaderStage shaderStages[2] { ShaderStage::Vertex, ShaderStage::Fragment };
			}

			struct Vertex {
				glm::vec3 position;
				glm::vec3 color;
			};

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

				
				// create the offscreen image, image view and memory
				vk->CreateRenderTexture(width, height, Texture2D::MSAA::None, Texture2D::BitsPerPixel::BITS_8, Texture2D::ColorChannels::RGBA, renderTexture);

				vk->CreateRenderPass(renderTexture, &renderPass);

				vk->CreateFramebuffer(width, height, renderTexture, renderPass, &framebuffer);

				// create vertex buffer.
				
				std::vector<Vertex> vertices = {
					{{-0.5f, -0.5f, 0.0f}, {1, 0, 0}},		// bottom left 
					{{ 0.5f, -0.5f, 0.0f}, {0, 1, 0}},		// bottom right
					{{ 0.5f,  0.5f, 0.0f}, {0, 0, 1}},		// top right
					{{-0.5f,  0.5f, 0.0f}, {1, 1, 1}},		// top left
				};

				VkDeviceSize vBufferSize = sizeof(Vertex) * vertices.size();
				vk->AllocateBuffer<BufferType::Vertex>(vBufferSize, vbuffer);
				vk->CopyToBuffer((void*)vertices.data(), vBufferSize, vbuffer);
				
				// create index buffer
				std::vector<uint16_t> indices = {
					0, 1, 2, 2, 3, 0
				};

				VkDeviceSize iBufferSize = sizeof(uint16_t) * indices.size();
				vk->AllocateBuffer<BufferType::Index>(iBufferSize, ibuffer);
				vk->CopyToBuffer((void*)indices.data(), iBufferSize, ibuffer);

				std::vector<VkShaderModule> basicShader;
				
				vk->CompileShader("assets/shaders/basic.glsl", basicShader, _countof(shaderStages), shaderStages);

				// Create Pipeline Layout
				VkPipelineLayout layout;
				vk->CreatePipelineLayout(&layout);

				// Create GraphicsPipeline 
				
				// pipeline state.
				std::vector<VkPipelineShaderStageCreateInfo> stages = {
						{
							.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
							.pNext = nullptr,
							.flags = 0,
							.stage = VK_SHADER_STAGE_VERTEX_BIT,
							.module = basicShader[0],
							.pName = "main",
							.pSpecializationInfo = nullptr
						},
						{
							.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
							.pNext = nullptr,
							.flags = 0,
							.stage = VK_SHADER_STAGE_FRAGMENT_BIT,
							.module = basicShader[1],
							.pName = "main",
							.pSpecializationInfo = nullptr
						}
				};
				std::vector<VkVertexInputBindingDescription> bindings = {
					{
						.binding		= 0,
						.stride			= sizeof(Vertex),
						.inputRate		= VK_VERTEX_INPUT_RATE_VERTEX
					}
				};
				std::vector<VkVertexInputAttributeDescription> attributes = {
					{
						.location = 0,
						.binding = 0,
						.format = VK_FORMAT_R32G32B32_SFLOAT,
						.offset = offsetof(Vertex, position)
					},
					{
						.location = 1,
						.binding=0,
						.format = VK_FORMAT_R32G32B32_SFLOAT,
						.offset= offsetof(Vertex, color)
					}
				};

				VkPipelineVertexInputStateCreateInfo	vertexInputState {
					.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
					.pNext = nullptr,
					.flags = 0,
					.vertexBindingDescriptionCount = (uint32_t)bindings.size(),
					.pVertexBindingDescriptions = bindings.data(),
					.vertexAttributeDescriptionCount = (uint32_t)attributes.size(),
					.pVertexAttributeDescriptions = attributes.data(),
				};
				VkPipelineInputAssemblyStateCreateInfo	inputAssemblyState {
					.sType					= VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
					.pNext					= nullptr,
					.flags					= 0,
					.topology				= VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
					.primitiveRestartEnable = VK_FALSE
				};
				VkPipelineTessellationStateCreateInfo	tessellationState {
					.sType = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO,
					.pNext = nullptr,
					.flags = 0,
					.patchControlPoints = 0
				};
				
				std::vector<VkRect2D> scissors		= {
					{ 
						.offset = { .x = 0, .y = 0}, 
						.extent = {.width = width, .height = height} }
				};

				std::vector<VkViewport> viewports	= {
					{
						.x			= 0,
						.y			= 0,
						.width		= (float) width,
						.height		= (float) height,
						.minDepth	= 0,
						.maxDepth	= 1.0f
					}
				};

				VkPipelineViewportStateCreateInfo		viewportState {
					.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
					.pNext = nullptr,
					.flags = 0,
					.viewportCount = (uint32_t) viewports.size(),
					.pViewports = viewports.data(),
					.scissorCount = (uint32_t) scissors.size(),
					.pScissors = scissors.data(),
				};
				VkPipelineRasterizationStateCreateInfo	rasterizationState {
					.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
					.pNext = nullptr,
					.flags = 0,
					.depthClampEnable = VK_FALSE,
					.rasterizerDiscardEnable = VK_FALSE,
					.polygonMode = VK_POLYGON_MODE_FILL,
					.cullMode = VK_CULL_MODE_BACK_BIT,
					.frontFace = VK_FRONT_FACE_CLOCKWISE,
					.depthBiasEnable = VK_FALSE,
					.depthBiasConstantFactor = 0,
					.depthBiasClamp = 0,
					.depthBiasSlopeFactor = 0,
					.lineWidth = 1.0f
				};
				VkPipelineMultisampleStateCreateInfo	multisampleState{
					.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
					.pNext = nullptr,
					.flags = 0,
					.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
					.sampleShadingEnable = VK_FALSE,
					.minSampleShading = 0,
					.pSampleMask = nullptr,
					.alphaToCoverageEnable = VK_FALSE,
					.alphaToOneEnable = VK_FALSE
				};
				VkPipelineDepthStencilStateCreateInfo	depthStencilState{
					.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
					.pNext = nullptr,
					.flags = 0,
					.depthTestEnable = VK_FALSE,
					.depthWriteEnable = VK_FALSE,
					.depthCompareOp = VK_COMPARE_OP_GREATER_OR_EQUAL,
					.depthBoundsTestEnable = VK_FALSE,
					.stencilTestEnable = VK_FALSE,
					.minDepthBounds = 0.1f,
					.maxDepthBounds = 1.0f,
				};
				VkPipelineColorBlendAttachmentState		blender {
					.blendEnable = VK_TRUE,
					.srcColorBlendFactor = VK_BLEND_FACTOR_ONE,
					.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO,
					.colorBlendOp = VK_BLEND_OP_ADD,
					.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
					.dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA,
					.alphaBlendOp = VK_BLEND_OP_ADD,
					.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT,
				};
				VkPipelineColorBlendStateCreateInfo		colorBlendState{
					.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
					.pNext = nullptr,
					.flags = 0,
					.logicOpEnable = VK_FALSE,
					.logicOp = VK_LOGIC_OP_AND,
					.attachmentCount = 1, 
					.pAttachments = &blender
				};

				std::vector<VkDynamicState> states
				{
						VK_DYNAMIC_STATE_VIEWPORT ,
						VK_DYNAMIC_STATE_SCISSOR ,
						VK_DYNAMIC_STATE_LINE_WIDTH ,
						VK_DYNAMIC_STATE_DEPTH_BIAS ,
						VK_DYNAMIC_STATE_BLEND_CONSTANTS ,
						VK_DYNAMIC_STATE_DEPTH_BOUNDS ,
						VK_DYNAMIC_STATE_STENCIL_COMPARE_MASK ,
						VK_DYNAMIC_STATE_STENCIL_WRITE_MASK ,
						VK_DYNAMIC_STATE_STENCIL_REFERENCE ,
						VK_DYNAMIC_STATE_CULL_MODE ,
						VK_DYNAMIC_STATE_FRONT_FACE ,
						VK_DYNAMIC_STATE_PRIMITIVE_TOPOLOGY ,
						VK_DYNAMIC_STATE_VIEWPORT_WITH_COUNT ,
						VK_DYNAMIC_STATE_SCISSOR_WITH_COUNT ,
						VK_DYNAMIC_STATE_VERTEX_INPUT_BINDING_STRIDE ,
						VK_DYNAMIC_STATE_DEPTH_TEST_ENABLE ,
						VK_DYNAMIC_STATE_DEPTH_WRITE_ENABLE ,
						VK_DYNAMIC_STATE_DEPTH_COMPARE_OP ,
						VK_DYNAMIC_STATE_DEPTH_BOUNDS_TEST_ENABLE ,
						VK_DYNAMIC_STATE_STENCIL_TEST_ENABLE ,
						VK_DYNAMIC_STATE_STENCIL_OP ,
						VK_DYNAMIC_STATE_RASTERIZER_DISCARD_ENABLE ,
						VK_DYNAMIC_STATE_DEPTH_BIAS_ENABLE ,
						VK_DYNAMIC_STATE_PRIMITIVE_RESTART_ENABLE ,
						VK_DYNAMIC_STATE_VIEWPORT_W_SCALING_NV ,
						VK_DYNAMIC_STATE_DISCARD_RECTANGLE_EXT ,
						VK_DYNAMIC_STATE_SAMPLE_LOCATIONS_EXT ,
						VK_DYNAMIC_STATE_RAY_TRACING_PIPELINE_STACK_SIZE_KHR ,
						VK_DYNAMIC_STATE_VIEWPORT_SHADING_RATE_PALETTE_NV ,
						VK_DYNAMIC_STATE_VIEWPORT_COARSE_SAMPLE_ORDER_NV ,
						VK_DYNAMIC_STATE_EXCLUSIVE_SCISSOR_NV ,
						VK_DYNAMIC_STATE_FRAGMENT_SHADING_RATE_KHR ,
						VK_DYNAMIC_STATE_LINE_STIPPLE_EXT ,
						VK_DYNAMIC_STATE_VERTEX_INPUT_EXT ,
						VK_DYNAMIC_STATE_PATCH_CONTROL_POINTS_EXT ,
						VK_DYNAMIC_STATE_LOGIC_OP_EXT ,
				};

				VkPipelineDynamicStateCreateInfo		dynamicState{
					.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
					.pNext = nullptr, 
					.flags = 0,
					.dynamicStateCount = (uint32_t)states.size(),
					.pDynamicStates = states.data(),
				};

				vk->CreateGraphicsPipeline(stages.size(), stages.data(), &vertexInputState, &inputAssemblyState, &tessellationState, &viewportState, &rasterizationState, &multisampleState, &depthStencilState, &colorBlendState, &dynamicState, layout, renderPass, 0, nullptr, 0, &graphicsPipeline);
				stages.clear();

				std::vector<VkShaderModule> wireframeShader;

				vk->CompileShader("assets/shaders/wireframe.glsl", wireframeShader, _countof(shaderStages), shaderStages);

				bindings = {
					{
						.binding = 0,
						.stride = sizeof(Vertex),
						.inputRate = VK_VERTEX_INPUT_RATE_VERTEX
					}
				};

				attributes = {
					{
						.location = 0,
						.binding = 0,
						.format = VK_FORMAT_R32G32B32_SFLOAT,
						.offset = offsetof(Vertex, position)
					},
				};

				stages = {
						{
							.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
							.pNext = nullptr,
							.flags = 0,
							.stage = VK_SHADER_STAGE_VERTEX_BIT,
							.module = wireframeShader[0],
							.pName = "main",
							.pSpecializationInfo = nullptr
						},
						{
							.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
							.pNext = nullptr,
							.flags = 0,
							.stage = VK_SHADER_STAGE_FRAGMENT_BIT,
							.module = wireframeShader[1],
							.pName = "main",
							.pSpecializationInfo = nullptr
						}
				};

				rasterizationState.polygonMode = VK_POLYGON_MODE_LINE;
				rasterizationState.cullMode = VK_CULL_MODE_NONE;

				vk->CreateGraphicsPipeline(stages.size(), stages.data(), &vertexInputState, &inputAssemblyState, &tessellationState, &viewportState, &rasterizationState, &multisampleState, &depthStencilState, &colorBlendState, &dynamicState, layout, renderPass, 0, nullptr, 0, &wireframePipeline);
				
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

				// Draw Geometry
				{
					// Record drawing commands here
					vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);
					VkDeviceSize offset = 0;
					vkCmdBindVertexBuffers(cmd, 0, 1, &vbuffer.buffer, &offset);
					vkCmdBindIndexBuffer(cmd, ibuffer.buffer, 0, VK_INDEX_TYPE_UINT16);
				
					vkCmdDrawIndexed(cmd, 6, 1, 0, 0, 0);
				}
				
				// Draw Wireframe over geometry
				{
					vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, wireframePipeline);
					VkDeviceSize offset = 0;
					vkCmdBindVertexBuffers(cmd, 0, 1, &vbuffer.buffer, &offset);
					vkCmdBindIndexBuffer(cmd, ibuffer.buffer, 0, VK_INDEX_TYPE_UINT16);

					vkCmdDrawIndexed(cmd, 6, 1, 0, 0, 0);
				}
				
				vkCmdEndRenderPass(cmd);

				vk->EndSingleTimeCommand(graphicsPool, graphicsQueue, cmd);

			}

			int VulkanFramebufferRenderer::Width()
			{
				return renderTexture.width;
			}

			int VulkanFramebufferRenderer::Height()
			{
				return renderTexture.height;
			}

			void VulkanFramebufferRenderer::OnResize(int nwidth, int nheight)
			{
				width = nwidth;
				height = nheight;

				Debug::Log("Resize VulkanFramebuffer: (%i, %i)", width, height);

				// Wait for the GPU to finish any operations on the old resources
				VkQueue graphicsQueue;
				vk->GetQueue(&graphicsQueue);
				vkQueueWaitIdle(graphicsQueue);

				// Destroy the old framebuffer and render pass
				vk->DestroyFramebuffer(&framebuffer);
				vk->DestroyRenderPass(&renderPass);
				vk->DestroyTexture2D(renderTexture);

				// Create the new texture with the new dimensions
				vk->CreateRenderTexture(width, height, Texture2D::MSAA::None, Texture2D::BitsPerPixel::BITS_8, Texture2D::ColorChannels::RGBA, renderTexture);

				// Create the new render pass
				vk->CreateRenderPass(renderTexture, &renderPass);

				// Create the new framebuffer with the new dimensions
				vk->CreateFramebuffer(width, height, renderTexture, renderPass, &framebuffer);
			}

			void* VulkanFramebufferRenderer::GetRenderTexture()
			{
				return (void*)renderTexture.descriptorSet;
			}
		}

	}
}