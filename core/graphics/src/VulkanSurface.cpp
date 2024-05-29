#include "VulkanSurface.h"

#include "backends/imgui_impl_vulkan.h"

namespace Boundless {
	namespace Graphics {

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

			{
				VkCommandPoolCreateInfo info{};
				info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
				info.pNext = nullptr;
				info.flags = 0;
				info.queueFamilyIndex = queueFamily;

				vkCreateCommandPool(device, &info, allocator, &commandPool);
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
		BReturn VulkanSurface::GetCommandPool(VkCommandPool* pPool) {
			if (commandPool == nullptr)
				return FAILURE;

			*pPool = commandPool;

			return SUCCESS;
		}


		BReturn VulkanSurface::BeginSingleTimeCommand(VkCommandPool commandPool, VkCommandBuffer& pCommandBuffer) {
			VkCommandBufferAllocateInfo allocInfo = {};
			allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
			allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
			allocInfo.commandPool = commandPool;
			allocInfo.commandBufferCount = 1;

			if (vkAllocateCommandBuffers(device, &allocInfo, &pCommandBuffer) != VK_SUCCESS) {
				return FAILURE;
			}

			VkCommandBufferBeginInfo beginInfo = {};
			beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

			if (vkBeginCommandBuffer(pCommandBuffer, &beginInfo) != VK_SUCCESS) {
				vkFreeCommandBuffers(device, commandPool, 1, &pCommandBuffer);
				return FAILURE;
			}

			return SUCCESS;
		}

		BReturn VulkanSurface::EndSingleTimeCommand(VkCommandPool commandPool, VkQueue queue, VkCommandBuffer& pCommandBuffer) {
			if (vkEndCommandBuffer(pCommandBuffer) != VK_SUCCESS) {
				return FAILURE;
			}

			VkSubmitInfo submitInfo = {};
			submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
			submitInfo.commandBufferCount = 1;
			submitInfo.pCommandBuffers = &pCommandBuffer;

			if (vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE) != VK_SUCCESS) {
				return FAILURE;
			}

			if (vkQueueWaitIdle(queue) != VK_SUCCESS) {
				return FAILURE;
			}

			vkFreeCommandBuffers(device, commandPool, 1, &pCommandBuffer);

			return SUCCESS;
		}


		VkFormat GetFormat(int b, int c) {
			
			if (b == 8 && c == 1)
				return VK_FORMAT_R8_UNORM;
			else if (b == 8 && c == 2)
				return VK_FORMAT_R8G8_UNORM;
			else if (b == 8 && c == 3)
				return VK_FORMAT_R8G8B8_UNORM;
			else if (b == 8 && c == 4)
				return VK_FORMAT_R8G8B8A8_UNORM;
			else if (b == 16 && c == 1)
				return VK_FORMAT_R16_UNORM;
			else if (b == 16 && c == 2)
				return VK_FORMAT_R16G16_UNORM;
			else if (b == 16 && c == 3)
				return VK_FORMAT_R16G16B16_UNORM;
			else if (b == 16 && c == 4)
				return VK_FORMAT_R16G16B16A16_UNORM;
			else if (b == 32 && c == 1)
				return VK_FORMAT_R32_SFLOAT;
			else if (b == 32 && c == 2)
				return VK_FORMAT_R32G32_SFLOAT;
			else if (b == 32 && c == 3)
				return VK_FORMAT_R32G32B32_SFLOAT;
			else if (b == 32 && c == 4)
				return VK_FORMAT_R32G32B32A32_SFLOAT;
			else
				throw std::runtime_error("Format Unrecognized!");

		}
		uint32_t VulkanSurface::FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
			VkPhysicalDeviceMemoryProperties memProperties;
			vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

