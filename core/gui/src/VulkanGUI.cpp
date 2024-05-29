#include "VulkanGUI.h"

#include "VulkanSurface.h"

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_vulkan.h"

namespace Boundless {
	namespace Graphics {

		static ImGui_ImplVulkanH_Window g_MainWindowData = {};
		static uint32_t s_CurrentFrameIndex = 0;
		static bool g_swapchainRebuild = false;

		VulkanGUI::VulkanGUI(VulkanSurface* vk)
			: vk{vk}
		{
			ImGui_ImplVulkanH_Window* wd = &g_MainWindowData;

			VkInstance instance = VK_NULL_HANDLE;					vk->GetInstance(&instance);
			VkAllocationCallbacks* allocator = nullptr;	vk->GetAllocator(allocator);

			VkDevice device = VK_NULL_HANDLE;						vk->GetDevice(&device);
			VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;		vk->GetPhysicalDevice(&physicalDevice);
			uint32_t queueFamily = (uint32_t)-1;					vk->GetQueueFamily(&queueFamily);
			VkQueue queue = VK_NULL_HANDLE;							vk->GetQueue(&queue);
			VkPipelineCache pipelineCache = VK_NULL_HANDLE;			vk->GetPipelineCache(&pipelineCache);
			VkDescriptorPool descriptorPool = VK_NULL_HANDLE;		vk->GetDescriptorPool(&descriptorPool);
			VkSurfaceKHR surface = VK_NULL_HANDLE;					vk->GetSurface(&surface);
			int width = vk->Width();
			int height = vk->Height();
			
			VkBool32 res;

			wd->Surface = surface;

			vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, queueFamily, wd->Surface, &res);
			if (res != VK_TRUE)
			{
				fprintf(stderr, "Error no WSI support on physical device 0\n");
				exit(-1);
			}

			// Select Surface Format
			const VkFormat requestSurfaceImageFormat[] = { VK_FORMAT_B8G8R8A8_UNORM, VK_FORMAT_R8G8B8A8_UNORM, VK_FORMAT_B8G8R8_UNORM, VK_FORMAT_R8G8B8_UNORM };
			const VkColorSpaceKHR requestSurfaceColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
			wd->SurfaceFormat = ImGui_ImplVulkanH_SelectSurfaceFormat(physicalDevice, wd->Surface, requestSurfaceImageFormat, (size_t)IM_ARRAYSIZE(requestSurfaceImageFormat), requestSurfaceColorSpace);

			// Select Present Mode
#ifdef IMGUI_UNLIMITED_FRAME_RATE
			VkPresentModeKHR present_modes[] = { VK_PRESENT_MODE_MAILBOX_KHR, VK_PRESENT_MODE_IMMEDIATE_KHR, VK_PRESENT_MODE_FIFO_KHR };
#else
			VkPresentModeKHR present_modes[] = { VK_PRESENT_MODE_FIFO_KHR };
#endif
			wd->PresentMode = ImGui_ImplVulkanH_SelectPresentMode(physicalDevice, wd->Surface, &present_modes[0], IM_ARRAYSIZE(present_modes));

			ImGui_ImplVulkanH_CreateOrResizeWindow(instance, physicalDevice, device, wd, queueFamily, allocator, width, height, 2);


			IMGUI_CHECKVERSION();
			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO(); (void)io;
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
			//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
			io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
			io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
			//io.ConfigViewportsNoAutoMerge = true;
			//io.ConfigViewportsNoTaskBarIcon = true;

			// Setup Dear ImGui style
			ImGui::StyleColorsDark();
			//ImGui::StyleColorsClassic();

			// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
			ImGuiStyle& style = ImGui::GetStyle();
			if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
			{
				style.WindowRounding = 0.0f;
				style.Colors[ImGuiCol_WindowBg].w = 1.0f;
			}

			GLFWwindow* glfwWindowHandle;
			vk->GetWindowHandle((void**)&glfwWindowHandle);

			// Setup Platform/Renderer backends
			ImGui_ImplGlfw_InitForVulkan(glfwWindowHandle, true);
			ImGui_ImplVulkan_InitInfo init_info = {};
			init_info.Instance = instance;
			init_info.PhysicalDevice = physicalDevice;
			init_info.Device = device;
			init_info.QueueFamily = queueFamily;
			init_info.Queue = queue;
			init_info.PipelineCache = pipelineCache;
			init_info.DescriptorPool = descriptorPool;
			init_info.Subpass = 0;
			init_info.MinImageCount = 2;
			init_info.ImageCount = 2;
			init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
			init_info.Allocator = nullptr;
			init_info.CheckVkResultFn = Graphics::check_vk_result;

			ImGui_ImplVulkan_Init(&init_info, wd->RenderPass);

			//// Load default font
			//ImFontConfig fontConfig;
			//fontConfig.FontDataOwnedByAtlas = false;
			//ImFont* robotoFont = io.Fonts->AddFontFromMemoryTTF((void*)g_RobotoRegular, sizeof(g_RobotoRegular), 20.0f, &fontConfig);
			//io.FontDefault = robotoFont;




		}

