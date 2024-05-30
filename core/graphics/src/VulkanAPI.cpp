#include "VulkanAPI.h"

#include "backends/imgui_impl_vulkan.h"


#include "Profiler.h"

namespace Boundless {
	namespace Graphics {
		
		#define to_bytes_per_pixel(b, p)(b / 8) * p
		
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

		VulkanAPI::VulkanAPI(I::IWindow* pWindow) : I::IGraphicsAPI(pWindow) 
		{
			fn_profiler("VulkanSurface::VulkanSurface()");
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
				VkPhysicalDeviceFeatures deviceFeatures;
				vkGetPhysicalDeviceFeatures(physicalDevice, &deviceFeatures);

				int device_extension_count = 1;
				const char* device_extensions[] = { "VK_KHR_swapchain" };
				const float queue_priority[] = { 1.0f };
				VkDeviceQueueCreateInfo queue_info[1] = {};
				queue_info[0].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
				queue_info[0].queueFamilyIndex = queueFamily;
				queue_info[0].queueCount = 1;
				queue_info[0].pQueuePriorities = queue_priority;
				
				VkPhysicalDeviceFeatures enabledFeatures{};
				
				if (deviceFeatures.fillModeNonSolid)
					enabledFeatures.fillModeNonSolid = VK_TRUE;

				
				VkDeviceCreateInfo create_info = {};
				create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
				create_info.queueCreateInfoCount = sizeof(queue_info) / sizeof(queue_info[0]);
				create_info.pQueueCreateInfos = queue_info;
				create_info.enabledExtensionCount = device_extension_count;
				create_info.ppEnabledExtensionNames = device_extensions;
				create_info.enabledExtensionCount = 1;
				create_info.pEnabledFeatures = &enabledFeatures;


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

		VulkanAPI::~VulkanAPI()
		{
			fn_profiler("VulkanSurface::~VulkanSurface()");

			vkDestroyCommandPool(device, commandPool, allocator);
			vkDestroyDescriptorPool(device, descriptorPool, allocator);

#ifdef VULKAN_DEBUG_REPORT
			// Remove the debug report callback
			auto vkDestroyDebugReportCallbackEXT = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT");
			vkDestroyDebugReportCallbackEXT(instance, debugReport, allocator);
#endif // IMGUI_VULKAN_DEBUG_REPORT

			vkDestroyDevice(device, allocator);
			vkDestroyInstance(instance, allocator);

		}
	
		BReturn VulkanAPI::GetInstance(VkInstance* pInstance)
		{
			*pInstance = instance;

			return SUCCESS;
		}

		BReturn VulkanAPI::GetAllocator(VkAllocationCallbacks* pAllocatorCallback)
		{

			if (allocator == nullptr)
				return FAILURE;

			pAllocatorCallback = allocator;

			return SUCCESS;
		}
		BReturn VulkanAPI::GetDevice(VkDevice* pDevice)
		{
			if (device == nullptr || *pDevice != VK_NULL_HANDLE)
				return FAILURE;

			*pDevice = device;

			return SUCCESS;
		}
		BReturn VulkanAPI::GetPhysicalDevice(VkPhysicalDevice* pPhysicalDevice)
		{
			if (physicalDevice == nullptr || *pPhysicalDevice != VK_NULL_HANDLE)
				return FAILURE;

			*pPhysicalDevice = physicalDevice;

			return SUCCESS;
		}
		BReturn VulkanAPI::GetQueueFamily(uint32_t* pQueueFamily)
		{
			if (queueFamily == (uint32_t)-1)
				return FAILURE;

			*pQueueFamily = queueFamily;

			return SUCCESS;
		}
		BReturn VulkanAPI::GetCommandBuffer(std::vector<VkCommandBuffer>& buffers, int idx)
		{
			if (commandBuffers.size() <= 0 || idx < 0 || idx > commandBuffers.size())
				return FAILURE;

			buffers = commandBuffers[idx];
			return SUCCESS;
		}
		BReturn VulkanAPI::GetQueue(VkQueue* pQueue)
		{
			if (graphicsQueue == nullptr)
				return FAILURE;

			*pQueue = graphicsQueue;

			return SUCCESS;
		}

		BReturn VulkanAPI::GetSurface(VkSurfaceKHR* pSurface)
		{
			if (surface == VK_NULL_HANDLE)
				return FAILURE;

			*pSurface = surface;

			return SUCCESS;
		}

		BReturn VulkanAPI::GetDescriptorPool(VkDescriptorPool* pDescriptorPool)
		{
			if (descriptorPool == nullptr)
				return FAILURE;

			*pDescriptorPool = descriptorPool;

			return SUCCESS;
		}

		BReturn VulkanAPI::GetPipelineCache(VkPipelineCache* pPipelineCache)
		{
			if (pipelineCache == nullptr)
				return FAILURE;

			*pPipelineCache = pipelineCache;

			return SUCCESS;
		}
		
		BReturn VulkanAPI::GetCommandPool(VkCommandPool* pPool) {
			if (commandPool == nullptr)
				return FAILURE;

			*pPool = commandPool;

			return SUCCESS;
		}

		BReturn VulkanAPI::BeginSingleTimeCommand(VkCommandPool commandPool, VkCommandBuffer& pCommandBuffer) {
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

		BReturn VulkanAPI::EndSingleTimeCommand(VkCommandPool commandPool, VkQueue queue, VkCommandBuffer& pCommandBuffer) {
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

		BReturn VulkanAPI::CreatePipelineLayout(VkPipelineLayout* pPipelineLayout)
		{
			VkPipelineLayoutCreateInfo info{};
			info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
			info.pNext = nullptr;
			info.flags = 0;

			+vkCreatePipelineLayout(device, &info, allocator, pPipelineLayout);

			return SUCCESS;
		}

		BReturn VulkanAPI::CreateGraphicsPipeline(
			uint32_t                                        stageCount,
			const VkPipelineShaderStageCreateInfo*			pStages,
			const VkPipelineVertexInputStateCreateInfo*		pVertexInputState,
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
			VkPipeline* pPipeline)
		{
			VkGraphicsPipelineCreateInfo info{ };
			info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
			info.pNext = nullptr;
			info.flags = 0;
			info.stageCount = stageCount;
			info.pStages = pStages;
			info.pVertexInputState = pVertexInputState;
			info.pInputAssemblyState = pInputAssemblyState;
			info.pViewportState = pViewportState;
			info.pRasterizationState = pRasterizationState;
			info.pMultisampleState = pMultisampleState;
			info.pDepthStencilState = pDepthStencilState;
			info.pColorBlendState = pColorBlendState;
			info.layout = layout;
			info.renderPass = renderPass;
			info.subpass = subpass;
			info.basePipelineHandle = basePipelineHandle;
			info.basePipelineIndex = basePipelineIndex;

			+vkCreateGraphicsPipelines(device, nullptr, 1, &info, allocator, pPipeline);

			return SUCCESS;
		}
		shaderc_shader_kind  DetermineShaderStage(ShaderStage t) {
			shaderc_shader_kind stage;
			if (t == ShaderStage::Vertex)
				stage = shaderc_vertex_shader;
			else if (t == ShaderStage::Fragment)
				stage = shaderc_fragment_shader;
			else if (t == ShaderStage::Geometry)
				stage = shaderc_geometry_shader;
			else if (t == ShaderStage::Tesselation)
				stage = shaderc_tess_evaluation_shader;
			else
				throw std::runtime_error("ShaderType is Unrecognized");

			return stage;
		}
		BReturn VulkanAPI::CompileShader(const std::string& filepath, std::vector<VkShaderModule>& modules, int stagesCount, const ShaderStage* pStages)
		{
			// read file.
			std::ifstream file(filepath, std::ios::ate);
			if (!file.is_open())
				return FAILURE;

			auto fileSize = file.tellg();
			file.seekg(0);
			std::vector<char> buffer(fileSize);
			file.read(buffer.data(), fileSize);
			file.close();

			// compile glsl to spirv
			
			std::string source = buffer.data();

			modules.resize(stagesCount);

			for (int i = 0; i < stagesCount; i++)
			{
				shaderc::Compiler compiler;
				shaderc::CompileOptions options;
				
				shaderc_shader_kind stage = DetermineShaderStage(pStages[i]);

				if (stage == shaderc_vertex_shader) {
					options.AddMacroDefinition("VERTEX_SHADER");
				}
				else if (stage == shaderc_fragment_shader) {
					options.AddMacroDefinition("FRAGMENT_SHADER");
				}

				shaderc::SpvCompilationResult result = compiler.CompileGlslToSpv(source, stage, filepath.c_str(), options);

				if (result.GetCompilationStatus() != shaderc_compilation_status_success) {
					//Debug::Log("Failed To Compile Shader to SPIR-V: %s", filepath.c_str());
					std::cout << result.GetErrorMessage() << std::endl;
					return FAILURE;
				}

				std::vector<uint32_t> spirv{ result.begin(), result.end() };

				VkShaderModuleCreateInfo info{};
				info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
				info.pNext = nullptr;
				info.flags = 0;
				info.codeSize = sizeof(uint32_t) * spirv.size();
				info.pCode = spirv.data();

				+vkCreateShaderModule(device, &info, allocator, &modules[i]);
			}

			// create shader module
			return SUCCESS;
		}

		uint32_t VulkanAPI::FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
			VkPhysicalDeviceMemoryProperties memProperties;
			vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

			for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
				if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
					return i;
				}
			}

			throw std::runtime_error("failed to find suitable memory type!");
		}

