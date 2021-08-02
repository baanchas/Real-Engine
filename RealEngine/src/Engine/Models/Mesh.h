#pragma once

#include <fbxsdk.h>
#include "OpenGL/VertexBuffer.h"

namespace RealEngine {

    class Mesh 
    {
    public:

        std::vector<Vertex>& GetVertices() { return m_Vertices; }
        std::vector<uint32_t>& GetIndices() { return m_Indices; }
    private:

        std::vector<Vertex> m_Vertices;
        std::vector<uint32_t> m_Indices;
    };

}