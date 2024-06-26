#pragma once

#include "pch.h"

#include "IFramebufferRenderer.h"

namespace Boundless {

	namespace I {
		class IGraphicsAPI;
	}

	namespace Graphics {
		namespace Renderers {

			class VulkanFramebufferRenderer : public I::IFramebufferRenderer
			{
			public:
				VulkanFramebufferRenderer(I::IGraphicsAPI* rasterSurface);
				~VulkanFramebufferRenderer();

				// Inherited via IRenderer
				//virtual void OnEvent(I::IEvent* event) override;

				virtual void Initillize() override;
				virtual void Shutdown() override;

				virtual void* RenderPass() override ;

				virtual void* Begin() override;
				virtual void End() override;

				virtual int Width() override;
				virtual int Height() override;

				virtual void OnResize(int nwidth, int nheight) override;

				void* GetRenderTexture() override;

				BReturn GetTexture(int idx, void** texture);
			};
		}
	}
}