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
				RGBA = 4
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

				if (b == BitsPerPixel::BITS_8 && c == ColorChannels::R)
					return VK_FORMAT_R8_UNORM;
				else if (b == BitsPerPixel::BITS_8 && c == ColorChannels::RG)
					return VK_FORMAT_R8G8_UNORM;
				else if (b == BitsPerPixel::BITS_8 && c == ColorChannels::RGB)
					return VK_FORMAT_R8G8B8_UNORM;
				else if (b == BitsPerPixel::BITS_8 && c == ColorChannels::RGBA)
					return VK_FORMAT_R8G8B8A8_UNORM;
				else if (b == BitsPerPixel::BITS_16 && c == ColorChannels::R)
					return VK_FORMAT_R16_UNORM;
				else if (b == BitsPerPixel::BITS_16 && c == ColorChannels::RG)
					return VK_FORMAT_R16G16_UNORM;
				else if (b == BitsPerPixel::BITS_16 && c == ColorChannels::RGB)
					return VK_FORMAT_R16G16B16_UNORM;
				else if (b == BitsPerPixel::BITS_16 && c == ColorChannels::RGBA)
					return VK_FORMAT_R16G16B16A16_UNORM;
				else if (b == BitsPerPixel::BITS_32 && c == ColorChannels::R)
					return VK_FORMAT_R32_SFLOAT;
				else if (b == BitsPerPixel::BITS_32 && c == ColorChannels::RG)
					return VK_FORMAT_R32G32_SFLOAT;
				else if (b == BitsPerPixel::BITS_32 && c == ColorChannels::RGB)
					return VK_FORMAT_R32G32B32_SFLOAT;
				else if (b == BitsPerPixel::BITS_32 && c == ColorChannels::RGBA)
					return VK_FORMAT_R32G32B32A32_SFLOAT;
				else
					throw std::runtime_error("Format Unrecognized!");

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

		struct Buffer {
			VkBuffer buffer;
			VkDeviceMemory memory;
			VkDeviceSize size;
		
		
		};

		enum class BufferType {
			Staging,
			Vertex,
			Index,
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
				(void)flags; (void)object; (void)location; (void)messageCode; (void)pUserData; (void)pLayerPrefix; // Unused arguments
				fprintf(stderr, "[vulkan] Debug report from ObjectType: %i\nMessage: %s\n\n", objectType, pMessage);
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
			BReturn GetDescriptorPool(VkDescriptorPool* pDescriptorPool);
			BReturn GetPipelineCache(VkPipelineCache* pipelineCache);


			// RESOURCES
			BReturn CreateRenderTexture(uint32_t width, uint32_t height, Texture2D::MSAA samples, Texture2D::BitsPerPixel bits, Texture2D::ColorChannels channels, Texture2D& image);

			BReturn CreateTexture2D(void* data, uint32_t width, uint32_t height, Texture2D::MSAA samples, Texture2D::BitsPerPixel bits, Texture2D::ColorChannels channels, Texture2D& image);
			BReturn DestroyTexture2D(Texture2D& image);

			BReturn CreateRenderPass(Texture2D& renderTexture, VkRenderPass* pRenderPass);
			BReturn DestroyRenderPass(VkRenderPass* pRenderPass);
			BReturn CreateFramebuffer(uint32_t width, uint32_t height, Texture2D& renderTexture, VkRenderPass renderPass, VkFramebuffer* pFramebuffer);
			BReturn DestroyFramebuffer(VkFramebuffer* pFramebuffer);


			// Utillitys
			BReturn BeginSingleTimeCommand(VkCommandPool commandPool, VkCommandBuffer& pCommandBuffer);
			BReturn EndSingleTimeCommand(VkCommandPool commandPool, VkQueue queue, VkCommandBuffer& pCommandBuffer);
			
			// Buffers
			template<BufferType T>
			BReturn AllocateBuffer(VkDeviceSize bufferSize, Buffer& buffer);
			BReturn FreeBuffer(Buffer& buffer);

			BReturn CopyToBuffer(void* data, VkDeviceSize size, Buffer& buffer);
			BReturn CopyBufferToImage(Buffer& buffer, Texture2D& image);
			
			BReturn CompileShader(const std::string& filepath, std::vector<VkShaderModule>& modules, int stagesCount, const ShaderStage* pStages);
			
			/*!
			* @param [ i n] required stageCount
			* @param [ in ] required pStages
			* @param [ in ] required VertexInputState
			* @param [ in ] required pInputAssemblyState
			* @param [ in ] required pTessellationState
			* @param [ in ] required pViewportState
			* @param [ in ] required pRasterizationState
			* @param [ in ] required pMultisampleState
			* @param [ in ] required pDepthStencilState
			* @param [ in ] required pColorBlendState
			* @param [ in ] required pDynamicState
			* @param [ in ] required layout
			* @param [ in ] required renderPass
			* @param [ in ] required subpass
			* @param [ in ] optional basePipelineHandl,
			* @param [ in ] optional basePipelineIndex
			* @param [ out ] required pPipeline
			*/
			BReturn CreateGraphicsPipeline(
				uint32_t                                        stageCount,
				const VkPipelineShaderStageCreateInfo*			pStages,
				const VkPipelineVertexInputStateCreateInfo*		VertexInputState,
				const VkPipelineInputAssemblyStateCreateInfo*	pInputAssemblyState,
				const VkPipelineTessellationStateCreateInfo*	pTessellationState,
				const VkPipelineViewportStateCreateInfo*		pViewportState,
				const VkPipelineRasterizationStateCreateInfo*	pRasterizationState,
				const VkPipelineMultisampleStateCreateInfo*		pMultisampleState,
				const VkPipelineDepthStencilStateCreateInfo*	pDepthStencilState,
				const VkPipelineColorBlendStateCreateInfo*		pColorBlendState,
				const VkPipelineDynamicStateCreateInfo*			pDynamicState,
				VkPipelineLayout                                layout,
				VkRenderPass                                    renderPass,
				uint32_t										subpass,
				VkPipeline                                      basePipelineHandle,
				int32_t                                         basePipelineIndex,
				VkPipeline*										pPipeline);

			BReturn CreatePipelineLayout(VkPipelineLayout* pPipelineLayout);

		protected:
			uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
		private:
			VkAllocationCallbacks*		allocator = NULL;
			VkInstance					instance = VK_NULL_HANDLE;			
			VkPhysicalDevice			physicalDevice = VK_NULL_HANDLE;	
			VkDevice					device = VK_NULL_HANDLE;			
			VkDebugReportCallbackEXT	debugReport = VK_NULL_HANDLE;		
			uint32_t					queueFamily = (uint32_t)-1;			
			VkQueue						graphicsQueue = VK_NULL_HANDLE;		
			VkPipelineCache				pipelineCache = VK_NULL_HANDLE;
			VkDescriptorPool			descriptorPool = VK_NULL_HANDLE;
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