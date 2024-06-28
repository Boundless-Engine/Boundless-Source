#pragma once

#include "pch.h"

#include "entt/entt.hpp"

#include <glm/glm.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


struct SceneTransform {
    SceneTransform() = default;
    SceneTransform(const SceneTransform& t) = default;
    ~SceneTransform() = default;

    glm::mat4 transform = glm::mat4(1.0f);
    glm::vec3 position;
    glm::quat rotation;
    glm::vec3 scale;

    operator glm::mat4() {
        return transform;
    }

    operator const glm::mat4() {
        return transform;
    }

    void Decompose() {
        glm::vec3 skew;
        glm::vec4 perspective;
        glm::decompose(transform, scale, rotation, position, skew, perspective);
        rotation = glm::conjugate(rotation); // glm::decompose gives the inverse rotation
    }

    std::string ToString() {
        Decompose(); // Ensure we decompose before converting to string

        std::string str = "Matrix:\n";
        str += "position: (" + std::to_string(position.x) + ", " + std::to_string(position.y) + ", " + std::to_string(position.z) + ")\n";
        str += "rotation: (" + std::to_string(glm::degrees(glm::eulerAngles(rotation).x)) + ", " + std::to_string(glm::degrees(glm::eulerAngles(rotation).y)) + ", " + std::to_string(glm::degrees(glm::eulerAngles(rotation).z)) + ")\n";
        str += "scale:    (" + std::to_string(scale.x) + ", " + std::to_string(scale.y) + ", " + std::to_string(scale.z) + ")\n";
        return str;
    }
};



namespace Boundless {
    namespace Graphics {

        struct Scene {
            int index = 0;
            std::string name;
            std::string guid;
            entt::registry registery;

            template<typename T, typename ...Args>
            T& AddComponent(uint32_t entity, Args&&... args) {
                registery.emplace<T>(entt::entity(entity), std::forward<Args>(args)...);
                return GetComponent<T>(entity);
            }

            template<typename T>
            void RemoveComponent(uint32_t entity)
            {
                registery.remove<T>(entt::entity(entity));
            }

            uint32_t CreateEntity() {
                uint32_t e = static_cast<uint32_t>(registery.create());
                AddComponent<SceneTransform>(e);
                return e;
            }

            void DestroyEntity(uint32_t entity) {
                registery.destroy(entt::entity(entity));
            }

            template<typename T>
            T& GetComponent(uint32_t entity) {
                return registery.get<T>(entt::entity(entity));
            }

        };


  

    }
}