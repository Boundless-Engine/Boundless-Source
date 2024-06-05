#pragma once

#include "pch.h"

#include "Mesh.h"

namespace Boundless {
	namespace Graphics {
        struct Model {
            Model() = default;
            Model(std::function<void(Model&)> onCreate) { onCreate(*this); }
            ~Model() {
                for (int i = meshes.size() - 1; i >= 0; i--)
                {
                    delete meshes[i];
                }
            }
            std::vector<Mesh*> meshes;
        };

        struct ModelData {
            glm::mat4 matrix;
            glm::vec4 color;
        };
	}
}