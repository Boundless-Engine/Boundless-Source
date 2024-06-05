#pragma once

#include "pch.h"
#include "IGraphicsAPI.h"

#include "resources/mesh.h"

namespace Boundless {
	namespace Graphics {

        enum PrimitiveType {
            None,
            Quad, // TODO! Implement me!
            Cube,
            Plane,
            IcoSphere,
            UVSphere,
            Capsule,
            Cylinder // TODO! Implement me!
        };
		
        namespace PrimitiveFactory {

            // Helper struct to check if all provided types match the required types
            template <typename T, typename... Args>
            struct are_same : std::conjunction<std::is_same<T, Args>...> {};

            // Specialization for tuple to check types
            template <typename Tuple, typename... Args, std::size_t... Is>
            bool check_types_impl(const std::tuple<Args...>& tup, const std::vector<const std::type_info*>& req, std::index_sequence<Is...>) {
                return ((typeid(std::get<Is>(tup)) == *req[Is]) && ...);
            }

            template <typename... Args>
            const bool check_types(const std::tuple<Args...>& tup, const std::vector<const std::type_info*>& req) {
                return check_types_impl(tup, req, std::index_sequence_for<Args...>{});
            }

            // Primary template for tuple_size
            template <typename Tuple>
            struct tuple_size;

            // Partial specialization for tuple_size with a tuple type
            template <typename... Ts>
            struct tuple_size<std::tuple<Ts...>> {
                static constexpr std::size_t value = sizeof...(Ts); // Number of types in the parameter pack
            };

            // Helper function to get the length of a tuple
            template <typename Tuple>
            constexpr bool is_empty(const Tuple& t) {
                return tuple_size<Tuple>::value == 0;
            }


            template<PrimitiveType T, typename ...Args>
            static Mesh* Create(I::IGraphicsAPI* pAPI, Args&& ...args) 
            {
                std::vector<glm::vec3>  verts;
                std::vector<uint16_t>   inds;
                
                Mesh* mesh = nullptr;

                switch (T)
                {
                case None:
                    break;
                case Quad:
                    break;
                case Cube:
                    GeometryGenerator::GenerateCube(verts, inds);
                    break;
                case Plane:
                {
                    auto params = std::vector<std::any>(std::forward<Args>(args)...);

                    if (!params.empty()) {
                        // function signature overrides
                        std::vector<const std::type_info*> req = { &typeid(int), &typeid(int), &typeid(float) };

                        //assert((sizeof...(args) == req.size() && check_types(tup, req)) && "Arguments Must Match and be Same Dimensions as Impl_Func");

                        auto width = std::any_cast<int>(params[0]);
                        auto height = std::any_cast<int>(params[1]);
                        auto unitSize = std::any_cast<float>(params[2]);

                        GeometryGenerator::GeneratePlane(verts, inds, width, height, unitSize);

                    }
                    else {
                        GeometryGenerator::GeneratePlane(verts, inds, 16, 16, 1.0f);
                    }
                    break;
                }
                case IcoSphere:
                {
                    auto params = std::vector<std::any>(std::forward<Args>(args)...);

                    if (!params.empty()) {
                        std::vector<const std::type_info*> req = { &typeid(float), &typeid(int) };

                        //assert((sizeof...(args) == req.size() && check_types(tup, req) )&& "Arguments Must Match and be Same Dimensions as Impl_Func");

                        auto radius = std::any_cast<int>(params[0]);
                        auto subdivisions = std::any_cast<int>(params[1]);

                        GeometryGenerator::GenerateIcoSphere(verts, inds, radius, subdivisions);

                    }
                    else {
                        GeometryGenerator::GenerateIcoSphere(verts, inds);
                    }
                    break;
                }
                case UVSphere:
                    break;
                case Capsule:
                    break;
                case Cylinder:
                    break;
                default:
                    break;
                }

                mesh = new Mesh(pAPI, verts, inds, glm::mat4(1.0f));
                return mesh;

            }
		}
	}
}