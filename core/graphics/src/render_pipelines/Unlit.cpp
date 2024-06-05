#include "Unlit.h"

#include "VulkanAPI.h"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


namespace Boundless {
	namespace Graphics {
		namespace Rendering {
			namespace RenderPipelines {
				namespace {
					struct Vertex {
						glm::vec3 position;
					};

					struct PushConstants {
						glm::mat4 model;
						glm::vec4 color;
					};

					struct UBO {
						glm::mat4 viewMatrix;
						glm::mat4 projectionMatrix;
					};

					VulkanAPI* vk;
					VulkanBuffer ubuffer{};
					VkDescriptorPool descriptorPool;
					VkDescriptorSet uniformBufferObject;
					VkPipelineLayout pipelineLayout;
					VkPipeline graphicsPipeline;
					VkRenderPass rpass;
					uint32_t width;
					uint32_t height;

					constexpr ShaderStage shaderStages[2]{ ShaderStage::Vertex, ShaderStage::Fragment };
				}
				Unlit::Unlit(I::IGraphicsAPI* pAPI, void* renderPass)
				{

					vk = reinterpret_cast<Graphics::VulkanAPI*>(pAPI);

					rpass = reinterpret_cast<VkRenderPass>(renderPass);

					width = 1;
					height = 1;

					VkDeviceSize ubufferSize = sizeof(UBO);
					vk->AllocateBuffer<BufferType::Uniform>(ubufferSize, ubuffer);

					std::vector<VkDescriptorSetLayout> setLayouts;

					VkDescriptorSetLayout setLayout;

					std::vector<VkDescriptorSetLayoutBinding> setBindings = {
						// UBO {Model Matrix, View Matrix, Projection Matrix}
						{
							.binding = 0,
							.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
							.descriptorCount = 1,
							.stageFlags = VK_SHADER_STAGE_VERTEX_BIT,
						},
					};

					vk->CreateDescriptorSetLayout(setBindings, &setLayout);

					setLayouts.push_back(setLayout);

					std::vector<VkPushConstantRange> pushConstants
					{
						{
							.stageFlags = VK_SHADER_STAGE_VERTEX_BIT,
							.offset = 0,
							.size = sizeof(PushConstants),
						}
					};

					vk->CreatePipelineLayout(setLayouts, pushConstants, &pipelineLayout);

					// Create Descriptor Pool
					std::vector<VkDescriptorPoolSize> pool_sizes =
					{
						{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,		 1 },
					};

					vk->CreateDescriptorPool(1, pool_sizes, &descriptorPool);
					vk->AllocateDescriptorSet(1, setLayout, descriptorPool, &uniformBufferObject);

					// default pipeline state.
#pragma region DEFAULT_PIPELINE_STATE

					std::vector<VkShaderModule> wireframeShader;
					vk->CompileShader("assets/shaders/unlit.glsl", wireframeShader, _countof(shaderStages), shaderStages);


					std::vector<VkPipelineShaderStageCreateInfo> stages;
					std::vector<VkVertexInputBindingDescription> bindings;
					std::vector<VkVertexInputAttributeDescription> attributes;


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


					std::vector<VkRect2D> scissors = {
						{
							.offset = {.x = 0, .y = 0},
							.extent = {.width = width, .height = height} }
					};

					std::vector<VkViewport> viewports = {
						{
							.x = 0,
							.y = 0,
							.width = (float)width,
							.height = (float)height,
							.minDepth = 0,
							.maxDepth = 1.0f
						}
					};


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
						}
					};

					VkPipelineVertexInputStateCreateInfo	vertexInputState{
					.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
					.pNext = nullptr,
					.flags = 0,
					.vertexBindingDescriptionCount = (uint32_t)bindings.size(),
					.pVertexBindingDescriptions = bindings.data(),
					.vertexAttributeDescriptionCount = (uint32_t)attributes.size(),
					.pVertexAttributeDescriptions = attributes.data(),
					};
					VkPipelineInputAssemblyStateCreateInfo	inputAssemblyState{
						.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
						.pNext = nullptr,
						.flags = 0,
						.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
						.primitiveRestartEnable = VK_FALSE
					};
					VkPipelineTessellationStateCreateInfo	tessellationState{
						.sType = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO,
						.pNext = nullptr,
						.flags = 0,
						.patchControlPoints = 0
					};
					VkPipelineViewportStateCreateInfo		viewportState{
						.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
						.pNext = nullptr,
						.flags = 0,
						.viewportCount = (uint32_t)viewports.size(),
						.pViewports = viewports.data(),
						.scissorCount = (uint32_t)scissors.size(),
						.pScissors = scissors.data(),
					};
					VkPipelineRasterizationStateCreateInfo	rasterizationState{
						.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
						.pNext = nullptr,
						.flags = 0,
						.depthClampEnable = VK_FALSE,
						.rasterizerDiscardEnable = VK_FALSE,
						.polygonMode = VK_POLYGON_MODE_FILL,
						.cullMode = VK_CULL_MODE_NONE,
						.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE,
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
						.depthTestEnable = VK_TRUE,
						.depthWriteEnable = VK_TRUE,
						.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL,
						.depthBoundsTestEnable = VK_FALSE,
						.stencilTestEnable = VK_FALSE,
						.minDepthBounds = 0.1f,
						.maxDepthBounds = 1.0f,
					};
					VkPipelineColorBlendAttachmentState blender{
						.blendEnable = VK_TRUE,
						.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA,
						.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
						.colorBlendOp = VK_BLEND_OP_ADD,
						.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
						.dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
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
							/*VK_DYNAMIC_STATE_DEPTH_BIAS ,
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
							VK_DYNAMIC_STATE_LOGIC_OP_EXT ,*/
					};

					VkPipelineDynamicStateCreateInfo dynamicState{
						.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
						.pNext = nullptr,
						.flags = 0,
						.dynamicStateCount = (uint32_t)states.size(),
						.pDynamicStates = states.data(),
					};

#pragma endregion

					vk->CreateGraphicsPipeline(stages.size(), stages.data(), &vertexInputState, &inputAssemblyState, &tessellationState, &viewportState, &rasterizationState, &multisampleState, &depthStencilState, &colorBlendState, &dynamicState, pipelineLayout, rpass, 0, nullptr, 0, &graphicsPipeline);

				}


				Unlit::~Unlit()
				{
					vk->DestroyDescriptorPool(&descriptorPool);
					vk->FreeBuffer(ubuffer);
					vk->DestroyPipeline(&graphicsPipeline);
				}

				void Unlit::OnUpdateUniforms(void* data)
				{
					UBO ubo = *static_cast<UBO*>(data);

					vk->CopyToBuffer(&ubo, sizeof(UBO), ubuffer);

					vk->UpdateUniforms(sizeof(UBO), ubuffer.buffer, 0, uniformBufferObject);
				}

				void Unlit::OnResize(uint32_t nwidth, uint32_t nheight)
				{
					width = nwidth;
					height = nheight;
				}

				void Unlit::OnBind(void* pCmdBuffer)
				{
					VkCommandBuffer cmd = reinterpret_cast<VkCommandBuffer>(pCmdBuffer);
					vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);
					vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &uniformBufferObject, 0, nullptr);

				}

				void Unlit::OnPushConstants(void* pCmd, void* pConstants)
				{
					VkCommandBuffer cmd = reinterpret_cast<VkCommandBuffer>(pCmd);
					PushConstants constants = *(reinterpret_cast<PushConstants*>(pConstants));

					vkCmdPushConstants(cmd, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(PushConstants), &constants);
				}
			}
		}
	}

}
