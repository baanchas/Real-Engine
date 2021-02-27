#include "repch.h"
#include "VertexBuffer.h"


namespace RealEngine {

    VertexBuffer::VertexBuffer(const void* data, unsigned int size)
    {
        glGenBuffers(1, &m_RendererID);
        glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 1000, nullptr, GL_DYNAMIC_DRAW);
    }

    VertexBuffer::~VertexBuffer()
    {
        glDeleteBuffers(1, &m_RendererID);
    }

    void VertexBuffer::Bind()
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    }

    void VertexBuffer::UnBind()
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    std::array<Vertex, 4> VertexBuffer::CreateQuad(float x, float y, float size)
    {
        Vertex v0;
        v0.Position = { x, y };
        v0.Color = { 0.18f, 0.6f, 0.96f, 1.0f };

        Vertex v1;
        v1.Position = { x + size, y };
        v1.Color = { 0.18f, 0.6f, 0.96f, 1.0f };

        Vertex v2;
        v2.Position = { x + size, y + size };
        v2.Color = { 0.18f, 0.6f, 0.96f, 1.0f };

        Vertex v3;
        v3.Position = { x, y + size };
        v3.Color = { 0.18f, 0.6f, 0.96f, 1.0f };

        return { v0, v1, v2, v3 };
    }

}