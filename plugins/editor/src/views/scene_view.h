#pragma once

#include "pch.h"
#include "Boundless/Core.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "backends/imgui_impl_vulkan.h"

#include "InputHandler.h"

#include "IRenderPipeline.h"

#include "render_pipelines/Wireframe.h"
#include "render_pipelines/Unlit.h"
#include "render_pipelines/Lit.h"
#include "render_pipelines/Skybox.h"


//#include "resources/Mesh.h"
#include "PrimitiveFactory.h"

#include "Scene.h"

using namespace Boundless;
using namespace Graphics;
using namespace Input;

#include <bitset>

//#include "Logger.h"

enum class PipelineType {
	Wireframe,
	Unlit,
	Lit,
	Skybox,
};

struct Material {
	glm::vec4 baseColor;
};

struct MeshRenderer {

	std::vector<Mesh*> meshes;
	Material material;

	MeshRenderer() = default;
	MeshRenderer(const MeshRenderer& m) = default;
	~MeshRenderer() = default;

	void AddMesh(Mesh* mesh) {
		meshes.push_back(mesh);
	}
};

struct Skybox {
	glm::vec3 topColor;
	glm::vec3 bottomColor;

};


class SceneView : public Boundless::I::ILayer {
public:
	SceneView(I::IGraphicsAPI* api) : api{ api }, enableBoxing{ false } {}
	~SceneView() {}

	virtual BReturn OnAttach() override {
		size.x = 1, size.y = 1;

		auto vk = reinterpret_cast<VulkanAPI*>(api);

		Rendering::CreateFrambufferRenderer(api, &renderer);
		renderer->Initillize();

		renderPipelines.emplace(PipelineType::Wireframe, new Rendering::RenderPipelines::Wireframe(api, renderer->RenderPass()));
		renderPipelines.emplace(PipelineType::Unlit, new Rendering::RenderPipelines::Unlit(api, renderer->RenderPass()));
		renderPipelines.emplace(PipelineType::Lit, new Rendering::RenderPipelines::Lit(api, renderer->RenderPass()));
		renderPipelines.emplace(PipelineType::Skybox, new Rendering::RenderPipelines::Skybox(api, renderer->RenderPass()));

		startTime = std::chrono::high_resolution_clock::now();

		auto skybox = scene.CreateEntity();
		auto& meshComp = scene.AddComponent<MeshRenderer>(skybox);
		meshComp.AddMesh(PrimitiveFactory::Create<PrimitiveType::IcoSphere>(api, 50, 3));
		auto& skyboxComp = scene.AddComponent<Skybox>(skybox);

		auto plane = scene.CreateEntity();
		auto& meshComponent = scene.AddComponent<MeshRenderer>(plane);
		meshComponent.AddMesh(PrimitiveFactory::Create<Plane>(api));
		meshComponent.material = { glm::vec4(1.0f, 0.0f, 1.0f, 1.0f) };

		return SUCCESS;
	}

	glm::mat4 testMatrix = glm::mat4(1.0f);

	virtual BReturn OnDetach() override {

		auto vk = reinterpret_cast<VulkanAPI*>(api);

		renderer->Shutdown();

		for (auto kvp : renderPipelines)
		{
			auto _ptr = kvp.second;
			delete _ptr;
		}
		renderPipelines.clear();



		Rendering::DestroyFramebufferRenderer(&renderer);
		return SUCCESS;
	}

	virtual void OnEvent(I::IEvent* event) override {
		if (event->type == Events::Type::KeyPressed) {
			auto keyPressed = reinterpret_cast<Events::KeyPressEvent*>(event);
			auto keycode = keyPressed->keycode;
			if (keycode == KEY_W) {
				moveDelta.z = -1.0f; // moves camera forward
			}
			else if (keycode == KEY_S) {
				moveDelta.z = 1.0f; // moves camera backward
			}

			if (keycode == KEY_A) {
				moveDelta.x = -1.0f; // moves camera to left
			}
			else if (keycode == KEY_D) {
				moveDelta.x = 1.0f; // moves camera to right
			}

			if (keycode == KEY_E) {
				moveDelta.y = -1.0f; // moves camera up
			}
			else if (keycode == KEY_Q) {
				moveDelta.y = 1.0f;	// moves camera down
			}

		}
		else if (event->type == Events::Type::KeyReleased) {
			auto keyReleased = reinterpret_cast<Events::KeyReleaseEvent*>(event);
			auto keycode = keyReleased->keycode;
			if (keycode == KEY_W || keycode == KEY_S) {
				moveDelta.z = 0;
			}
			else if (keycode == KEY_E || keycode == KEY_Q) {
				moveDelta.y = 0;

			}
			else if (keycode == KEY_A || keycode == KEY_D) {
				moveDelta.x = 0;
			}
		}
	}

