#pragma once

#include "pch.h"
#include "IGraphicsAPI.h"

#include <vector>
#include <functional>


#include <stdio.h>          // printf, fprintf
#include <stdlib.h>         // abort

#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>



namespace Boundless {

	namespace Graphics {
		void check_vk_result(VkResult err);
		
		void operator +(const VkResult res);

		struct Texture2D {
			enum class BitsPerPixel {
				BITS_8 = 0x08,
				BITS_16 = 0x10,
				BITS_32 = 0x20
			};

			enum class ColorChannels {
				R = 1,
				RG = 2,
				RGB = 3,
				RGBA = 4,
				
				D,
			};


			enum class MSAA {
				None,
				x2,
				x4,
				x8,
				x16,
				x32,
				x64
			};

			VkImage image;
			VkDeviceMemory memory;
			VkImageView view;
			
			VkSampler sampler;
			VkDescriptorSet descriptorSet;
			
			VkSampleCountFlagBits samples;
			VkFormat format;

			uint32_t width;
			uint32_t height;

			BitsPerPixel bits;
			ColorChannels channels;

			static VkFormat ConvertFormat(BitsPerPixel b, ColorChannels c) {
				if (b == BitsPerPixel::BITS_8) {
					switch (c) {
					case ColorChannels::R: return VK_FORMAT_R8_UNORM;
					case ColorChannels::RG: return VK_FORMAT_R8G8_UNORM;
					case ColorChannels::RGB: return VK_FORMAT_R8G8B8_UNORM;
					case ColorChannels::RGBA: return VK_FORMAT_R8G8B8A8_UNORM;
					default: throw std::runtime_error("Format Unrecognized!");
					}
				}
				else if (b == BitsPerPixel::BITS_16) {
					switch (c) {
					case ColorChannels::R: return VK_FORMAT_R16_UNORM;
					case ColorChannels::RG: return VK_FORMAT_R16G16_UNORM;
					case ColorChannels::RGB: return VK_FORMAT_R16G16B16_UNORM;
					case ColorChannels::RGBA: return VK_FORMAT_R16G16B16A16_UNORM;
					default: throw std::runtime_error("Format Unrecognized!");
					}
				}
				else if (b ==  BitsPerPixel::BITS_32) {
					switch (c) {
					case ColorChannels::R: return VK_FORMAT_R32_SFLOAT;
					case ColorChannels::RG: return VK_FORMAT_R32G32_SFLOAT;
					case ColorChannels::RGB: return VK_FORMAT_R32G32B32_SFLOAT;
					case ColorChannels::RGBA: return VK_FORMAT_R32G32B32A32_SFLOAT;
					case ColorChannels::D: return VK_FORMAT_D32_SFLOAT;  // This is correctly identified as a depth format
					default: throw std::runtime_error("Format Unrecognized!");
					}
				}
				else {
					throw std::runtime_error("Format Unrecognized!");
				}
			}

			static VkSampleCountFlagBits ConvertSampleRate(MSAA r) {
				
				if (r == MSAA::None)
					return VK_SAMPLE_COUNT_1_BIT;
				else if (r == MSAA::x2)
					return VK_SAMPLE_COUNT_2_BIT;
				else if (r == MSAA::x4)
					return VK_SAMPLE_COUNT_4_BIT;
				else if (r == MSAA::x8)
					return VK_SAMPLE_COUNT_8_BIT;
				else if (r == MSAA::x16)
					return VK_SAMPLE_COUNT_16_BIT;
				else if (r == MSAA::x32)
					return VK_SAMPLE_COUNT_32_BIT;
				else if (r == MSAA::x64)
					return VK_SAMPLE_COUNT_64_BIT;
				else
					throw std::runtime_error("unknow msaa format selected.");

			}
			

			VkDeviceSize Size() {
				return (width * height) * (((int)bits / 8) * (int)channels);
			}
		};

		struct VulkanBuffer {
			VkBuffer buffer;
			VkDeviceMemory memory;
			VkDeviceSize size;
		};

		enum class BufferType {
			Staging,
			Vertex,
			Index,
			Uniform,
			Constant,
		};

		enum class ShaderStage {
			Vertex,
			Fragment,
			Geometry,
			Tesselation
		};

