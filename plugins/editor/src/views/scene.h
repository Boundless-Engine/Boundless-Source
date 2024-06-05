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


#include "resources/Mesh.h"

#include "SceneGraph.h"

using namespace Boundless;
using namespace Graphics;
using namespace Input;

#include <bitset>

enum class PipelineType {
    Wireframe,
    Unlit,
    Lit,
    Skybox,
};


class SceneView : public Boundless::I::ILayer {
public:
    SceneView(I::IGraphicsAPI* api) : api{ api }, enableBoxing{ false } {}
    ~SceneView() {}

    virtual BReturn OnAttach() override {
        size.x = 1, size.y = 1;

        auto vk = reinterpret_cast<VulkanAPI*>(api);
        scene = new SceneGraph(vk);

        Rendering::CreateFrambufferRenderer(api, &renderer);
        renderer->Initillize();

        renderPipelines.emplace(PipelineType::Wireframe, new Rendering::RenderPipelines::Wireframe(api, renderer->RenderPass()));
        renderPipelines.emplace(PipelineType::Unlit, new Rendering::RenderPipelines::Unlit(api, renderer->RenderPass()));
        renderPipelines.emplace(PipelineType::Lit, new Rendering::RenderPipelines::Lit(api, renderer->RenderPass()));
        renderPipelines.emplace(PipelineType::Skybox, new Rendering::RenderPipelines::Skybox(api, renderer->RenderPass()));

        startTime = std::chrono::high_resolution_clock::now();

        
        
		Model* plane = new Model([&](Model& m) {
			m.meshes.push_back(PrimitiveFactory::Create<Plane>(api));
            });
		
        scene->AddModel(plane, { glm::translate(glm::mat4{1.0f},  {0, 0, 0}), {0.7f, 0.6f, 0.7f, 1.0}  });

        Model* cube = new Model([&](Model& m) {
            m.meshes.push_back(PrimitiveFactory::Create<Cube>(api));
        });

        scene->AddModel(cube, { testMatrix, {0.9f, 0.0f, 0.0f, 1.0} });

        return SUCCESS;
    }

        glm::mat4 testMatrix = glm::mat4(1.0f);
    Model sphere_skybox;

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
        
