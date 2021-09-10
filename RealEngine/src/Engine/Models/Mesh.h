#pragma once

#include <fbxsdk.h>
#include "OpenGL/VertexBuffer.h"

namespace RealEngine {

    struct Material
    {
        glm::vec3 Albedo = { 1.0f, 1.0f, 1.0f };
        float Metallic = 0.0f;
        float Roughness = 0.5f;
        float AO = 1.0f;
    };

    class Mesh 
    {
    public:
        Mesh() {};
        ~Mesh() {};// delete VerticesBase; delete IndicesBase; };

        //std::vector<Vertex>& GetVertices() { return m_Vertices; }
        //std::vector<uint32_t>& GetIndices() { return m_Indices; }

        Material m_Material;

        //std::vector<Vertex> m_Vertices;
        //std::vector<uint32_t> m_Indices;
        uint32_t QuadIndexCount = 0;
        uint32_t QuadVerticesCount = 0;

        Vertex* VerticesPtr = nullptr;
        Vertex* VerticesBase = nullptr;

        uint32_t* IndicesPtr = nullptr;
        uint32_t* IndicesBase = nullptr;


        std::string FilePath = "";
        std::string REMFilePath = "";
    private:
    };


}