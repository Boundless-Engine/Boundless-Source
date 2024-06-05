#pragma once

#include "pch.h"

#include "IGraphicsAPI.h"

#include "PrimitiveFactory.h"

#include "resources/Model.h"


namespace Boundless {
    namespace Graphics {

        class SceneGraph {
        public:
            SceneGraph(I::IGraphicsAPI* api)
            {
                skybox = new Model([&](Model& m) {
                        m.meshes.push_back(PrimitiveFactory::Create<IcoSphere>(api, 250.0f, 6));
                    });
            };
            ~SceneGraph()
            {
                for (int i = models.size() - 1; i >= 0; i--)
                {
                    auto ptr = models[i].first;
                    delete ptr;
                }

                delete skybox;
            };

            void AddModel(Model* m, ModelData data) {
                models.push_back(std::make_pair(m, data));
            }

            Model* Skybox() { return skybox; }

            std::vector<std::pair<Model*, ModelData>> GetModels() { return models; }

        private:
            std::vector<std::pair<Model*, ModelData>> models;
            Model* skybox;

        };


    }
}