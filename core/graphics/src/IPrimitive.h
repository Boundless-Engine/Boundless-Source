#pragma once

#include "pch.h"

#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace Boundless {
	namespace Graphics {
		// UTILLITY FUNCTIONS 
		namespace GeometryGenerator {

			typedef std::pair<uint16_t, uint16_t> Edge;
			struct EdgeHash {
				std::size_t operator()(const Edge& e) const {
					auto h1 = std::hash<uint16_t>{}(e.first);
					auto h2 = std::hash<uint16_t>{}(e.second);
					return h1 ^ (h2 << 1);
				}
			};

			uint16_t getMidpointVertex(uint16_t v0, uint16_t v1, std::vector<glm::vec3>& positions, std::unordered_map<Edge, uint16_t, EdgeHash>& midpointCache) {
				// Ensure the smaller index is first for consistency
				if (v0 > v1) std::swap(v0, v1);
				Edge edge = std::make_pair(v0, v1);

				auto it = midpointCache.find(edge);
				if (it != midpointCache.end()) {
					return it->second;
				}

				glm::vec3 midpoint = glm::normalize((positions[v0] + positions[v1]) / 2.0f);
				positions.push_back(midpoint);
				uint16_t midpointIndex = positions.size() - 1;

				midpointCache[edge] = midpointIndex;
				return midpointIndex;
			}

			BReturn Subdivide(std::vector<glm::vec3>& positions, std::vector<uint16_t>& indices) {
				std::vector<uint16_t> newIndices;
				std::unordered_map<Edge, uint16_t, EdgeHash> midpointCache;

				for (size_t i = 0; i < indices.size(); i += 3) {
					uint16_t v0 = indices[i];
					uint16_t v1 = indices[i + 1];
					uint16_t v2 = indices[i + 2];

					uint16_t mid01 = getMidpointVertex(v0, v1, positions, midpointCache);
					uint16_t mid12 = getMidpointVertex(v1, v2, positions, midpointCache);
					uint16_t mid20 = getMidpointVertex(v2, v0, positions, midpointCache);

					newIndices.insert(newIndices.end(), {
						v0, mid01, mid20,
						v1, mid12, mid01,
						v2, mid20, mid12,
						mid01, mid12, mid20
						});
				}

				indices = newIndices;
				return SUCCESS;
			}
	
			BReturn GeneratePlane(std::vector<glm::vec3>& positions, std::vector<uint16_t>& indices, int width = 10, int height = 10, float unitSize = 1.0f) {
				// Ensure subdivisions are valid
				if (width < 1 || height < 1) {
					return FAILURE;
				}

				positions.clear();
				indices.clear();

				float halfWidth = width * unitSize / 2.0f;
				float halfHeight = height * unitSize / 2.0f;

				float bufferSize = (width + 1) * (height + 1);

				for (int i = 0; i < bufferSize; i++) {
					int x = (i % (width + 1)) * unitSize - halfWidth;
					int z = (i / (width + 1)) * unitSize - halfHeight;

					// Create quad (2 triangles) around this position
					glm::vec3 p0 = glm::vec3(x, 0, z) + glm::vec3(-unitSize / 2, 0, unitSize / 2);
					glm::vec3 p1 = glm::vec3(x, 0, z) + glm::vec3(-unitSize / 2, 0, -unitSize / 2);
					glm::vec3 p2 = glm::vec3(x, 0, z) + glm::vec3(unitSize / 2, 0, unitSize / 2);
					glm::vec3 p3 = glm::vec3(x, 0, z) + glm::vec3(unitSize / 2, 0, -unitSize / 2);

					// Generate first triangle
					positions.push_back(p0);
					positions.push_back(p1);
					positions.push_back(p2);

					// Generate second triangle
					positions.push_back(p1);
					positions.push_back(p3);
					positions.push_back(p2);
				}

				// Generate indices
				for (int i = 0; i < bufferSize; i++) {
					indices.push_back(i * 6 + 0);
					indices.push_back(i * 6 + 1);
					indices.push_back(i * 6 + 2);

					indices.push_back(i * 6 + 3);
					indices.push_back(i * 6 + 4);
					indices.push_back(i * 6 + 5);
				}

				return SUCCESS;
			}

			// GEOMETRY GENERATION
			BReturn CalculateNormals(std::vector<glm::vec3> positions, std::vector<glm::vec3>& normals) {

				// We assume positions.size() is a multiple of 3, representing triangles
				if (positions.size() % 3 != 0) {
					return FAILURE; // or handle the error as appropriate
				}

				// calculate normals for each vertex.
				normals.clear();
				normals.resize(positions.size(), glm::vec3(0));

				for (size_t i = 0; i < positions.size(); i += 3)
				{
					glm::vec3 p0 = positions[i];
					glm::vec3 p1 = positions[i + 1];
					glm::vec3 p2 = positions[i + 2];


					glm::vec3 edge1 = p1 - p0;
					glm::vec3 edge2 = p2 - p0;

					glm::vec3 nFace = glm::normalize(glm::cross(edge1, edge2));

					normals[i] += nFace;
					normals[i + 1] += nFace;
					normals[i + 2] += nFace;
				}

				for (size_t i = 0; i < normals.size(); i++)
				{
					normals[i] = glm::normalize(normals[i]);
				}

				return SUCCESS;
			}

			BReturn GenerateCube(std::vector<glm::vec3>& positions, std::vector<uint16_t>& indices) {
				// Define the 8 vertices of the cube
				positions = {
					// Front face (CCW)
					{-0.5f, -0.5f, 0.5f},
					{0.5f, -0.5f, 0.5f},
					{0.5f, 0.5f, 0.5f},
					{-0.5f, 0.5f, 0.5f},
					// Back face (CCW)
					{-0.5f, -0.5f, -0.5f},
					{0.5f, -0.5f, -0.5f},
					{0.5f, 0.5f, -0.5f},
					{-0.5f, 0.5f, -0.5f},
					// Right face (CCW)
					{0.5f, -0.5f, 0.5f},
					{0.5f, -0.5f, -0.5f},
					{0.5f, 0.5f, -0.5f},
					{0.5f, 0.5f, 0.5f},
					// Left face (CCW)
					{-0.5f, -0.5f, -0.5f},
					{-0.5f, -0.5f, 0.5f},
					{-0.5f, 0.5f, 0.5f},
					{-0.5f, 0.5f, -0.5f},
					// Top face (CCW)
					{-0.5f, 0.5f, 0.5f},
					{0.5f, 0.5f, 0.5f},
					{0.5f, 0.5f, -0.5f},
					{-0.5f, 0.5f, -0.5f},
					// Bottom face (CCW)
					{-0.5f, -0.5f, -0.5f},
					{0.5f, -0.5f, -0.5f},
					{0.5f, -0.5f, 0.5f},
					{-0.5f, -0.5f, 0.5f}
				};

				// Define the indices for the cube (6 faces, 2 triangles per face, 3 indices per triangle)
				indices = {
					// Front face (CCW)
					0, 1, 2, 2, 3, 0,
					// Back face (CCW)
					4, 5, 6, 6, 7, 4,
					// Right face (CCW)
					8, 9, 10, 10, 11, 8,
					// Left face (CCW)
					12, 13, 14, 14, 15, 12,
					// Top face (CCW)
					16, 17, 18, 18, 19, 16,
					// Bottom face (CCW)
					20, 21, 22, 22, 23, 20
				};

				return SUCCESS;
			}



			BReturn GenerateUVSphere(float radius, int latitudeSegments, int longitudeSegments, std::vector<glm::vec3>& positions, std::vector<uint16_t>& indices) {
				positions.clear();
				indices.clear();

				for (int lat = 0; lat <= latitudeSegments; ++lat) {
					float theta = lat * glm::pi<float>() / latitudeSegments;
					float sinTheta = sin(theta);
					float cosTheta = cos(theta);

					for (int lon = 0; lon <= longitudeSegments; ++lon) {
						float phi = lon * 2 * glm::pi<float>() / longitudeSegments;
						float sinPhi = sin(phi);
						float cosPhi = cos(phi);

						glm::vec3 position = glm::vec3(cosPhi * sinTheta, cosTheta, sinPhi * sinTheta);
						glm::vec2 uv = glm::vec2(lon / static_cast<float>(longitudeSegments), 1.0f - lat / static_cast<float>(latitudeSegments));

						positions.push_back(position * radius);
					}
				}

				for (int lat = 0; lat < latitudeSegments; ++lat) {
					for (int lon = 0; lon < longitudeSegments; ++lon) {
						uint16_t first = lat * (longitudeSegments + 1) + lon;
						uint16_t second = first + longitudeSegments + 1;

						indices.push_back(first);
						indices.push_back(first + 1);
						indices.push_back(second);

						indices.push_back(second);
						indices.push_back(first + 1);
						indices.push_back(second + 1);
					}
				}

				return SUCCESS;
			}

			BReturn GenerateIcosahedron(std::vector<glm::vec3>& positions, std::vector<uint16_t>& indices) {
				const float t = (1.0f + std::sqrt(5.0f)) / 2.0f;

				positions = {
					{ -1,  t,  0 },
					{  1,  t,  0 },
					{ -1, -t,  0 },
					{  1, -t,  0 },
					{  0, -1,  t },
					{  0,  1,  t },
					{  0, -1, -t },
					{  0,  1, -t },
					{  t,  0, -1 },
					{  t,  0,  1 },
					{ -t,  0, -1 },
					{ -t,  0,  1 }
				};

				indices = {
					0, 11, 5, 0, 5, 1, 0, 1, 7, 0, 7, 10, 0, 10, 11,
					1, 5, 9, 5, 11, 4, 11, 10, 2, 10, 7, 6, 7, 1, 8,
					3, 9, 4, 3, 4, 2, 3, 2, 6, 3, 6, 8, 3, 8, 9,
					4, 9, 5, 2, 4, 11, 6, 2, 10, 8, 6, 7, 9, 8, 1
				};

				for (auto& pos : positions) {
					pos = glm::normalize(pos);
				}

				return SUCCESS;
			}

			BReturn GenerateIcoSphere(std::vector<glm::vec3>& positions, std::vector<uint16_t>& indices, float radius = 1, int subdivisions = 4) {
				GenerateIcosahedron(positions, indices);

				for (int i = 0; i < subdivisions; i++) {
					Subdivide(positions, indices);
				}

				for (auto& pos : positions) {
					pos *= radius;
				}

				return SUCCESS;
			}

			// FIXME! Capsule doesnt appear as it should!
			BReturn GenerateCapsule(float radius, float height, int longitudeSegments, int latitudeSegments, std::vector<glm::vec3>& positions, std::vector<uint16_t>& indices) {
				positions.clear();
				indices.clear();

				// Generate cylinder vertices
				float halfHeight = height * 0.5f;
				float sectorStep = 2 * glm::pi<float>() / longitudeSegments;
				float stackStep = glm::pi<float>() / latitudeSegments;

				for (int i = 0; i <= latitudeSegments; ++i) {
					float stackAngle = glm::pi<float>() / 2 - i * stackStep; // starting from pi/2 to -pi/2
					float xy = radius * cosf(stackAngle); // r * cos(u)
					float z = radius * sinf(stackAngle);  // r * sin(u)

					// add (longitudeSegments+1) vertices per stack
					// the first and last vertices have same position and normal, but different tex coords
					for (int j = 0; j <= longitudeSegments; ++j) {
						float sectorAngle = j * sectorStep;  // starting from 0 to 2pi

						// vertex position (x, y, z)
						float x = xy * cosf(sectorAngle); // r * cos(u) * cos(v)
						float y = xy * sinf(sectorAngle); // r * cos(u) * sin(v)
						positions.push_back(glm::vec3(x, y, z + halfHeight));

						// vertex position (x, y, z)
						x = xy * cosf(sectorAngle + sectorStep); // r * cos(u) * cos(v+du)
						y = xy * sinf(sectorAngle + sectorStep); // r * cos(u) * sin(v+du)
						positions.push_back(glm::vec3(x, y, z + halfHeight));
					}
				}

				// Generate cylinder indices
				for (int i = 0; i < latitudeSegments; ++i) {
					for (int j = 0; j < longitudeSegments; ++j) {
						int first = (i * (longitudeSegments + 1) + j) * 2;
						int second = first + longitudeSegments + 1;

						indices.push_back(first);
						indices.push_back(second);
						indices.push_back(first + 1);

						indices.push_back(second);
						indices.push_back(second + 1);
						indices.push_back(first + 1);
					}
				}

				// Generate half-sphere vertices
				int verticesStartIndex = positions.size();
				float sectorStepHalfSphere = 2 * glm::pi<float>() / longitudeSegments;
				float stackStepHalfSphere = glm::pi<float>() / (latitudeSegments * 2); // Only half of the latitude

				for (int i = 0; i <= latitudeSegments; ++i) {
					float stackAngle = glm::pi<float>() / 2 - i * stackStepHalfSphere; // starting from pi/2 to 0
					float xy = radius * cosf(stackAngle); // r * cos(u)
					float z = radius * sinf(stackAngle);  // r * sin(u)

					// add (longitudeSegments+1) vertices per stack
					// the first and last vertices have same position and normal, but different tex coords
					for (int j = 0; j <= longitudeSegments; ++j) {
						float sectorAngle = j * sectorStepHalfSphere;  // starting from 0 to 2pi

						// vertex position (x, y, z)
						float x = xy * cosf(sectorAngle); // r * cos(u) * cos(v)
						float y = xy * sinf(sectorAngle); // r * cos(u) * sin(v)
						positions.push_back(glm::vec3(x, y, z + halfHeight));

						// vertex position (x, y, z)
						x = xy * cosf(sectorAngle + sectorStepHalfSphere); // r * cos(u) * cos(v+du)
						y = xy * sinf(sectorAngle + sectorStepHalfSphere); // r * cos(u) * sin(v+du)
						positions.push_back(glm::vec3(x, y, z + halfHeight));
					}
				}

				// Generate half-sphere indices
				int halfSphereVerticesStartIndex = verticesStartIndex;
				for (int i = 0; i < latitudeSegments; ++i) {
					for (int j = 0; j < longitudeSegments; ++j) {
						int first = (i * (longitudeSegments + 1) + j) * 2 + halfSphereVerticesStartIndex;
						int second = first + longitudeSegments + 1;

						indices.push_back(first);
						indices.push_back(second);
						indices.push_back(first + 1);

						indices.push_back(second);
						indices.push_back(second + 1);
						indices.push_back(first + 1);
					}
				}

				return SUCCESS;
			}

			BReturn GenerateNGon(int n, float radius, float height, float offset, std::vector<glm::vec3>& positions, std::vector<uint16_t>& indices) {
				if (n == 0)
					return FAILURE;

				positions.clear();
				indices.clear();

				// vertices
				float step = (2.0f * glm::pi<float>() / n);

				{
					// Bottom face vertices
					for (int i = 0; i < n; ++i) {
						float angle = i * step - glm::radians(offset);
						float x = radius * cos(angle);
						float z = radius * sin(angle);
						positions.push_back({ glm::vec3(x, -height / 2, z) }); // Bottom face vertex
					}

					// Top face vertices
					for (int i = 0; i < n; ++i) {
						float angle = i * step - glm::radians(offset);
						float x = radius * cos(angle);
						float z = radius * sin(angle);
						positions.push_back({ glm::vec3(x, height / 2, z) }); // Top face vertex
					}
				}
				// indices
				{
					// Indices for bottom face (CCW)
					for (int i = 1; i < n - 1; ++i) {
						indices.push_back(0);
						indices.push_back(i);
						indices.push_back(i + 1);
					}

					// Indices for top face (CCW)
					int offset = n;
					for (int i = 1; i < n - 1; ++i) {
						indices.push_back(offset);
						indices.push_back(offset + i + 1);
						indices.push_back(offset + i);
					}

					// Indices for side faces
					for (int i = 0; i < n; ++i) {
						int next = (i + 1) % n;

						// First triangle
						indices.push_back(i);
						indices.push_back(offset + i);
						indices.push_back(next);

						// Second triangle
						indices.push_back(offset + i);
						indices.push_back(offset + next);
						indices.push_back(next);
					}

				}
				return SUCCESS;
			}

		}
	

	}
}