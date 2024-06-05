#pragma once

#include "IRenderPipeline.h"

#include "VulkanAPI.h"

namespace Boundless {
	
	namespace Graphics {
		namespace Rendering {

			namespace RenderPipelines {

				class Wireframe : public I::IRenderPipeline {
				public:
					Wireframe(I::IGraphicsAPI* pAPI, void* renderPass);
					~Wireframe();

					virtual void OnResize(uint32_t width, uint32_t height) override;

					virtual void OnBind(void* pCmd) override;
					virtual void OnUpdateUniforms(void*) override;
					virtual void OnPushConstants(void* pCmd, void* pConstants) override;
					
				};
			}
		}
	}
}
