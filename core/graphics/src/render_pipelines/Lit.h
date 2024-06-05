#pragma once

#include "pch.h"
#include "IRenderPipeline.h"

#include "VulkanAPI.h"

namespace Boundless {
	namespace Graphics {
		namespace Rendering {

			namespace RenderPipelines {
				class Lit : public I::IRenderPipeline {
				public:
					Lit(I::IGraphicsAPI* pAPI, void* renderPass);
					~Lit();

					virtual void OnResize(uint32_t width, uint32_t height) override;
					virtual void OnBind(void* pCmd) override;
					virtual void OnUpdateUniforms(void* pPuffer) override;
					virtual void OnPushConstants(void* pCmd, void* pConstants) override;
				};
			}
		}
	}
}