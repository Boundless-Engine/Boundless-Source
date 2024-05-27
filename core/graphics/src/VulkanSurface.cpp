#include "VulkanSurface.h"

#include <string>
#include <unordered_map>

namespace Boundless {
	namespace Graphics {
#include <string>
#include <unordered_map>

		std::string vkResultToString(VkResult result) {
			static const std::unordered_map<VkResult, std::string> resultStrings = {
				{VK_SUCCESS, "VK_SUCCESS"},
				{VK_NOT_READY, "VK_NOT_READY"},
				{VK_TIMEOUT, "VK_TIMEOUT"},
				{VK_EVENT_SET, "VK_EVENT_SET"},
				{VK_EVENT_RESET, "VK_EVENT_RESET"},
				{VK_INCOMPLETE, "VK_INCOMPLETE"},
				{VK_ERROR_OUT_OF_HOST_MEMORY, "VK_ERROR_OUT_OF_HOST_MEMORY"},
				{VK_ERROR_OUT_OF_DEVICE_MEMORY, "VK_ERROR_OUT_OF_DEVICE_MEMORY"},
				{VK_ERROR_INITIALIZATION_FAILED, "VK_ERROR_INITIALIZATION_FAILED"},
				{VK_ERROR_DEVICE_LOST, "VK_ERROR_DEVICE_LOST"},
				{VK_ERROR_MEMORY_MAP_FAILED, "VK_ERROR_MEMORY_MAP_FAILED"},
				{VK_ERROR_LAYER_NOT_PRESENT, "VK_ERROR_LAYER_NOT_PRESENT"},
				{VK_ERROR_EXTENSION_NOT_PRESENT, "VK_ERROR_EXTENSION_NOT_PRESENT"},
				{VK_ERROR_FEATURE_NOT_PRESENT, "VK_ERROR_FEATURE_NOT_PRESENT"},
				{VK_ERROR_INCOMPATIBLE_DRIVER, "VK_ERROR_INCOMPATIBLE_DRIVER"},
				{VK_ERROR_TOO_MANY_OBJECTS, "VK_ERROR_TOO_MANY_OBJECTS"},
				{VK_ERROR_FORMAT_NOT_SUPPORTED, "VK_ERROR_FORMAT_NOT_SUPPORTED"},
				{VK_ERROR_FRAGMENTED_POOL, "VK_ERROR_FRAGMENTED_POOL"},
				{VK_ERROR_UNKNOWN, "VK_ERROR_UNKNOWN"},
				{VK_ERROR_OUT_OF_POOL_MEMORY, "VK_ERROR_OUT_OF_POOL_MEMORY"},
				{VK_ERROR_INVALID_EXTERNAL_HANDLE, "VK_ERROR_INVALID_EXTERNAL_HANDLE"},
				{VK_ERROR_FRAGMENTATION, "VK_ERROR_FRAGMENTATION"},
				{VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS, "VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS"},
				{VK_PIPELINE_COMPILE_REQUIRED, "VK_PIPELINE_COMPILE_REQUIRED"},
				{VK_ERROR_OUT_OF_POOL_MEMORY_KHR, "VK_ERROR_OUT_OF_POOL_MEMORY_KHR"},
				{VK_ERROR_INVALID_EXTERNAL_HANDLE_KHR, "VK_ERROR_INVALID_EXTERNAL_HANDLE_KHR"},
				{VK_ERROR_FRAGMENTATION_EXT, "VK_ERROR_FRAGMENTATION_EXT"},
				{VK_ERROR_INVALID_DEVICE_ADDRESS_EXT, "VK_ERROR_INVALID_DEVICE_ADDRESS_EXT"},
				{VK_PIPELINE_COMPILE_REQUIRED_EXT, "VK_PIPELINE_COMPILE_REQUIRED_EXT"},
				{VK_ERROR_OUT_OF_DATE_KHR, "VK_ERROR_OUT_OF_DATE_KHR"},
				{VK_SUBOPTIMAL_KHR, "VK_SUBOPTIMAL_KHR"},
				{VK_ERROR_SURFACE_LOST_KHR, "VK_ERROR_SURFACE_LOST_KHR"},
				{VK_ERROR_NATIVE_WINDOW_IN_USE_KHR, "VK_ERROR_NATIVE_WINDOW_IN_USE_KHR"},
				{VK_ERROR_INCOMPATIBLE_DISPLAY_KHR, "VK_ERROR_INCOMPATIBLE_DISPLAY_KHR"},
				{VK_ERROR_VALIDATION_FAILED_EXT, "VK_ERROR_VALIDATION_FAILED_EXT"},
				{VK_ERROR_INVALID_SHADER_NV, "VK_ERROR_INVALID_SHADER_NV"},
				{VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT, "VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT"},
				{VK_ERROR_NOT_PERMITTED_KHR, "VK_ERROR_NOT_PERMITTED_KHR"},
				{VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT, "VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT"},
				{VK_THREAD_IDLE_KHR, "VK_THREAD_IDLE_KHR"},
				{VK_THREAD_DONE_KHR, "VK_THREAD_DONE_KHR"},
				{VK_OPERATION_DEFERRED_KHR, "VK_OPERATION_DEFERRED_KHR"},
				{VK_OPERATION_NOT_DEFERRED_KHR, "VK_OPERATION_NOT_DEFERRED_KHR"},
			};

			auto it = resultStrings.find(result);
			if (it != resultStrings.end()) {
				return it->second;
			}
			else {
				return "Unknown VKResult";
			}
		}

