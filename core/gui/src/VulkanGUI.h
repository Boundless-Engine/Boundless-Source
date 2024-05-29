#pragma once

#include "pch.h"
#include "IGUI.h"

#include <functional>

namespace Boundless {
	
	namespace Graphics {
		class VulkanAPI;

		class VulkanGUI : public I::IGUI
		{
		public:
			VulkanGUI(VulkanAPI* vk);
			~VulkanGUI();

			virtual void Begin() override;
			virtual void End() override;
			
			virtual void Present() override;

			void RebuildSwapchain();
			
			
		private:
			VulkanAPI* vk;

		};
	}
}