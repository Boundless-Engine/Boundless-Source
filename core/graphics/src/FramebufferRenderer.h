#pragma once

#include "IFramebufferRenderer.h"
#include "IGraphicsAPI.h"

#ifdef ENABLE_VULKAN
#include "VulkanFramebufferRenderer.h"
#endif

namespace Boundless {

	namespace Graphics {

		namespace Rendering {

			static BReturn CreateFrambufferRenderer(I::IGraphicsAPI* pAPI, I::IFramebufferRenderer** ppIFramebufferRenderer)
			{
				if (pAPI == nullptr)
					return ARGUMENT_ERROR;

#ifdef ENABLE_VULKAN
				*ppIFramebufferRenderer = new Renderers::VulkanFramebufferRenderer(pAPI);
#endif
				return SUCCESS;
			}

			static BReturn DestroyFramebufferRenderer(I::IFramebufferRenderer** ppIFramebufferRenderer) {
				
				if (*ppIFramebufferRenderer == nullptr)
					return ATTEMPT_DELETE_NULLPTR;

				delete* ppIFramebufferRenderer;

				return SUCCESS;

			}
		}
	}
}
