#pragma once

#include "pch.h"
#include "IRenderer.h"

namespace Boundless {
	namespace I {
		class IFramebufferRenderer
		{
		public:
			virtual ~IFramebufferRenderer() {};
			
			virtual void Initillize() = 0;
			virtual void Shutdown() = 0;

			virtual int Width()  = 0;
			virtual int Height() = 0;
			
			virtual void* Begin() = 0;
			virtual void End() = 0;


			virtual void* RenderPass() = 0;

			virtual void OnResize(int nwidth, int nheight) = 0;

			virtual void* GetRenderTexture() = 0;

		};
	}
}
