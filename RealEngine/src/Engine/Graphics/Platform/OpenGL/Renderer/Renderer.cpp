#include "repch.h"
#include "Renderer.h"

namespace RealEngine {

	Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData;

    static RendererData s_Data;

    void Renderer::BeginScene(OrthographicCamera& camera)
	{
		m_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();

        s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;
	}

	void Renderer::EndScene()
	{
        uint32_t dataSize = (uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase;
        s_Data.VertexBuffer.SetData(s_Data.QuadVertexBufferBase, dataSize);


        DrawIndexed();
	}

    void Renderer::Init()
    {
        unsigned int indices[12] = {
            0, 1, 2, 2, 3, 0,
            4, 5, 6, 6, 7, 4
        };

        VertexBufferLayout m_Layout;

        std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec4 a_Position;
			layout(location = 1) in vec4 a_Color;
            
			uniform mat4 u_ViewProjection;
            uniform mat4 u_Transform;

            out vec4 v_Color;

			void main()
			{
                v_Color = a_Color;
				gl_Position = u_ViewProjection * a_Position;	
            }
		)";

        std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;
            
            in vec4 v_Color;
                        
			void main()
			{
            	color = v_Color;
			}
		)";


        s_Data.VertexArray.Create();
        s_Data.Shader.Create(vertexSrc, fragmentSrc);
        s_Data.VertexBuffer.Create();

        uint32_t* quadIndices = new uint32_t[s_Data.MaxIndices];

        uint32_t offset = 0;
        for (uint32_t i = 0; i < s_Data.MaxIndices; i += 6)
        {
            quadIndices[i + 0] = offset + 0;
            quadIndices[i + 1] = offset + 1;
            quadIndices[i + 2] = offset + 2;

            quadIndices[i + 3] = offset + 2;
            quadIndices[i + 4] = offset + 3;
            quadIndices[i + 5] = offset + 0;

            offset += 4;
        }

        s_Data.IndexBuffer.Create(quadIndices, s_Data.MaxIndices);
        //s_Data.QuadVertexArray->SetIndexBuffer(quadIB);
        delete[] quadIndices;


        s_Data.VertexBuffer.Bind();
        s_Data.Shader.Bind();
        s_Data.IndexBuffer.Bind();

        m_Layout.Push<float>(3);
        m_Layout.Push<float>(4);
        s_Data.VertexArray.Addbuffer(s_Data.VertexBuffer, m_Layout);

