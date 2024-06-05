#pragma once

#include "pch.h"

#include "IGraphicsAPI.h"
#include "IRenderPipeline.h"

namespace Boundless {
	namespace Graphics {
		class VulkanAPI;
		namespace Rendering {
			class VulkanRenderPipeline : I::IRenderPipeline {
			public:
				VulkanRenderPipeline(I::IGraphicsAPI* pAPI) 
					: vk{reinterpret_cast<VulkanAPI*>(pAPI)} {};
				virtual ~VulkanRenderPipeline() = default;

			private:
				VulkanAPI* vk;
			};
		}
	}
}