        auto skybox = scene->Skybox();
        skybox->meshes[0]->matrix = glm::mat4(1.0f);
        skybox->meshes[0]->matrix[3][0] = cameraPosition.x;
        skybox->meshes[0]->matrix[3][1] = cameraPosition.y;
        skybox->meshes[0]->matrix[3][2] = cameraPosition.z;

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
        Draw_None = 0,				                        //!< draw nothing
        Draw_Wireframe = 0b0000'0001,			            //!< only draw the wireframe
        Draw_Lit = 0b0000'0010,			                    //!< only draw using the lit pipeline
        Draw_Lit_Wireframe = Draw_Wireframe | Draw_Lit,		//!< use both lit and wireframe 0x0000'0011
        Draw_Unlit = 0b0000'0100,			                //!< only draw using unlit pipeline
        Draw_Unlit_Wireframe = Draw_Wireframe | Draw_Unlit,	//!< use both unlit and wireframe 0x0000'0101
        Draw_Skybox = 0b100'0000			                //!< enable/disable skybox can be used with ALL or NONE
    };

    uint32_t draw_options = Draw_Lit_Wireframe | Draw_Skybox;

    void DrawScene(VkCommandBuffer cmd, I::IRenderPipeline* pipeline) {
        pipeline->OnBind(cmd);

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

        auto models = scene->GetModels();

        for (auto& pair : models)
        {
            auto model = pair.first;
            auto model_constants = pair.second;

            VkDeviceSize offset = 0;

            for (auto& mesh : model->meshes)
            {
                struct { glm::mat4 model;  glm::vec4 color; } constants;
                constants.color = model_constants.color;
                constants.model = model_constants.matrix * mesh->matrix;

                pipeline->OnPushConstants(cmd, &constants);

                bool draw_indexed = mesh->index_count > 0;

                if (mesh->vertex_count > 0) {
                    vkCmdBindVertexBuffers(cmd, 0, 1, &mesh->vbuffer.buffer, &offset);
                }
                if (mesh->index_count > 0) {
                    vkCmdBindIndexBuffer(cmd, mesh->ibuffer.buffer, 0, VK_INDEX_TYPE_UINT16);
                }

                if (draw_indexed)
                    vkCmdDrawIndexed(cmd, mesh->index_count, 1, 0, 0, 0);
                else
                    vkCmdDraw(cmd, mesh->vertex_count, 1, 0, 0);
            }
        }

    }

    void DrawSkybox(VkCommandBuffer cmd) {
        auto pipeline = renderPipelines[PipelineType::Skybox];
        
        auto model = scene->Skybox();

        pipeline->OnBind(cmd);

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


        VkDeviceSize offset = 0;

        for (auto& mesh : model->meshes)
        {
            struct SkyConstants { glm::mat4 mat; glm::vec4 bottomColor; glm::vec4 topColor; float height; } constants;
            constants.mat = mesh->matrix;
            constants.topColor = glm::vec4(0.5f, 0.7f, 1.0f, 1.0f);
            constants.bottomColor = glm::vec4(0.8f, 0.8, 0.8f, 1.0f);
            constants.height = renderer->Height();

            pipeline->OnPushConstants(cmd, &constants);

            bool draw_indexed = mesh->index_count > 0;

            if (mesh->vertex_count > 0) {
                vkCmdBindVertexBuffers(cmd, 0, 1, &mesh->vbuffer.buffer, &offset);
            }
            if (mesh->index_count > 0) {
                vkCmdBindIndexBuffer(cmd, mesh->ibuffer.buffer, 0, VK_INDEX_TYPE_UINT16);
            }

            if (draw_indexed)
                vkCmdDrawIndexed(cmd, mesh->index_count, 1, 0, 0, 0);
            else
                vkCmdDraw(cmd, mesh->vertex_count, 1, 0, 0);
        }
    }

    void DrawSceneLit(VkCommandBuffer cmd) {
        auto pipeline = renderPipelines[PipelineType::Lit];
        
        pipeline->OnBind(cmd);

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

        auto models = scene->GetModels();

        for (auto& pair : models)
        {
            auto model = pair.first;
            auto model_constants = pair.second;

            VkDeviceSize offsets[] = { 0, 0 };

            for (auto& mesh : model->meshes)
            {
                struct Light {
                    glm::vec3 position = glm::vec3{-3.0f, -2.0f, 1.0f}; glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
                }light;
                
                struct { glm::mat4 model = glm::mat4(1.0f); glm::vec4 baseColor = glm::vec4(1.0f); Light light = {}; } constants;
                constants.baseColor = model_constants.color;
                constants.model = model_constants.matrix * mesh->matrix;
                constants.light = light;

                pipeline->OnPushConstants(cmd, &constants);

                bool draw_indexed = mesh->index_count > 0;

                if (mesh->vertex_count > 0) {
                    
                    VkBuffer vbuffers [] = {mesh->vbuffer.buffer, mesh->nbuffer.buffer };

                    vkCmdBindVertexBuffers(cmd, 0, _countof(vbuffers), vbuffers, offsets);
                }
                if (mesh->index_count > 0) {
                    vkCmdBindIndexBuffer(cmd, mesh->ibuffer.buffer, 0, VK_INDEX_TYPE_UINT16);
                }

                if (draw_indexed)
                    vkCmdDrawIndexed(cmd, mesh->index_count, 1, 0, 0, 0);
                else
                    vkCmdDraw(cmd, mesh->vertex_count, 1, 0, 0);
            }
        }
    }

    virtual void OnRender() override {
        VkCommandBuffer cmd = (VkCommandBuffer)renderer->Begin();

        // drawing commands go in here.. 

        if ((draw_options & Draw_Skybox) == Draw_Skybox) {
            DrawSkybox(cmd);
        }

        if ((draw_options & Draw_Unlit) ==  Draw_Unlit) {
            DrawScene(cmd, renderPipelines[PipelineType::Unlit]);
        }
        else if ((draw_options & Draw_Lit) == Draw_Lit)
        {
            DrawSceneLit(cmd);
        }
        // Draw Wireframe over geometry
        if ((draw_options & Draw_Wireframe) == Draw_Wireframe) {
            DrawScene(cmd, renderPipelines[PipelineType::Wireframe]);
        }



        renderer->End();

    }

   


    virtual void OnGUI() override {
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

        // Retrieve the render texture ID
        ImTextureID sceneTexture = reinterpret_cast<ImTextureID>(renderer->GetRenderTexture());

        // Display the render texture with the correct size and position
        if (enableBoxing) {
            ImGui::SetCursorPos(texturePosition);
        }
        ImGui::Image(sceneTexture, size, { 0, 1 }, { 1, 0 });

        ImGui::EndChild();

        //ImGui::SetNextWindowBgAlpha(0);
        ImGui::SetCursorPos(ImVec2(0,0));

        ImGui::BeginChild("###Overlay", size);

        OverlayButton({ size.x / 2,  48 }, "Press Me!", [&]() {
            play = !play;
            if (play) {
                startTime = std::chrono::high_resolution_clock::now();
            }
            });

        ImGui::SetCursorPos({ 12, 48 });

        const char* options[6]{
            "None",
            "Wireframe",
            "Lit",
            "Lit-Wireframe",
            "Unlit",
            "Unlit-Wireframe",
        };
        bool skyboxActive = (draw_options & Draw_Skybox) != 0;

        ImGui::BeginChild("###MENU_DRAW_OPTIONS", {250, 50});
        {
            static int idx = (int)(draw_options);

            if (ImGui::Combo(" ", &idx, options, 6)) {
                draw_options = idx;

                if (skyboxActive)
                    draw_options |= Draw_Skybox;

                    
            }

        }
        ImGui::EndChild();

        const char* toggleables[1]{
            "ToggleSkybox",
        };

        ImGui::SetCursorPos({ 300, 48 });
        ImGui::BeginChild("###MENU_TOGGLE_OPTIONS", { 250, 50 });
        {
           

            if (ImGui::Checkbox("Skybox", &skyboxActive)) {
                draw_options ^= Draw_Skybox;
            }

        }
        ImGui::EndChild();

        ImGui::EndChild();
        ImGui::End();
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
    SceneGraph* scene;

    
    // temp 

    // state 
    bool play = false;

    glm::vec3 cameraPosition{ 0, 0, 5 };
    glm::vec3 moveDelta{0};


    glm::mat4 projectionMatrix;

    // render pipelines
    std::unordered_map<PipelineType, I::IRenderPipeline*> renderPipelines{};

    std::chrono::high_resolution_clock::time_point startTime;

    ImVec2 size{};
    I::IGraphicsAPI* api;
    I::IFramebufferRenderer* renderer;
    bool enableBoxing = false; // Flag to toggle boxing
};
