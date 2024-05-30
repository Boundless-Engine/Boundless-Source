#pragma once

#include "pch.h"
#include "IRenderer.h"

namespace Boundless {
	namespace I {
		class IFramebufferRenderer : public I::IRenderer
		{
		public:
			virtual ~IFramebufferRenderer() {};
			
			virtual int Width()  = 0;
			virtual int Height() = 0;
			
			virtual void OnResize(int nwidth, int nheight) = 0;

			virtual void* GetRenderTexture() = 0;

		};
	}
}
