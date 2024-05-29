#pragma once

#include "pch.h"
#include "IRasterSurface.h"

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
			VkImage image;
			VkDeviceMemory memory;
			VkImageView view;
			VkSampler sampler;
			VkDescriptorSet descriptorSet;
		};

		class VulkanSurface 
			: public I::IRasterSurface
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
			VulkanSurface(I::IWindow* pWindow);
			~VulkanSurface();

			virtual void BeginFrame() override;
			virtual void EndFrame() override;

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

			BReturn CreateTexture2D(void* data, int width, int height, int bits, int channels, Texture2D& image);
			BReturn DestroyTexture2D(Texture2D& image);

			BReturn BeginSingleTimeCommand(VkCommandPool commandPool, VkCommandBuffer& pCommandBuffer);
			BReturn EndSingleTimeCommand(VkCommandPool commandPool, VkQueue queue, VkCommandBuffer& pCommandBuffer);

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