	virtual void OnUpdate(float ts) override {

		float elapsed = 0;

		if (!play) {
			elapsed = 0;
		}

		if (play) {
			auto currentTime = std::chrono::high_resolution_clock::now();
			elapsed = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
		}

		static float rot = 0;

		if (play)
			rot += ts * glm::radians(180.f);

		//// objects
		testMatrix = glm::rotate(glm::mat4(1.0f), rot, glm::vec3(1.0f, 0.0f, 0.0f));


		cameraPosition += moveDelta * 2.0f * ts;
		glm::mat4 viewMatrix = glm::translate(glm::mat4(1.0f), -cameraPosition);

		auto view= scene.registery.view<Skybox, SceneTransform>();
		for (auto ent : view)
		{
			auto& skybox = view.get<Skybox>(ent);
			auto& transform = view.get<SceneTransform>(ent);

			transform.transform[3][0] = cameraPosition.x;
			transform.transform[3][1] = cameraPosition.y;
			transform.transform[3][2] = cameraPosition.z;

		}

		struct UBO { glm::mat4 view; glm::mat4 proj; } ubo;

		ubo.proj = projectionMatrix;
		ubo.view = viewMatrix;

		for (auto& kvp : renderPipelines)
		{
			auto& pipe = kvp.second;

			pipe->OnUpdateUniforms((void*)&ubo);
		}

	}

	enum DrawOptions {
		Draw_None = 0,				                                //!< draw nothing
		Enable_Lighting = 0b0000'0000'0000'0001,                            //!< enable/disable lighting in viewport
		Enable_Skybox = 0b1000'0000'0000'0000,			                //!< enable/disable skybox render pass in viewport 
		Enable_Wireframe = 0b0100'0000'0000'0000,			                //!< enable/disable wireframe overlay pass in viewport
	};

	uint32_t draw_options = Enable_Lighting | Enable_Skybox | Enable_Wireframe;

	//void DrawSkybox(VkCommandBuffer cmd) {
	//    auto pipeline = renderPipelines[PipelineType::Skybox];
	//    
	//    auto model = scene->Skybox();

	//    pipeline->OnBind(cmd);

	//    VkViewport viewport{};
	//    viewport.width = size.x;
	//    viewport.height = size.y;
	//    viewport.minDepth = 0.01f;
	//    viewport.maxDepth = 1.0f;

	//    vkCmdSetViewport(cmd, 0, 1, &viewport);

	//    VkRect2D scissor{};
	//    scissor.extent = { (uint32_t)size.x, (uint32_t)size.y };
	//    scissor.offset = { 0, 0 };

	//    vkCmdSetScissor(cmd, 0, 1, &scissor);


	//    VkDeviceSize offset = 0;

	//    for (auto& mesh : model->meshes)
	//    {
	//        struct SkyConstants { glm::mat4 mat; glm::vec4 bottomColor; glm::vec4 topColor; float height; } constants;
	//        constants.mat = mesh->matrix;
	//        constants.topColor = glm::vec4(0.5f, 0.7f, 1.0f, 1.0f);
	//        constants.bottomColor = glm::vec4(0.8f, 0.8, 0.8f, 1.0f);
	//        constants.height = renderer->Height();

	//        pipeline->OnPushConstants(cmd, &constants);

	//        bool draw_indexed = mesh->index_count > 0;

