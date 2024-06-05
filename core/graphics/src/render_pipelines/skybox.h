#pragma once
#pragma once
#pragma once

#include "IRenderPipeline.h"

#include "VulkanAPI.h"

namespace Boundless {

	namespace Graphics {
		namespace Rendering {

			namespace RenderPipelines {

				class Skybox : public I::IRenderPipeline {
				public:
					Skybox(I::IGraphicsAPI* pAPI, void* renderPass);
					~Skybox();

					virtual void OnResize(uint32_t width, uint32_t height) override;

					virtual void OnBind(void* pCmd) override;
					virtual void OnUpdateUniforms(void*) override;
					virtual void OnPushConstants(void* pCmd, void* pConstants) override;

				};
			}
		}
	}
}