		class VulkanAPI 
			: public I::IGraphicsAPI
		{
#ifdef _DEBUG
#define VULKAN_DEBUG_REPORT
#endif

#ifdef VULKAN_DEBUG_REPORT
			static VKAPI_ATTR VkBool32 VKAPI_CALL debug_report(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objectType, uint64_t object, size_t location, int32_t messageCode, const char* pLayerPrefix, const char* pMessage, void* pUserData)
			{
				(void)flags; (void)object; (void)location; /*(void)messageCode;*/ (void)pUserData; (void)pLayerPrefix; // Unused arguments
				fprintf(stderr, "\n[vulkan-deb] ObjectType:%i\nMessage(%i):\n\t%s\n\n", objectType, messageCode, pMessage);
				return VK_FALSE;
			}
#endif // IMGUI_VULKAN_DEBUG_REPORT


		public:
			VulkanAPI(I::IWindow* pWindow);
			~VulkanAPI();


			BReturn GetInstance(VkInstance* pInstance);
			BReturn GetAllocator(VkAllocationCallbacks* pAllocatorCallback);
			
			BReturn GetDevice(VkDevice* pDevice);
			BReturn GetPhysicalDevice(VkPhysicalDevice* pPhysicalDevice);

			BReturn GetQueueFamily(uint32_t* pQueueFamily);

			BReturn GetCommandBuffer(std::vector<VkCommandBuffer>& buffers, int idx);

			BReturn GetCommandPool(VkCommandPool* pPool);
			BReturn GetQueue(VkQueue* pQueue);
			BReturn GetSurface(VkSurfaceKHR* pSurface);

			//BReturn GetDescriptorPool(VkDescriptorPool* pDescriptorPool);

			BReturn CreateDescriptorPool(uint32_t maxSets, std::vector<VkDescriptorPoolSize>& sizes, VkDescriptorPool* pDescriptorPool);
			BReturn DestroyDescriptorPool(VkDescriptorPool* pDescriptorPool);

			BReturn CreateDescriptorSetLayout(std::vector<VkDescriptorSetLayoutBinding>& bindings, VkDescriptorSetLayout* pDescriptorSetLayout);

			BReturn AllocateDescriptorSet(uint32_t count, VkDescriptorSetLayout layout, VkDescriptorPool& pool, VkDescriptorSet* pDescriptorSet);

			BReturn GetPipelineCache(VkPipelineCache* pipelineCache);


			// RESOURCES
			BReturn CreateRenderTexture(uint32_t width, uint32_t height, Texture2D::MSAA samples, Texture2D::BitsPerPixel bits, Texture2D::ColorChannels channels, Texture2D& image);
			BReturn CreateDepthTexture(uint32_t width, uint32_t height, Texture2D& texture);

			BReturn CreateTexture2D(void* data, uint32_t width, uint32_t height, Texture2D::MSAA samples, Texture2D::BitsPerPixel bits, Texture2D::ColorChannels channels, Texture2D& image);
			BReturn DestroyTexture2D(Texture2D& image);

			BReturn CreateRenderPass(std::vector<Texture2D>& colorAttachments, VkRenderPass* pRenderPass);
			BReturn CreateRenderPass(std::vector<Texture2D>& colorAttachments, Texture2D& depthAttachment, VkRenderPass* pRenderPass);
			BReturn DestroyRenderPass(VkRenderPass* pRenderPass);
			BReturn CreateFramebuffer(uint32_t width, uint32_t height, std::vector<Texture2D>& attachments, VkRenderPass renderPass, VkFramebuffer* pFramebuffer);
			BReturn DestroyFramebuffer(VkFramebuffer* pFramebuffer);
			

			// Utillitys
			BReturn BeginSingleTimeCommand(VkCommandPool commandPool, VkCommandBuffer& pCommandBuffer);
			BReturn EndSingleTimeCommand(VkCommandPool commandPool, VkQueue queue, VkCommandBuffer& pCommandBuffer);
		

			// Buffers
			template<BufferType T>
			BReturn AllocateBuffer(VkDeviceSize bufferSize, VulkanBuffer& buffer);
			BReturn FreeBuffer(VulkanBuffer& buffer);

			BReturn CopyToBuffer(void* data, VkDeviceSize size, VulkanBuffer& buffer);
			BReturn CopyBufferToImage(VulkanBuffer& buffer, Texture2D& image);
			
			BReturn CompileShader(const std::string& filepath, std::vector<VkShaderModule>& modules, int stagesCount, const ShaderStage* pStages);
			BReturn DestroyShaderPackage(std::vector<VkShaderModule>& modules);

			/*!
			 * @brief Creates a graphics pipeline.
			 *
			 * @param[in] stageCount Number of shader stages in the pipeline.
			 * @param[in] pStages Pointer to an array of `VkPipelineShaderStageCreateInfo` structures specifying the shader stages.
			 * @param[in] VertexInputState Pointer to a `VkPipelineVertexInputStateCreateInfo` structure specifying the vertex input state.
			 * @param[in] pInputAssemblyState Pointer to a `VkPipelineInputAssemblyStateCreateInfo` structure specifying the input assembly state.
			 * @param[in] pTessellationState Pointer to a `VkPipelineTessellationStateCreateInfo` structure specifying the tessellation state. Can be `nullptr` if not using tessellation.
			 * @param[in] pViewportState Pointer to a `VkPipelineViewportStateCreateInfo` structure specifying the viewport state.
			 * @param[in] pRasterizationState Pointer to a `VkPipelineRasterizationStateCreateInfo` structure specifying the rasterization state.
			 * @param[in] pMultisampleState Pointer to a `VkPipelineMultisampleStateCreateInfo` structure specifying the multisample state.
			 * @param[in] pDepthStencilState Pointer to a `VkPipelineDepthStencilStateCreateInfo` structure specifying the depth and stencil state.
			 * @param[in] pColorBlendState Pointer to a `VkPipelineColorBlendStateCreateInfo` structure specifying the color blend state.
			 * @param[in] pDynamicState Pointer to a `VkPipelineDynamicStateCreateInfo` structure specifying the dynamic state. Can be `nullptr` if not using dynamic state.
			 * @param[in] layout The `VkPipelineLayout` object used by the pipeline.
			 * @param[in] renderPass The `VkRenderPass` object that the pipeline will be used with.
			 * @param[in] subpass Index of the subpass in the render pass where this pipeline will be used.
			 * @param[in] basePipelineHandle Optional handle to an existing pipeline to derive from. Can be `VK_NULL_HANDLE`.
			 * @param[in] basePipelineIndex Optional index of an existing pipeline to derive from. Can be -1.
			 * @param[out] pPipeline Pointer to a `VkPipeline` handle where the created pipeline will be stored.
			 *
			 * @return A `BReturn` indicating success or failure.
			 */
			BReturn CreateGraphicsPipeline(
				uint32_t                                        stageCount,
				const VkPipelineShaderStageCreateInfo* pStages,
				const VkPipelineVertexInputStateCreateInfo* VertexInputState,
				const VkPipelineInputAssemblyStateCreateInfo* pInputAssemblyState,
				const VkPipelineTessellationStateCreateInfo* pTessellationState,
				const VkPipelineViewportStateCreateInfo* pViewportState,
				const VkPipelineRasterizationStateCreateInfo* pRasterizationState,
				const VkPipelineMultisampleStateCreateInfo* pMultisampleState,
				const VkPipelineDepthStencilStateCreateInfo* pDepthStencilState,
				const VkPipelineColorBlendStateCreateInfo* pColorBlendState,
				const VkPipelineDynamicStateCreateInfo* pDynamicState,
				VkPipelineLayout                                layout,
				VkRenderPass                                    renderPass,
				uint32_t										subpass,
				VkPipeline                                      basePipelineHandle,
				int32_t                                         basePipelineIndex,
				VkPipeline* pPipeline);


			BReturn CreatePipelineLayout(std::vector<VkDescriptorSetLayout>& setLayouts, std::vector<VkPushConstantRange>& pushConstants, VkPipelineLayout* pPipelineLayout);
			BReturn DestroyPipeline(VkPipeline* pPipeline);

			BReturn UpdateUniforms(uint32_t bufferSize, VkBuffer& uniformBuffer, uint32_t binding, VkDescriptorSet& descriptorSet);


		protected:
			uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
		private:
			VkAllocationCallbacks*		allocator = NULL;
			VkInstance					instance = VK_NULL_HANDLE;			
			VkPhysicalDevice			physicalDevice = VK_NULL_HANDLE;	
			VkDevice					device = VK_NULL_HANDLE;			
			VkDebugReportCallbackEXT	debugReport = VK_NULL_HANDLE;		

			// TODO: these need to be worked out of the API
			// which also means all of the Get*() methods that 
			// mach each of these need to be removed and responsibillity of 
			// creation/descrution of ALL vkresources/vktypes NEEDS to be up
			// to the consumer of this API.
			uint32_t					queueFamily = (uint32_t)-1;			
			VkQueue						graphicsQueue = VK_NULL_HANDLE;		
			VkPipelineCache				pipelineCache = VK_NULL_HANDLE;
			//VkDescriptorPool			descriptorPool = VK_NULL_HANDLE;
			VkSurfaceKHR				surface = VK_NULL_HANDLE;			
			int							MinImageCount = 2;
			VkCommandPool				commandPool = VK_NULL_HANDLE;

			// Per-frame-in-flight
			std::vector<std::vector<VkCommandBuffer>> commandBuffers;
			
			// state 
			bool						SwapChainRebuild = false;
			uint32_t				    CurrentFrameIndex = 0;

		};
	}
}

#include "VulkanAPI.impl.h"