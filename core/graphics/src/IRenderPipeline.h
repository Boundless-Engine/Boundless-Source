#pragma once

#include "pch.h"

namespace Boundless {
	namespace I {
		class IRenderPipeline {
		public:
			virtual ~IRenderPipeline() = default;

			virtual void OnResize(uint32_t width, uint32_t height) = 0;

			virtual void OnBind(void* pCmd) = 0;
			virtual void OnUpdateUniforms(void* pPuffer) = 0;
			virtual void OnPushConstants(void* pCmd, void* pConstants) = 0;
			;
		};
	}
}
