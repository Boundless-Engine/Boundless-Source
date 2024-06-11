#pragma once

#include "IPrimitive.h"

#include "VulkanAPI.h"

namespace Boundless {
    namespace Graphics {
        static int _id_ = 0;
        struct Mesh {

            Mesh(I::IGraphicsAPI* api, std::vector<glm::vec3> positions, std::vector<uint16_t> indices, glm::mat4 matrix = glm::mat4(1.0))
                : api{ api }, vertex_count{ (uint32_t)positions.size() }, index_count{ (uint32_t)indices.size() }, matrix{ matrix }
            {
                auto vk = reinterpret_cast<VulkanAPI*>(api);

                VkDeviceSize vbufferSize = sizeof(glm::vec3) * positions.size();
                vk->AllocateBuffer<BufferType::Vertex>(vbufferSize, vbuffer);
                vk->CopyToBuffer((void*)positions.data(), vbufferSize, vbuffer);


                VkDeviceSize ibufferSize = sizeof(uint16_t) * indices.size();
                vk->AllocateBuffer<BufferType::Index>(ibufferSize, ibuffer);
                vk->CopyToBuffer((void*)indices.data(), ibufferSize, ibuffer);

                std::vector<glm::vec3> normals;
                Graphics::GeometryGenerator::CalculateNormals(positions, normals);
                normal_count = (uint32_t)normals.size();

                VkDeviceSize nbufferSize = sizeof(glm::vec3) * normals.size();
                vk->AllocateBuffer<BufferType::Vertex>(nbufferSize, nbuffer);
                vk->CopyToBuffer((void*)normals.data(), nbufferSize, nbuffer);

                std::vector<float> ids(positions.size(), _id_++);

                VkDeviceSize idbufferSize = sizeof(float) * ids.size();
                vk->AllocateBuffer<BufferType::Vertex>(idbufferSize, idbuffer);
                vk->CopyToBuffer((void*)ids.data(), idbufferSize, idbuffer);

            }

            ~Mesh() {
                auto vk = reinterpret_cast<VulkanAPI*>(api);

                vk->FreeBuffer(ibuffer);
                vk->FreeBuffer(vbuffer);
                vk->FreeBuffer(nbuffer);
            }

            uint32_t index_count = 0;
            uint32_t vertex_count = 0;
            uint32_t normal_count = 0;

            I::IGraphicsAPI* api;

            VulkanBuffer vbuffer{};
            VulkanBuffer ibuffer{};
            VulkanBuffer idbuffer{};
            VulkanBuffer nbuffer{};


            glm::mat4 matrix;

        };
    }
}