		VulkanGUI::~VulkanGUI()
		{
			// get required objects from vulkan
			VkInstance instance = VK_NULL_HANDLE;	vk->GetInstance(&instance);
			VkAllocationCallbacks* allocator = nullptr;	vk->GetAllocator(allocator);
			VkDevice device = VK_NULL_HANDLE;		vk->GetDevice(&device);

			ImGui_ImplVulkanH_Window* wd = &g_MainWindowData;
			ImGui_ImplVulkanH_DestroyWindow(instance, device, wd, nullptr);


			ImGui_ImplVulkan_Shutdown();
			ImGui_ImplGlfw_Shutdown();
			ImGui::DestroyContext();


		}

		void VulkanGUI::Begin()
		{
			ImDrawData* draw_data = nullptr;
			draw_data = ImGui::GetDrawData();

			VkResult err;

			// get required objects from vulkan
			VkInstance instance = VK_NULL_HANDLE;					vk->GetInstance(&instance);
			//VkAllocationCallbacks allocator = {};					vk->GetAllocator(&allocator);
			VkDevice device = VK_NULL_HANDLE;						vk->GetDevice(&device);
			VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;		vk->GetPhysicalDevice(&physicalDevice);
			uint32_t queueFamily = (uint32_t)-1;					vk->GetQueueFamily(&queueFamily);
			VkQueue renderQueue = VK_NULL_HANDLE;					vk->GetQueue(&renderQueue);


			// Start the Dear ImGui frame
			ImGui_ImplVulkan_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			{
				static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

				// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
				// because it would be confusing to have two docking targets within each others.
				ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;
				if (menubarCallback)
					window_flags |= ImGuiWindowFlags_MenuBar;

				const ImGuiViewport* viewport = ImGui::GetMainViewport();
				ImGui::SetNextWindowPos(viewport->WorkPos);
				ImGui::SetNextWindowSize(viewport->WorkSize);
				ImGui::SetNextWindowViewport(viewport->ID);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
				window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
				window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

				// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
				// and handle the pass-thru hole, so we ask Begin() to not render a background.

				if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
					window_flags |= ImGuiWindowFlags_NoBackground;

				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
				ImGui::Begin("###BoundlessDockspace", nullptr, window_flags);
				ImGui::PopStyleVar();

				ImGui::PopStyleVar(2);

				// Submit the DockSpace
				ImGuiIO& io = ImGui::GetIO();
				if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
				{
					ImGuiID dockspace_id = ImGui::GetID("###BoundlessDockspace");
					ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
				}

				if (menubarCallback)
				{
					if (ImGui::BeginMenuBar())
					{
						menubarCallback();
						ImGui::EndMenuBar();
					}
				}

				ImGui::End();
			}

			// Rendering
		
		}

		void VulkanGUI::End()
		{
			ImGui::Render();

			// Update and Render additional Platform Windows
			ImGuiIO& io = ImGui::GetIO();
			if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
			{
				ImGui::UpdatePlatformWindows();
				ImGui::RenderPlatformWindowsDefault();
			}

		}