		void check_vk_result(VkResult err)
		{
			if (err == 0)
				return;
			
			fprintf(stderr, "[vulkan] Error: VkResult = %s\n", vkResultToString(err).c_str());

			if (err < 0)
				abort();
		}

		void operator +(const VkResult res)
		{
			check_vk_result(res);
		}

		VulkanSurface::VulkanSurface(I::IWindow* pWindow) : I::IRasterSurface(pWindow) 
		{
			uint32_t extensions_count = 0;
			const char** extensions = glfwGetRequiredInstanceExtensions(&extensions_count);

			// Create Vulkan Instance
			{
				VkInstanceCreateInfo create_info = {};
				create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
				create_info.enabledExtensionCount = extensions_count;
				create_info.ppEnabledExtensionNames = extensions;

#ifdef VULKAN_DEBUG_REPORT
				// Enabling validation layers
				const char* layers[] = { "VK_LAYER_KHRONOS_validation" };
				create_info.enabledLayerCount = 1;
				create_info.ppEnabledLayerNames = layers;

				// Enable debug report extension (we need additional storage, so we duplicate the user array to add our new extension to it)
				const char** extensions_ext = (const char**)malloc(sizeof(const char*) * (extensions_count + 1));
				memcpy(extensions_ext, extensions, extensions_count * sizeof(const char*));
				extensions_ext[extensions_count] = "VK_EXT_debug_report";
				create_info.enabledExtensionCount = extensions_count + 1;
				create_info.ppEnabledExtensionNames = extensions_ext;

				// Create Vulkan Instance
				+vkCreateInstance(&create_info, allocator, &instance);
				free(extensions_ext);

				// Get the function pointer (required for any extensions)
				auto vkCreateDebugReportCallbackEXT = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT");
				//IM_ASSERT(vkCreateDebugReportCallbackEXT != NULL);

				// Setup the debug report callback
				VkDebugReportCallbackCreateInfoEXT debug_report_ci = {};
				debug_report_ci.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
				debug_report_ci.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT | VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT;
				debug_report_ci.pfnCallback = debug_report;
				debug_report_ci.pUserData = NULL;
				+vkCreateDebugReportCallbackEXT(instance, &debug_report_ci, allocator, &debugReport);
#else
				// Create Vulkan Instance without any debug feature
				+vkCreateInstance(&create_info, allocator, &instance);
				//IM_UNUSED(DebugReport);
#endif
			}

			// Select GPU
			{
				uint32_t gpu_count;
				+vkEnumeratePhysicalDevices(instance, &gpu_count, NULL);
				//IM_ASSERT(gpu_count > 0);

				VkPhysicalDevice* gpus = (VkPhysicalDevice*)malloc(sizeof(VkPhysicalDevice) * gpu_count);
				+vkEnumeratePhysicalDevices(instance, &gpu_count, gpus);

				// If a number >1 of GPUs got reported, find discrete GPU if present, or use first one available. This covers
				// most common cases (multi-gpu/integrated+dedicated graphics). Handling more complicated setups (multiple
				// dedicated GPUs) is out of scope of this sample.
				int use_gpu = 0;
				for (int i = 0; i < (int)gpu_count; i++)
				{
					VkPhysicalDeviceProperties properties;
					vkGetPhysicalDeviceProperties(gpus[i], &properties);
					if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
					{
						use_gpu = i;
						break;
					}
				}

				physicalDevice = gpus[use_gpu];
				free(gpus);
			}

			// Select graphics queue family
			{
				uint32_t count;
				vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &count, NULL);
				VkQueueFamilyProperties* queues = (VkQueueFamilyProperties*)malloc(sizeof(VkQueueFamilyProperties) * count);
				vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &count, queues);
				for (uint32_t i = 0; i < count; i++)
					if (queues[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
					{
						queueFamily = i;
						break;
					}
				free(queues);
			}

			// Create Logical Device (with 1 queue)
			{
				int device_extension_count = 1;
				const char* device_extensions[] = { "VK_KHR_swapchain" };
				const float queue_priority[] = { 1.0f };
				VkDeviceQueueCreateInfo queue_info[1] = {};
				queue_info[0].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
				queue_info[0].queueFamilyIndex = queueFamily;
				queue_info[0].queueCount = 1;
				queue_info[0].pQueuePriorities = queue_priority;
				VkDeviceCreateInfo create_info = {};
				create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
				create_info.queueCreateInfoCount = sizeof(queue_info) / sizeof(queue_info[0]);
				create_info.pQueueCreateInfos = queue_info;
				create_info.enabledExtensionCount = device_extension_count;
				create_info.ppEnabledExtensionNames = device_extensions;
				+vkCreateDevice(physicalDevice, &create_info, allocator, &device);
				vkGetDeviceQueue(device, queueFamily, 0, &graphicsQueue);
			}

			// Create Descriptor Pool
			{
				VkDescriptorPoolSize pool_sizes[] =
				{
					{ VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
					{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
					{ VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
					{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
					{ VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
					{ VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
					{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
					{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
					{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
					{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
					{ VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
				};
				VkDescriptorPoolCreateInfo pool_info = {};
				pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
				pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
				pool_info.maxSets = 1000 * _countof(pool_sizes);
				pool_info.poolSizeCount = (uint32_t)_countof(pool_sizes);
				pool_info.pPoolSizes = pool_sizes;
				+vkCreateDescriptorPool(device, &pool_info, allocator, &descriptorPool);
			}

			// Create Window Surface
			{
				GLFWwindow* handle = nullptr;
				pWindow->Get((void**)&handle);
				+glfwCreateWindowSurface(instance, handle, allocator, &surface);
			}
		}

		VulkanSurface::~VulkanSurface()
		{
			vkDestroyDescriptorPool(device, descriptorPool, allocator);

#ifdef VULKAN_DEBUG_REPORT
			// Remove the debug report callback
			auto vkDestroyDebugReportCallbackEXT = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT");
			vkDestroyDebugReportCallbackEXT(instance, debugReport, allocator);
#endif // IMGUI_VULKAN_DEBUG_REPORT

			vkDestroyDevice(device, allocator);
			vkDestroyInstance(instance, allocator);

		}
	

		void VulkanSurface::BeginFrame()
		{

		}

		void VulkanSurface::EndFrame()
		{

		}

		BReturn VulkanSurface::GetInstance(VkInstance* pInstance)
		{
			*pInstance = instance;

			return SUCCESS;
		}

		BReturn VulkanSurface::GetAllocator(VkAllocationCallbacks* pAllocatorCallback)
		{

			if (allocator == nullptr)
				return FAILURE;

			pAllocatorCallback = allocator;

			return SUCCESS;
		}
		BReturn VulkanSurface::GetDevice(VkDevice* pDevice)
		{
			if (device == nullptr || *pDevice != VK_NULL_HANDLE)
				return FAILURE;

			*pDevice = device;

			return SUCCESS;
		}
		BReturn VulkanSurface::GetPhysicalDevice(VkPhysicalDevice* pPhysicalDevice)
		{
			if (physicalDevice == nullptr || *pPhysicalDevice != VK_NULL_HANDLE)
				return FAILURE;

			*pPhysicalDevice = physicalDevice;

			return SUCCESS;
		}
		BReturn VulkanSurface::GetQueueFamily(uint32_t* pQueueFamily)
		{
			if (queueFamily == (uint32_t)-1)
				return FAILURE;

			*pQueueFamily = queueFamily;

			return SUCCESS;
		}
		BReturn VulkanSurface::GetCommandBuffer(std::vector<VkCommandBuffer>& buffers, int idx)
		{
			if (commandBuffers.size() <= 0 || idx < 0 || idx > commandBuffers.size())
				return FAILURE;

			buffers = commandBuffers[idx];
			return SUCCESS;
		}
		BReturn VulkanSurface::GetQueue(VkQueue* pQueue)
		{
			if (graphicsQueue == nullptr)
				return FAILURE;

			*pQueue = graphicsQueue;

			return SUCCESS;
		}

		BReturn VulkanSurface::GetSurface(VkSurfaceKHR* pSurface)
		{
			if (surface == VK_NULL_HANDLE)
				return FAILURE;

			*pSurface = surface;

			return SUCCESS;
		}

		BReturn VulkanSurface::GetDescriptorPool(VkDescriptorPool* pDescriptorPool)
		{
			if (descriptorPool == nullptr)
				return FAILURE;

			*pDescriptorPool = descriptorPool;

			return SUCCESS;
		}

		BReturn VulkanSurface::GetPipelineCache(VkPipelineCache* pPipelineCache)
		{
			if (pipelineCache == nullptr)
				return FAILURE;

			*pPipelineCache = pipelineCache;

			return SUCCESS;
		}

	}
}
