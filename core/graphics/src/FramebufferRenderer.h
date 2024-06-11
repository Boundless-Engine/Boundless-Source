#pragma once

#include "IFramebufferRenderer.h"
#include "IGraphicsAPI.h"

#ifdef ENABLE_VULKAN
#include "VulkanFramebufferRenderer.h"
#include "FramebufferUtils.h"
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

			static BReturn ResizeFramebuffer(int width, int height, I::IFramebufferRenderer** ppFramebufferRenderer, bool* isResize)
			{
				auto& pFramebuffer = *ppFramebufferRenderer;

				int w, h;

				w = pFramebuffer->Width();
				h = pFramebuffer->Height();

				if (w != width || h != height) {
					*isResize = true;
					pFramebuffer->OnResize(width, height);
				}
				

				return SUCCESS;
			}
		}
	}
}