			for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
				if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
					return i;
				}
			}

			throw std::runtime_error("failed to find suitable memory type!");
		}

		BReturn VulkanSurface::CreateTexture2D(void* data, int width, int height, int bits, int channels, Texture2D& image)
		{
			// create image
			{
				VkImageCreateInfo info{};
				info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
				info.flags = 0;
				info.pNext = nullptr;
				info.extent.width = width;
				info.extent.height = height;
				info.extent.depth = 1;
				info.arrayLayers = 1;
				info.format = GetFormat(bits, channels);
				info.imageType = VK_IMAGE_TYPE_2D;
				info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
				info.mipLevels = 1;
				info.tiling = VK_IMAGE_TILING_OPTIMAL;
				info.usage = VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
				info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
				info.samples = VK_SAMPLE_COUNT_1_BIT;

				+vkCreateImage(device, &info, allocator, &image.image);
			}

			// allocate and bind memory
			{
				VkMemoryRequirements reqs;
				vkGetImageMemoryRequirements(device, image.image, &reqs);

				VkMemoryAllocateInfo info{};
				info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
				info.allocationSize = reqs.size;
				info.memoryTypeIndex = FindMemoryType(reqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

				+vkAllocateMemory(device, &info, allocator, &image.memory);

				+vkBindImageMemory(device, image.image, image.memory, 0);
			}

			// if data exists buffer data 
			{
				// If data exists, buffer data
				if (data) {
					// Create a staging buffer
					VkBuffer stagingBuffer;
					VkDeviceMemory stagingBufferMemory;
					VkDeviceSize imageSize = width * height * (bits / 8) * channels;

					VkBufferCreateInfo bufferInfo{};
					bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
					bufferInfo.size = imageSize;
					bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
					bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

					if (vkCreateBuffer(device, &bufferInfo, allocator, &stagingBuffer) != VK_SUCCESS) {
						return FAILURE;
					}

					VkMemoryRequirements memRequirements;
					vkGetBufferMemoryRequirements(device, stagingBuffer, &memRequirements);

					VkMemoryAllocateInfo allocInfo{};
					allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
					allocInfo.allocationSize = memRequirements.size;
					allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

					if (vkAllocateMemory(device, &allocInfo, allocator, &stagingBufferMemory) != VK_SUCCESS) {
						return FAILURE;
					}

					if (vkBindBufferMemory(device, stagingBuffer, stagingBufferMemory, 0) != VK_SUCCESS) {
						return FAILURE;
					}

					// Map memory and copy data to the staging buffer
					void* mappedData;
					vkMapMemory(device, stagingBufferMemory, 0, imageSize, 0, &mappedData);
					memcpy(mappedData, data, static_cast<size_t>(imageSize));
					vkUnmapMemory(device, stagingBufferMemory);

					// Transition image layout and copy staging buffer to the image
					VkCommandBuffer cmd;
					if (BeginSingleTimeCommand(commandPool, cmd) != SUCCESS) {
						return FAILURE;
					}

					VkImageMemoryBarrier barrier{};
					barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
					barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
					barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
					barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
					barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
					barrier.image = image.image;
					barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
					barrier.subresourceRange.baseMipLevel = 0;
					barrier.subresourceRange.levelCount = 1;
					barrier.subresourceRange.baseArrayLayer = 0;
					barrier.subresourceRange.layerCount = 1;
					barrier.srcAccessMask = 0;
					barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

					vkCmdPipelineBarrier(
						cmd,
						VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
						0,
						0, nullptr,
						0, nullptr,
						1, &barrier
					);

					VkBufferImageCopy region{};
					region.bufferOffset = 0;
					region.bufferRowLength = 0;
					region.bufferImageHeight = 0;
					region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
					region.imageSubresource.mipLevel = 0;
					region.imageSubresource.baseArrayLayer = 0;
					region.imageSubresource.layerCount = 1;
					region.imageOffset = { 0, 0, 0 };
					region.imageExtent = { static_cast<uint32_t>(width), static_cast<uint32_t>(height), 1 };

					vkCmdCopyBufferToImage(
						cmd,
						stagingBuffer,
						image.image,
						VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
						1,
						&region
					);

					barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
					barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
					barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
					barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

					vkCmdPipelineBarrier(
						cmd,
						VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
						0,
						0, nullptr,
						0, nullptr,
						1, &barrier
					);

					if (EndSingleTimeCommand(commandPool, graphicsQueue, cmd) != SUCCESS) {
						return FAILURE;
					}

					// Clean up staging buffer
					vkDestroyBuffer(device, stagingBuffer, allocator);
					vkFreeMemory(device, stagingBufferMemory, allocator);
				}
			}

			// create view 
			{
				VkImageViewCreateInfo info = {};
				info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
				info.image = image.image;
				info.viewType = VK_IMAGE_VIEW_TYPE_2D;
				info.format = GetFormat(bits, channels);
				info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				info.subresourceRange.baseMipLevel = 0;
				info.subresourceRange.levelCount = 1;
				info.subresourceRange.baseArrayLayer = 0;
				info.subresourceRange.layerCount = 1;

				+vkCreateImageView(device, &info, nullptr, &image.view);
			}

			// create sampler 
			{
				VkSamplerCreateInfo info{};
				info.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
				info.magFilter = VK_FILTER_LINEAR;
				info.minFilter = VK_FILTER_LINEAR;
				info.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
				info.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
				info.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
				info.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
				info.mipLodBias = 0.0f;
				info.anisotropyEnable = VK_FALSE;
				info.maxAnisotropy = 1.0f;
				info.compareEnable = VK_FALSE;
				info.compareOp = VK_COMPARE_OP_ALWAYS;
				info.minLod = 0.0f;
				info.maxLod = 1.0f;
				info.unnormalizedCoordinates = VK_FALSE;
				info.unnormalizedCoordinates = VK_FALSE;

				vkCreateSampler(device, &info, allocator, &image.sampler);

			}

			image.descriptorSet = ImGui_ImplVulkan_AddTexture(image.sampler, image.view, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
			
			return SUCCESS;
		}

		BReturn VulkanSurface::DestroyTexture2D(Texture2D& image)
		{
			vkFreeMemory(device, image.memory, allocator);
			vkDestroyImage(device, image.image, allocator);
			vkDestroyImageView(device, image.view, allocator);

			return SUCCESS;
		}

	}
}