		BReturn VulkanAPI::CreateRenderTexture(uint32_t width, uint32_t height, Texture2D::MSAA samples, Texture2D::BitsPerPixel bits, Texture2D::ColorChannels channels, Texture2D& image)
		{
			return CreateTexture2D(nullptr, width, height, samples, bits, channels, image);
		}

		BReturn VulkanAPI::FreeBuffer(Buffer& buffer)
		{
			vkDestroyBuffer(device, buffer.buffer, allocator);
			vkFreeMemory(device, buffer.memory, allocator);

			return SUCCESS;
		}

		BReturn VulkanAPI::CopyToBuffer(void* data, VkDeviceSize bufferSize, Buffer& buffer) {

			void* mappedData;
			vkMapMemory(device, buffer.memory, 0, bufferSize, 0, &mappedData);
			memcpy(mappedData, data, static_cast<size_t>(bufferSize));
			vkUnmapMemory(device, buffer.memory);

			return SUCCESS;
		}

		BReturn VulkanAPI::CopyBufferToImage(Buffer& buffer, Texture2D& image)
		{

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
			region.imageExtent = { image.width, image.height, 1 };

			vkCmdCopyBufferToImage(
				cmd,
				buffer.buffer,
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

			return SUCCESS;
		}

		BReturn VulkanAPI::CreateTexture2D(void* data, uint32_t width, uint32_t height, Texture2D::MSAA samples, Texture2D::BitsPerPixel bits, Texture2D::ColorChannels channels, Texture2D& image)
		{
			image.width = width;
			image.height = height;
			image.channels = channels;
			image.bits = bits;
			image.samples = Texture2D::ConvertSampleRate(samples);
			image.format = Texture2D::ConvertFormat(bits, channels);

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
				info.format = image.format;
				info.imageType = VK_IMAGE_TYPE_2D;
				info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
				info.mipLevels = 1;
				info.tiling = VK_IMAGE_TILING_OPTIMAL;
				info.usage = VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
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

			// buffer default data or texture data
			{
				// If data exists, the use wants to upload texture data.
				// take care of this.
				if (data) {

					// Create a staging buffer
					Buffer staging{};
					int size = width * height * to_bytes_per_pixel((int)bits, (int)channels);

					AllocateBuffer<BufferType::Staging>(size, staging);
					// Copy data to Staging Buffer
					CopyToBuffer(data, size, staging);

					// Copy Staging Buffer to Image.
					CopyBufferToImage(staging, image);

					// Clean up staging buffer
					FreeBuffer(staging);
				}
				else {
					// no data is passed in.. we need some dummy data
					size_t count = width * height;
					uint32_t* dummy_data = new uint32_t[count]{ 0x000000FF };
					for (int i = 0; i < width * height; i++) dummy_data[i] = 0xFF000000;
					
					// Create Staging Buffer
					Buffer staging{};
					int size = width * height * to_bytes_per_pixel((int)bits, (int)channels);
					
					AllocateBuffer<BufferType::Staging>(size, staging);
					
					// Copy data to Staging Buffer
					CopyToBuffer(dummy_data, size, staging);

					// Copy Staging Buffer to Image.
					CopyBufferToImage(staging, image);

					// Clean up staging buffer
					FreeBuffer(staging);

					// delte unnecissary data, its on the GPU now.
					delete[] dummy_data;
				}
			}

			// create view 
			{
				VkImageViewCreateInfo info = {};
				info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
				info.image = image.image;
				info.viewType = VK_IMAGE_VIEW_TYPE_2D;
				info.format = image.format;
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

		BReturn VulkanAPI::DestroyTexture2D(Texture2D& image)
		{
			vkFreeMemory(device, image.memory, allocator);
			vkDestroyImage(device, image.image, allocator);
			vkDestroyImageView(device, image.view, allocator);
			vkDestroySampler(device, image.sampler, allocator);

			return SUCCESS;
		}

		BReturn VulkanAPI::CreateRenderPass(Texture2D& renderTexture, VkRenderPass* pRenderPass)
		{
			VkAttachmentDescription colorAttachment{};
			colorAttachment.format = renderTexture.format;
			colorAttachment.samples = renderTexture.samples;
			colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
			colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
			colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
			colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
			colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			colorAttachment.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

			VkAttachmentReference colorAttachmentRef{};
			colorAttachmentRef.attachment = 0;
			colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

			VkSubpassDescription subpass{};
			subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
			subpass.colorAttachmentCount = 1;
			subpass.pColorAttachments = &colorAttachmentRef;

			VkRenderPassCreateInfo renderPassInfo{};
			renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
			renderPassInfo.attachmentCount = 1;
			renderPassInfo.pAttachments = &colorAttachment;
			renderPassInfo.subpassCount = 1;
			renderPassInfo.pSubpasses = &subpass;


			vkCreateRenderPass(device, &renderPassInfo, allocator, pRenderPass);

			return SUCCESS;
		}

		BReturn VulkanAPI::DestroyRenderPass(VkRenderPass* pRenderPass)
		{
			if (*pRenderPass == VK_NULL_HANDLE)
				return ATTEMPT_DELETE_NULLPTR;

			vkDestroyRenderPass(device, *pRenderPass, allocator);

			return SUCCESS;
		}

		BReturn VulkanAPI::CreateFramebuffer(uint32_t width, uint32_t height, Texture2D& renderTexture, VkRenderPass renderPass, VkFramebuffer* pFramebuffer)
		{
			VkImageView attachments[1]{ renderTexture.view };

			VkFramebufferCreateInfo info{};
			info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			info.pNext = nullptr;
			info.flags = 0;
			info.attachmentCount = _countof(attachments);
			info.pAttachments = attachments;
			info.width = width;
			info.height = height;
			info.layers = 1;
			info.renderPass = renderPass;

			+vkCreateFramebuffer(device, &info, allocator, pFramebuffer);

			return SUCCESS;
		}

		BReturn VulkanAPI::DestroyFramebuffer(VkFramebuffer* pFramebuffer)
		{
			if (*pFramebuffer == VK_NULL_HANDLE)
				return ATTEMPT_DELETE_NULLPTR;

			vkDestroyFramebuffer(device, *pFramebuffer, allocator);

			return SUCCESS;
		}


	}
}