        s_Data.QuadVertexBufferBase = new Vertex[s_Data.MaxVertices];
    }

    void Renderer::DrawQuad(glm::vec3 position, glm::vec2 size, glm::vec4 color)
    {
        s_Data.QuadVertexBufferPtr->Position = { position.x, position.y, position.z };
        s_Data.QuadVertexBufferPtr->Color = { color.x, color.y, color.z, color.w };
        s_Data.QuadVertexBufferPtr++;

        s_Data.QuadVertexBufferPtr->Position = { position.x + size.x, position.y, position.z };
        s_Data.QuadVertexBufferPtr->Color = { color.x, color.y, color.z, color.w };
        s_Data.QuadVertexBufferPtr++;

        s_Data.QuadVertexBufferPtr->Position = { position.x + size.x, position.y + size.y, position.z };
        s_Data.QuadVertexBufferPtr->Color = { color.x, color.y, color.z, color.w };
        s_Data.QuadVertexBufferPtr++;

        s_Data.QuadVertexBufferPtr->Position = { position.x, position.y + size.y, position.z };
        s_Data.QuadVertexBufferPtr->Color = { color.x, color.y, color.z, color.w };

        s_Data.QuadVertexBufferPtr++;
    }

    void Renderer::DrawQuad(float posX, float posY, float posZ, float sizeX, float sizeY, float r, float g, float b, float t)
    {
        s_Data.QuadVertexBufferPtr->Position = { posX, posY, posZ };
        s_Data.QuadVertexBufferPtr->Color = { r, g, b, t };
        s_Data.QuadVertexBufferPtr++;

        s_Data.QuadVertexBufferPtr->Position = { posX + sizeX, posY, posZ };
        s_Data.QuadVertexBufferPtr->Color = { r, g, b, t };
        s_Data.QuadVertexBufferPtr++;

        s_Data.QuadVertexBufferPtr->Position = { posX + sizeX, posY + sizeY, posZ };
        s_Data.QuadVertexBufferPtr->Color = { r, g, b, t };
        s_Data.QuadVertexBufferPtr++;

        s_Data.QuadVertexBufferPtr->Position = { posX, posY + sizeY, posZ };
        s_Data.QuadVertexBufferPtr->Color = { r, g, b, t };

        s_Data.QuadVertexBufferPtr++;
    }

    void Renderer::DrawQuad(Quad quad)
    {
        s_Data.QuadVertexBufferPtr->Position = { quad.Vertex_0.Position.x, quad.Vertex_0.Position.y, quad.Vertex_0.Position.z };
        s_Data.QuadVertexBufferPtr->Color = { quad.Vertex_0.Color.x, quad.Vertex_0.Color.y, quad.Vertex_0.Color.z, quad.Vertex_0.Color.w };
        s_Data.QuadVertexBufferPtr++;

        s_Data.QuadVertexBufferPtr->Position = { quad.Vertex_0.Position.x + quad.Size, quad.Vertex_0.Position.y, quad.Vertex_0.Position.z };
        s_Data.QuadVertexBufferPtr->Color = { quad.Vertex_0.Color.x, quad.Vertex_0.Color.y, quad.Vertex_0.Color.z, quad.Vertex_0.Color.w };
        s_Data.QuadVertexBufferPtr++;

        s_Data.QuadVertexBufferPtr->Position = { quad.Vertex_0.Position.x + quad.Size, quad.Vertex_0.Position.y + quad.Size, quad.Vertex_0.Position.z };
        s_Data.QuadVertexBufferPtr->Color = { quad.Vertex_0.Color.x, quad.Vertex_0.Color.y, quad.Vertex_0.Color.z, quad.Vertex_0.Color.w };
        s_Data.QuadVertexBufferPtr++;

        s_Data.QuadVertexBufferPtr->Position = { quad.Vertex_0.Position.x, quad.Vertex_0.Position.y + quad.Size, quad.Vertex_0.Position.z };
        s_Data.QuadVertexBufferPtr->Color = { quad.Vertex_0.Color.x, quad.Vertex_0.Color.y, quad.Vertex_0.Color.z, quad.Vertex_0.Color.w };

        s_Data.QuadVertexBufferPtr++;
    }

	void Renderer::DrawIndexed()
	{
       	s_Data.Shader.Bind();
        s_Data.Shader.SetUniformMat4f("u_ViewProjection", m_SceneData->ViewProjectionMatrix);
       
		s_Data.VertexArray.Bind();
        s_Data.IndexBuffer.Bind();

		glDrawElements(GL_TRIANGLES, s_Data.IndexBuffer.GetCount(), GL_UNSIGNED_INT, nullptr);
	}


    Quad Renderer::CreateQuad(float x, float y, float z, float size)
    {
        Quad quad;

        quad.Vertex_0.Position = { x, y, z };
        quad.Vertex_0.Color = { 0.18f, 0.6f, 0.96f, 1.0f };

        quad.Vertex_1.Position = { x + size, y, z };
        quad.Vertex_1.Color = { 0.18f, 0.6f, 0.96f, 1.0f };

        quad.Vertex_2.Position = { x + size, y + size, z };
        quad.Vertex_2.Color = { 0.18f, 0.6f, 0.96f, 1.0f };

        quad.Vertex_3.Position = { x, y + size, z };
        quad.Vertex_3.Color = { 0.18f, 0.6f, 0.96f, 1.0f };

        return quad;
    }

    std::array<Vertex, 4> Renderer::CreateQuadByVerticies(float x, float y, float z, float size)
    {

        Vertex v0;
        v0.Position = { x, y, z };
        v0.Color = { 0.18f, 0.6f, 0.96f, 1.0f };

        Vertex v1;
        v1.Position = { x + size, y, z };
        v1.Color = { 0.18f, 0.6f, 0.96f, 1.0f };

        Vertex v2;
        v2.Position = { x + size, y + size, z };
        v2.Color = { 0.18f, 0.6f, 0.96f, 1.0f };

        Vertex v3;
        v3.Position = { x, y + size, z };
        v3.Color = { 0.18f, 0.6f, 0.96f, 1.0f };

        return { v0, v1, v2, v3 };
    }
}