	//        if (mesh->vertex_count > 0) {
	//            vkCmdBindVertexBuffers(cmd, 0, 1, &mesh->vbuffer.buffer, &offset);
	//        }
	//        if (mesh->index_count > 0) {
	//            vkCmdBindIndexBuffer(cmd, mesh->ibuffer.buffer, 0, VK_INDEX_TYPE_UINT16);
	//        }
	//        
	//        if (draw_indexed)
	//            vkCmdDrawIndexed(cmd, mesh->index_count, 1, 0, 0, 0);
	//        else
	//            vkCmdDraw(cmd, mesh->vertex_count, 1, 0, 0);
	//    }
	//}

	//void DrawSceneLit(VkCommandBuffer cmd) {
	//    auto pipeline = renderPipelines[PipelineType::Lit];
	//    
	//    pipeline->OnBind(cmd);

	//    VkViewport viewport{};
	//    viewport.width = size.x;
	//    viewport.height = size.y;
	//    viewport.minDepth = 0.01f;
	//    viewport.maxDepth = 1.0f;

	//    vkCmdSetViewport(cmd, 0, 1, &viewport);

	//    VkRect2D scissor{};
	//    scissor.extent = { (uint32_t)size.x, (uint32_t)size.y };
	//    scissor.offset = { 0, 0 };

	//    vkCmdSetScissor(cmd, 0, 1, &scissor);

	//    auto models = scene->GetModels();

	//    for (auto& pair : models)
	//    {
	//        auto model = pair.first;
	//        auto model_constants = pair.second;

	//        VkDeviceSize offsets[] = { 0, 0, 0};

	//        for (auto& mesh : model->meshes)
	//        {
	//            struct Light {
	//                glm::vec3 position = glm::vec3{-3.0f, -2.0f, 1.0f}; glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
	//            }light;
	//            
	//            struct { glm::mat4 model = glm::mat4(1.0f); glm::vec4 baseColor = glm::vec4(1.0f); Light light = {}; } constants;
	//            constants.baseColor = model_constants.color;
	//            constants.model = model_constants.matrix * mesh->matrix;
	//            constants.light = light;

	//            pipeline->OnPushConstants(cmd, &constants);

	//            bool draw_indexed = mesh->index_count > 0;

	//            if (mesh->vertex_count > 0) {
	//                
	//                VkBuffer vbuffers [] = {mesh->vbuffer.buffer, mesh->nbuffer.buffer, mesh->idbuffer.buffer };

	//                vkCmdBindVertexBuffers(cmd, 0, _countof(vbuffers), vbuffers, offsets);
	//            }
	//            if (mesh->index_count > 0) {
	//                vkCmdBindIndexBuffer(cmd, mesh->ibuffer.buffer, 0, VK_INDEX_TYPE_UINT16);
	//            }

	//            if (draw_indexed)
	//                vkCmdDrawIndexed(cmd, mesh->index_count, 1, 0, 0, 0);
	//            else
	//                vkCmdDraw(cmd, mesh->vertex_count, 1, 0, 0);
	//        }
	//    }
	//}

