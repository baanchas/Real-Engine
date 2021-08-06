#pragma once

#include <fbxsdk.h>
#include "OpenGL/VertexBuffer.h"

namespace RealEngine {

    struct Material
    {
        glm::vec3 Ambient;
        glm::vec3 Diffuse;
        glm::vec3 Specular;
        float Shininess;

        glm::vec3 Albedo;
        float Metallic;
        float Roughness;
        float AO;
    };

    class Mesh 
    {
    public:
        ~Mesh() {
            
        };

        std::vector<Vertex>& GetVertices() { return m_Vertices; }
        std::vector<uint32_t>& GetIndices() { return m_Indices; }

        Material m_Material;

        std::vector<Vertex> m_Vertices;
        std::vector<uint32_t> m_Indices;

        Vertex* VerticesPtr = nullptr;
        Vertex* VerticesBase = nullptr;

        uint32_t* IndicesPtr = nullptr;
        uint32_t* IndicesBase = nullptr;

        uint32_t QuadIndexCount = 0;
    private:
    };


}