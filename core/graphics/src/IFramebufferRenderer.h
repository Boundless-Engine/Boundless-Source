#pragma once

#include "pch.h"
#include "IRenderer.h"

namespace Boundless {
	namespace I {
		class IFramebufferRenderer : public I::IRenderer
		{
		public:
			virtual ~IFramebufferRenderer() {};
			virtual void* GetRenderTexture() = 0;
		};
	}
}