	virtual void OnRender() override {
		VkCommandBuffer cmd = (VkCommandBuffer)renderer->Begin();

		// drawing commands go in here.. 

		//if ((draw_options & Draw_Skybox) == Draw_Skybox) {
		//    DrawSkybox(cmd);
		//}

		//if ((draw_options & Draw_Unlit) ==  Draw_Unlit) {
		//    DrawScene(cmd, renderPipelines[PipelineType::Unlit]);
		//}
		//else if ((draw_options & Draw_Lit) == Draw_Lit)
		//{
		//    DrawSceneLit(cmd);
		//}
		//// Draw Wireframe over geometry
		//if ((draw_options & Draw_Wireframe) == Draw_Wireframe) {
		//    DrawScene(cmd, renderPipelines[PipelineType::Wireframe]);
		//}

		// draw all renderables
		// a renderable == Transform (model_matrix) + MeshRenderer(vertex stuff)

		bool drawSkybox = (draw_options & Enable_Skybox) != 0;

		if (drawSkybox) {
			auto view = scene.registery.view<SceneTransform, Skybox, MeshRenderer>();
			for (auto ent : view)
			{
				auto t = view.get<SceneTransform>(ent);
				auto rend = view.get<MeshRenderer>(ent);
				auto mesh = rend.meshes[0];
				auto skybox = view.get<Skybox>(ent);
				bool drawIndexed = mesh->index_count > 0;

				VkViewport viewport{};
				viewport.width = size.x;
				viewport.height = size.y;
				viewport.minDepth = 0.01f;
				viewport.maxDepth = 1.0f;

				vkCmdSetViewport(cmd, 0, 1, &viewport);

				VkRect2D scissor{};
				scissor.extent = { (uint32_t)size.x, (uint32_t)size.y };
				scissor.offset = { 0, 0 };

				vkCmdSetScissor(cmd, 0, 1, &scissor);

				auto pipeline = renderPipelines[PipelineType::Skybox];
				pipeline->OnBind(cmd);

				struct SkyConstants { glm::mat4 mat; glm::vec4 bottomColor; glm::vec4 topColor; float height; } constants;
				constants.mat = t.transform;
				constants.topColor = glm::vec4(0.5f, 0.7f, 1.0f, 1.0f);
				constants.bottomColor = glm::vec4(0.8f, 0.8, 0.8f, 1.0f);
				constants.height = renderer->Height();

				pipeline->OnPushConstants(cmd, &constants);

				bool draw_indexed = mesh->index_count > 0;
				VkDeviceSize offset = 0;

				if (mesh->vertex_count > 0) {
					vkCmdBindVertexBuffers(cmd, 0, 1, &mesh->vbuffer.buffer, &offset);
				}
				if (mesh->index_count > 0) {
					vkCmdBindIndexBuffer(cmd, mesh->ibuffer.buffer, 0, VK_INDEX_TYPE_UINT16);
				}



				if (drawIndexed)
					vkCmdDrawIndexed(cmd, mesh->index_count, 1, 0, 0, 0);
				else
					vkCmdDraw(cmd, mesh->vertex_count, 1, 0, 0);


			}
		}

		auto group = scene.registery.group<SceneTransform>(entt::get<MeshRenderer>, entt::exclude_t<Skybox>());
		for (auto ent : group)
		{
			auto [transform, meshRenderer] = group.get<SceneTransform, MeshRenderer>(ent);

			printf("Draw A Mesh... %s\n", transform.ToString().c_str());

			VkViewport viewport{};
			viewport.width = size.x;
			viewport.height = size.y;
			viewport.minDepth = 0.01f;
			viewport.maxDepth = 1.0f;

			vkCmdSetViewport(cmd, 0, 1, &viewport);

			VkRect2D scissor{};
			scissor.extent = { (uint32_t)size.x, (uint32_t)size.y };
			scissor.offset = { 0, 0 };

			vkCmdSetScissor(cmd, 0, 1, &scissor);

			for (auto mesh : meshRenderer.meshes)
			{
				bool drawLit = (draw_options & Enable_Lighting) != 0;
				bool drawWireframe = (draw_options & Enable_Wireframe) != 0;
				bool drawIndexed = mesh->index_count > 0;

				if (drawLit) {

					auto pipeline = renderPipelines[PipelineType::Lit];
					pipeline->OnBind(cmd);

					// TODO! [] create light entities. [] query light enities. [] plop in info and update foreach light instead of static one here... 
					struct Light {
						glm::vec3 position = glm::vec3{ -3.0f, -2.0f, 1.0f }; glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
					}light;

					struct { glm::mat4 model = glm::mat4(1.0f); glm::vec4 baseColor = glm::vec4(1.0f); Light light = {}; } constants;
					constants.baseColor = meshRenderer.material.baseColor;
					constants.model = transform.transform;
					constants.light = light;

					pipeline->OnPushConstants(cmd, &constants);

					if (mesh->vertex_count > 0) {
						VkBuffer vbuffers[] = { mesh->vbuffer.buffer, mesh->nbuffer.buffer, mesh->idbuffer.buffer };
						VkDeviceSize offsets[] = { 0, 0, 0 };

						vkCmdBindVertexBuffers(cmd, 0, _countof(vbuffers), vbuffers, offsets);
					}
					if (mesh->index_count > 0) {
						vkCmdBindIndexBuffer(cmd, mesh->ibuffer.buffer, 0, VK_INDEX_TYPE_UINT16);
					}

				}
				else {
					auto pipeline = renderPipelines[PipelineType::Unlit];
					pipeline->OnBind(cmd);

					struct { glm::mat4 model = glm::mat4(1.0f); glm::vec4 color = glm::vec4(1.0f); } constants;
					constants.color = meshRenderer.material.baseColor;
					constants.model = transform.transform;

					pipeline->OnPushConstants(cmd, &constants);

					if (mesh->vertex_count > 0) {
						VkBuffer vbuffers[] = { mesh->vbuffer.buffer,  mesh->idbuffer.buffer };
						VkDeviceSize offsets[] = { 0, 0 };

						vkCmdBindVertexBuffers(cmd, 0, _countof(vbuffers), vbuffers, offsets);
					}
					if (mesh->index_count > 0) {
						vkCmdBindIndexBuffer(cmd, mesh->ibuffer.buffer, 0, VK_INDEX_TYPE_UINT16);
					}
				}

				if (drawIndexed)
					vkCmdDrawIndexed(cmd, mesh->index_count, 1, 0, 0, 0);
				else
					vkCmdDraw(cmd, mesh->vertex_count, 1, 0, 0);

				if (drawWireframe) {
					auto pipeline = renderPipelines[PipelineType::Wireframe];
					pipeline->OnBind(cmd);

					struct { glm::mat4 model = glm::mat4(1.0f); } constants;
					constants.model = transform.transform;

					pipeline->OnPushConstants(cmd, &constants);

					if (mesh->vertex_count > 0) {
						VkBuffer vbuffers[] = { mesh->vbuffer.buffer, mesh->idbuffer.buffer };
						VkDeviceSize offsets[] = { 0, 0 };

						vkCmdBindVertexBuffers(cmd, 0, _countof(vbuffers), vbuffers, offsets);
					}
					if (mesh->index_count > 0) {
						vkCmdBindIndexBuffer(cmd, mesh->ibuffer.buffer, 0, VK_INDEX_TYPE_UINT16);
					}

					if (drawIndexed)
						vkCmdDrawIndexed(cmd, mesh->index_count, 1, 0, 0, 0);
					else
						vkCmdDraw(cmd, mesh->vertex_count, 1, 0, 0);
				}


			}



		}


		renderer->End();

	}