		void VulkanGUI::Present()
		{
			
			if (g_swapchainRebuild) {
				RebuildSwapchain();
				return;
			}


			VkInstance instance = VK_NULL_HANDLE;					vk->GetInstance(&instance);
			VkAllocationCallbacks allocator = {};					vk->GetAllocator(&allocator);
			VkDevice device = VK_NULL_HANDLE;						vk->GetDevice(&device);
			VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;		vk->GetPhysicalDevice(&physicalDevice);
			uint32_t queueFamily = (uint32_t)-1;					vk->GetQueueFamily(&queueFamily);
			VkQueue renderQueue = VK_NULL_HANDLE;					vk->GetQueue(&renderQueue);
			VkDescriptorSet descriptorset = VK_NULL_HANDLE; 

			ImGui_ImplVulkanH_Window* wd = &g_MainWindowData;


			ImDrawData* draw_data = ImGui::GetDrawData();
			const bool main_is_minimized = (draw_data->DisplaySize.x <= 0.0f || draw_data->DisplaySize.y <= 0.0f);
			ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

			wd->ClearValue.color.float32[0] = clear_color.x * clear_color.w;
			wd->ClearValue.color.float32[1] = clear_color.y * clear_color.w;
			wd->ClearValue.color.float32[2] = clear_color.z * clear_color.w;
			wd->ClearValue.color.float32[3] = clear_color.w;

			VkSemaphore image_acquired_semaphore = wd->FrameSemaphores[wd->SemaphoreIndex].ImageAcquiredSemaphore;
			VkSemaphore render_complete_semaphore = wd->FrameSemaphores[wd->SemaphoreIndex].RenderCompleteSemaphore;
			auto err = vkAcquireNextImageKHR(device, wd->Swapchain, UINT64_MAX, image_acquired_semaphore, VK_NULL_HANDLE, &wd->FrameIndex);
			if (err == VK_ERROR_OUT_OF_DATE_KHR || err == VK_SUBOPTIMAL_KHR)
			{
				g_swapchainRebuild = true;
				return;
			}

			Graphics::check_vk_result(err);

			s_CurrentFrameIndex = (s_CurrentFrameIndex + 1) % wd->ImageCount;

			ImGui_ImplVulkanH_Frame* fd = &wd->Frames[wd->FrameIndex];
			{
				+vkWaitForFences(device, 1, &fd->Fence, VK_TRUE, UINT64_MAX);    // wait indefinitely instead of periodically checking
				+vkResetFences(device, 1, &fd->Fence);
			}


			// aquire command buffer
			// reset, begin cmd buffer
			{
				// Free command buffers allocated by Application::GetCommandBuffer
				// These use g_MainWindowData.FrameIndex and not s_CurrentFrameIndex because they're tied to the swapchain image index
				std::vector<VkCommandBuffer> commandBuffers;
				vk->GetCommandBuffer(commandBuffers, wd->FrameIndex);

				if (commandBuffers.size() > 0)
				{
					vkFreeCommandBuffers(device, fd->CommandPool, (uint32_t)commandBuffers.size(), commandBuffers.data());
					commandBuffers.clear();
				}

				+ vkResetCommandPool(device, fd->CommandPool, 0);
				VkCommandBufferBeginInfo info = {};
				info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
				info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
				+vkBeginCommandBuffer(fd->CommandBuffer, &info);
			}
			// begin renderpass
			{
				VkRenderPassBeginInfo info = {};
				info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
				info.renderPass = wd->RenderPass;
				info.framebuffer = fd->Framebuffer;
				info.renderArea.extent.width = wd->Width;
				info.renderArea.extent.height = wd->Height;
				info.clearValueCount = 1;
				info.pClearValues = &wd->ClearValue;
				vkCmdBeginRenderPass(fd->CommandBuffer, &info, VK_SUBPASS_CONTENTS_INLINE);
			}

			// Record dear imgui primitives into command buffer
			ImGui_ImplVulkan_RenderDrawData(draw_data, fd->CommandBuffer);
			// end rendpass, submit command buffer
			{
				vkCmdEndRenderPass(fd->CommandBuffer);

				VkPipelineStageFlags wait_stage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
				VkSubmitInfo info = {};
				info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
				info.waitSemaphoreCount = 1;
				info.pWaitSemaphores = &image_acquired_semaphore;
				info.pWaitDstStageMask = &wait_stage;
				info.commandBufferCount = 1;
				info.pCommandBuffers = &fd->CommandBuffer;
				info.signalSemaphoreCount = 1;
				info.pSignalSemaphores = &render_complete_semaphore;

				+vkEndCommandBuffer(fd->CommandBuffer);
				+vkQueueSubmit(renderQueue, 1, &info, fd->Fence);
			}


			VkQueue presentQueue = VK_NULL_HANDLE; vk->GetQueue(&presentQueue);


			//VkSemaphore render_complete_semaphore = wd->FrameSemaphores[wd->SemaphoreIndex].RenderCompleteSemaphore;
			VkPresentInfoKHR info = {};
			info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
			info.waitSemaphoreCount = 1;
			info.pWaitSemaphores = &render_complete_semaphore;
			info.swapchainCount = 1;
			info.pSwapchains = &wd->Swapchain;
			info.pImageIndices = &wd->FrameIndex;

			err = vkQueuePresentKHR(presentQueue, &info);

			if (err == VK_ERROR_OUT_OF_DATE_KHR || err == VK_SUBOPTIMAL_KHR)
			{
				g_swapchainRebuild = true;
				return;
			}
			Graphics::check_vk_result(err);

			wd->SemaphoreIndex = (wd->SemaphoreIndex + 1) % wd->ImageCount; // Now we can use the next set of semaphores
		}

		void VulkanGUI::RebuildSwapchain()
		{
			if (!g_swapchainRebuild)
				return;

			g_swapchainRebuild = false;

			VkInstance instance = VK_NULL_HANDLE; vk->GetInstance(&instance);
			VkDevice device = VK_NULL_HANDLE; vk->GetDevice(&device);
			VkPhysicalDevice physicalDevice = VK_NULL_HANDLE; vk->GetPhysicalDevice(&physicalDevice);
			VkAllocationCallbacks* allocator = nullptr; vk->GetAllocator(allocator);
			uint32_t queueFamily = vk->GetQueueFamily(&queueFamily);
			int width = vk->Width();
			int height = vk->Height();

			ImGui_ImplVulkanH_Window* wd = &g_MainWindowData;
			ImGui_ImplVulkanH_CreateOrResizeWindow(instance, physicalDevice, device, wd, queueFamily, allocator, width, height, wd->ImageCount);
		}


	}
}