	virtual void OnGUI() override {

		ImGui::PushStyleVar(ImGuiStyleVar_::ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGui::Begin("Scene");

		ImVec2 texturePosition;
		ImVec2 contentSize = ImGui::GetContentRegionAvail();
		ImVec2 textureSize = contentSize;

		if (enableBoxing) {
			// Desired aspect ratio (e.g., 16:9)
			const float desiredAspectRatio = 16.0f / 9.0f;

			// Calculate the actual aspect ratio of the window
			float windowAspectRatio = contentSize.x / contentSize.y;

			// Calculate the size of the texture with letterboxing/pillarboxing
			if (windowAspectRatio > desiredAspectRatio) {
				// Window is wider than desired aspect ratio (letterboxing)
				textureSize.x = contentSize.y * desiredAspectRatio;
				textureSize.y = contentSize.y;
			}
			else {
				// Window is narrower than desired aspect ratio (pillarboxing)
				textureSize.x = contentSize.x;
				textureSize.y = contentSize.x / desiredAspectRatio;
			}

			// Center the texture within the available space
			texturePosition = { (contentSize.x - textureSize.x) * 0.5f, (contentSize.y - textureSize.y) * 0.5f };
		}

		bool resize = false;
		// Check if the size has changed
		if (textureSize.x != size.x || textureSize.y != size.y) {
			// Update the size of the texture and framebuffer
			size = textureSize;
			OnResize(size.x, size.y);
		}

		ImGui::BeginChild("###FrambufferTexture");

		//const char* viewTextures[3]{
		//    "Color",
		//    "Depth",
		//    "ID",
		//};

		//static int texture_id = (int)(draw_options);
		// Retrieve the render texture ID
		//ImTextureID sceneTexture = reinterpret_cast<ImTextureID>(renderer->GetRenderTexture(texture_id));
		ImTextureID sceneTexture = reinterpret_cast<ImTextureID>(renderer->GetRenderTexture());

		// Display the render texture with the correct size and position
		if (enableBoxing) {
			ImGui::SetCursorPos(texturePosition);
		}

		ImGui::Image(sceneTexture, size, { 0, 1 }, { 1, 0 });

		if (ImGui::IsMouseClicked(0, 0)) {
			ImVec2 mousePos = ImGui::GetMousePos();
			ImVec2 windowPos = ImGui::GetWindowPos();
			ImVec2 relativePos = { mousePos.x - windowPos.x, mousePos.y - windowPos.y };

			std::cout << std::format("mouse pressed x: {}, y: {}", relativePos.x, relativePos.y) << std::endl;

			if (relativePos.x >= 0 && relativePos.x < textureSize.x && relativePos.y >= 0 && relativePos.y < textureSize.y) {
				float pixelValue;
				VulkanTexture2D* tex;

				// get the id buffer.
				renderer->GetTexture(-1, (void**)&tex);
				Framebuffer::GetPixelInfo(api, (uint32_t)relativePos.x, (uint32_t)relativePos.y, sizeof(float), *tex, &pixelValue);

				std::cout << std::format("Pixel Picked:\tposition: <{},{}>\t value: {}", relativePos.x, relativePos.y, pixelValue) << std::endl;

			}

		}

		ImGui::EndChild();

		//ImGui::SetNextWindowBgAlpha(0);
		ImGui::SetCursorPos(ImVec2(0, 0));

		ImGui::BeginChild("###Overlay", size);

		OverlayButton({ size.x / 2,  48 }, "Press Me!", [&]() {
			play = !play;
			if (play) {
				startTime = std::chrono::high_resolution_clock::now();
			}
			});

		ImGui::SetCursorPos({ 12, 48 });

		bool skyboxActive = (draw_options & Enable_Skybox) != 0;
		bool lightsActive = (draw_options & Enable_Lighting) != 0;
		bool wireframeActive = (draw_options & Enable_Wireframe) != 0;

		ImGui::BeginChild("###MENU_TOGGLE_OPTIONS");
		{
			if (ImGui::Checkbox("Skybox", &skyboxActive)) {
				draw_options ^= Enable_Skybox;
			}ImGui::SameLine();
			if (ImGui::Checkbox("Lighting", &lightsActive)) {
				draw_options ^= Enable_Lighting;
			}ImGui::SameLine();
			if (ImGui::Checkbox("Wireframe", &wireframeActive)) {
				draw_options ^= Enable_Wireframe;
			}
		}

		ImGui::EndChild();

		ImGui::EndChild();
		ImGui::End();

		ImGui::PopStyleVar();

	}


	void OverlayButton(ImVec2 position, const char* label, std::function<void()> onClick) {
		ImGui::SetCursorPos(position);
		if (ImGui::Button(label))
		{
			onClick();
		}
	}


	void OnResize(int width, int height) {
		projectionMatrix = glm::perspectiveRH(glm::radians(60.0f), (float)width / (float)height, 0.01f, 1000.0f);
		projectionMatrix[1][1] *= -1;

		for (auto& kvp : renderPipelines)
		{
			auto pipe = kvp.second;
			pipe->OnResize(width, width);
		}
		bool resize = false;
		Rendering::ResizeFramebuffer(size.x, size.y, &renderer, &resize);
	}

private:
	Scene scene;


	// temp 

	// state 
	bool play = false;

	glm::vec3 cameraPosition{ 0, 0, 5 };
	glm::vec3 moveDelta{ 0 };


	glm::mat4 projectionMatrix;

	// render pipelines
	std::unordered_map<PipelineType, I::IRenderPipeline*> renderPipelines{};

	std::chrono::high_resolution_clock::time_point startTime;

	ImVec2 size{};
	I::IGraphicsAPI* api;
	I::IFramebufferRenderer* renderer;
	bool enableBoxing = false; // Flag to toggle boxing
};
