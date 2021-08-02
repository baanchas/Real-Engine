#include "repch.h"
#include "Renderer.h"

namespace RealEngine {

	Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData;

    static RendererData s_Data;

    void Renderer::BeginScene(Camera& camera, glm::mat4& transform)
    {
        m_SceneData->ViewProjectionMatrix = camera.GetProjection() * glm::inverse(transform);

        s_Data.IndicesOffset = 0;
        s_Data.QuadIndexCount = 0;  
        s_Data.m_EntityID = 0;

        s_Data.VertexBufferPtr = s_Data.VertexBufferBase;
    }

    void Renderer::BeginScene(EditorCamera& camera)
    {
        m_SceneData->ViewProjectionMatrix = camera.GetViewProjection();

        s_Data.QuadIndicesPtr = s_Data.QuadIndices;
        s_Data.IndicesOffset = 0;
        s_Data.QuadIndexCount = 0;
        s_Data.m_EntityID = 0;

        s_Data.VertexBufferPtr = s_Data.VertexBufferBase;
    }

    void Renderer::BeginScene(OrthographicCamera& camera)
	{
		m_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
        
        s_Data.IndicesOffset = 0;
        s_Data.QuadIndexCount = 0;
        s_Data.m_EntityID = 0;

        s_Data.VertexBufferPtr = s_Data.VertexBufferBase;
	}

	void Renderer::EndScene()
	{
    
        for (uint32_t i = 0; i < s_Data.TextureIndex; i++)
        {
            //if (s_Data.TextureSlots[i] != nullptr)
                s_Data.TextureSlots[i].Bind(i);
        }

        s_Data.Shader.SetUniformMat4f("u_ViewProjection", m_SceneData->ViewProjectionMatrix);
        //s_Data.Shader.SetUniform3f("lightPos", );

        uint32_t dataSize = (uint8_t*)s_Data.VertexBufferPtr - (uint8_t*)s_Data.VertexBufferBase;
      
        
        s_Data.IndexBuffer.SetData((void*)s_Data.QuadIndicesPtr, s_Data.QuadIndexCount);
        s_Data.IndexBuffer.Bind();
        s_Data.VertexBuffer.SetData((void*)s_Data.VertexBufferBase, dataSize);

        glDrawElements(GL_TRIANGLES, s_Data.QuadIndexCount, GL_UNSIGNED_INT, nullptr);
	}

    void Renderer::ShutDown()
    {
        delete[] s_Data.VertexBufferBase;
    }

    void Renderer::Init()
    {
        
        //glEnable(GL_LINE_SMOOTH);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_MULTISAMPLE);

        s_Data.VertexArray.Create();
        s_Data.Shader.Create("assets/shaders/Vertex.Shader", "assets/shaders/Fragment.Shader");
        s_Data.VertexBuffer.Create(sizeof(Vertex) * s_Data.MaxVertices);

        VertexBufferLayout m_Layout;

        s_Data.QuadIndices = new uint32_t[s_Data.MaxIndices];

        s_Data.IndexBuffer.Create(s_Data.QuadIndices, s_Data.MaxIndices);

        s_Data.Shader.Bind();
        s_Data.VertexBuffer.Bind();

        m_Layout.Push<float>(3);
        m_Layout.Push<float>(4);
        m_Layout.Push<float>(2);
        m_Layout.Push<float>(3);
        m_Layout.Push<float>(1);
        m_Layout.Push<float>(1);
        m_Layout.Push<int>(1);
        m_Layout.Push<float>(4);
        m_Layout.Push<float>(4);
        m_Layout.Push<float>(4);
        m_Layout.Push<float>(4);
        s_Data.VertexArray.Addbuffer(s_Data.VertexBuffer, m_Layout);

        s_Data.VertexBufferBase = new Vertex[s_Data.MaxVertices];

        int32_t samplers[s_Data.MaxTextureSlots];
        for (uint32_t i = 0; i < s_Data.MaxTextureSlots; i++)
        {
            samplers[i] = i;
        }
        s_Data.Shader.SetUniformIntArray("u_Texture", samplers, s_Data.MaxTextureSlots);

        for (uint32_t i = 0; i < s_Data.TextureIndex; i++)
        {
            //if (s_Data.TextureSlots[i] != nullptr)
                s_Data.TextureSlots[i].SetRendererID(0);
        }

        s_Data.VertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f};
        s_Data.VertexPositions[1] = {  0.5f, -0.5f, 0.0f, 1.0f};
        s_Data.VertexPositions[2] = {  0.5f,  0.5f, 0.0f, 1.0f};
        s_Data.VertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f};
    }

    void Renderer::SetUniform3f(const std::string& name, const glm::vec3& position)
    {
        s_Data.Shader.SetUniform3f(name, position.x, position.y, position.z);
    }

    void Renderer::DrawQuad(const glm::mat4& transform, const glm::vec4& color, int entityID)
    {
        s_Data.VertexBufferPtr->Position = s_Data.VertexPositions[0];
        s_Data.VertexBufferPtr->Color = { color.x, color.y, color.z, color.w };
        s_Data.VertexBufferPtr->TexCoord = { 0.0f, 0.0f };
        s_Data.VertexBufferPtr->Normal = { 0.0f, 0.0f, 1.0f };
        s_Data.VertexBufferPtr->TexId = 32.0f;
        s_Data.VertexBufferPtr->TilingFactor = 1.0f;
        s_Data.VertexBufferPtr->entityID = entityID;
        s_Data.VertexBufferPtr->Matrix = transform;
        s_Data.VertexBufferPtr++;

        s_Data.VertexBufferPtr->Position = s_Data.VertexPositions[1];
        s_Data.VertexBufferPtr->Color = { color.x, color.y, color.z, color.w };
        s_Data.VertexBufferPtr->TexCoord = { 1.0f, 0.0f };
        s_Data.VertexBufferPtr->Normal = { 0.0f, 0.0f, 1.0f };
        s_Data.VertexBufferPtr->TexId = 32.0f;
        s_Data.VertexBufferPtr->TilingFactor = 1.0f;
        s_Data.VertexBufferPtr->entityID = entityID;
        s_Data.VertexBufferPtr->Matrix = transform;
        s_Data.VertexBufferPtr++;

        s_Data.VertexBufferPtr->Position = s_Data.VertexPositions[2];
        s_Data.VertexBufferPtr->Color = { color.x, color.y, color.z, color.w };
        s_Data.VertexBufferPtr->TexCoord = { 1.0f, 1.0f };
        s_Data.VertexBufferPtr->Normal = { 0.0f, 0.0f, 1.0f };
        s_Data.VertexBufferPtr->TexId = 32.0f;
        s_Data.VertexBufferPtr->TilingFactor = 1.0f;
        s_Data.VertexBufferPtr->entityID = entityID;
        s_Data.VertexBufferPtr->Matrix = transform;
        s_Data.VertexBufferPtr++;

        s_Data.VertexBufferPtr->Position = s_Data.VertexPositions[3];
        s_Data.VertexBufferPtr->Color = { color.x, color.y, color.z, color.w };
        s_Data.VertexBufferPtr->TexCoord = { 0.0f, 1.0f };
        s_Data.VertexBufferPtr->Normal = { 0.0f, 0.0f, 1.0f };
        s_Data.VertexBufferPtr->TexId = 32.0f;
        s_Data.VertexBufferPtr->TilingFactor = 1.0f;
        s_Data.VertexBufferPtr->entityID = entityID;
        s_Data.VertexBufferPtr->Matrix = transform;
        s_Data.VertexBufferPtr++;

        s_Data.QuadIndicesPtr[s_Data.QuadIndexCount + 0] = s_Data.IndicesOffset + 0;
        s_Data.QuadIndicesPtr[s_Data.QuadIndexCount + 1] = s_Data.IndicesOffset + 1;
        s_Data.QuadIndicesPtr[s_Data.QuadIndexCount + 2] = s_Data.IndicesOffset + 2;

        s_Data.QuadIndicesPtr[s_Data.QuadIndexCount + 3] = s_Data.IndicesOffset + 3;
        s_Data.QuadIndicesPtr[s_Data.QuadIndexCount + 4] = s_Data.IndicesOffset + 2;
        s_Data.QuadIndicesPtr[s_Data.QuadIndexCount + 5] = s_Data.IndicesOffset + 0;


        //s_Data.IndicesIndex += 6;
        s_Data.IndicesOffset += 4;
        s_Data.QuadIndexCount += 6;
    }

    void Renderer::DrawQuad(const glm::mat4& transform, Texture2D& texture, float tilingFactor, float entityID)
    {
        float textureIndex = 0.0f;

        for (uint32_t i = 1; i < s_Data.TextureIndex; i++)
        {
            if (s_Data.TextureSlots[i] == texture)
            {
                textureIndex = (float)i;
                break;
            }
        }

        if (textureIndex == 0.0f)
        {
            textureIndex = (float)s_Data.TextureIndex;

            s_Data.TextureSlots[s_Data.TextureIndex] = texture;
            s_Data.TextureIndex++;
        }

        s_Data.VertexBufferPtr->Position = s_Data.VertexPositions[0];
        s_Data.VertexBufferPtr->Color = { 1.0f, 1.0f, 1.0f, 1.0f };
        s_Data.VertexBufferPtr->TexCoord = { 0.0f, 0.0f };
        s_Data.VertexBufferPtr->Normal = { 0.0f, 0.0f, 1.0f };
        s_Data.VertexBufferPtr->TexId = textureIndex;
        s_Data.VertexBufferPtr->TilingFactor = tilingFactor;
        s_Data.VertexBufferPtr->entityID = entityID;
        s_Data.VertexBufferPtr->Matrix = transform;
        s_Data.VertexBufferPtr++;

        s_Data.VertexBufferPtr->Position = s_Data.VertexPositions[1];
        s_Data.VertexBufferPtr->Color = { 1.0f, 1.0f, 1.0f, 1.0f };
        s_Data.VertexBufferPtr->TexCoord = { 1.0f, 0.0f };
        s_Data.VertexBufferPtr->Normal = { 0.0f, 0.0f, 1.0f };
        s_Data.VertexBufferPtr->TexId = textureIndex;
        s_Data.VertexBufferPtr->TilingFactor = tilingFactor;
        s_Data.VertexBufferPtr->entityID = entityID;
        s_Data.VertexBufferPtr->Matrix = transform;
        s_Data.VertexBufferPtr++;

        s_Data.VertexBufferPtr->Position = s_Data.VertexPositions[2];
        s_Data.VertexBufferPtr->Color = { 1.0f, 1.0f, 1.0f, 1.0f };
        s_Data.VertexBufferPtr->TexCoord = { 1.0f, 1.0f };
        s_Data.VertexBufferPtr->Normal = { 0.0f, 0.0f, 1.0f };
        s_Data.VertexBufferPtr->TexId = textureIndex;
        s_Data.VertexBufferPtr->TilingFactor = tilingFactor;
        s_Data.VertexBufferPtr->entityID = entityID;
        s_Data.VertexBufferPtr->Matrix = transform;
        s_Data.VertexBufferPtr++;
      
        s_Data.VertexBufferPtr->Position = s_Data.VertexPositions[3];
        s_Data.VertexBufferPtr->Color = { 1.0f, 1.0f, 1.0f, 1.0f };
        s_Data.VertexBufferPtr->TexCoord = { 0.0f, 1.0f };
        s_Data.VertexBufferPtr->Normal = { 0.0f, 0.0f, 1.0f };
        s_Data.VertexBufferPtr->TexId = textureIndex;
        s_Data.VertexBufferPtr->TilingFactor = tilingFactor;
        s_Data.VertexBufferPtr->entityID = entityID;
        s_Data.VertexBufferPtr->Matrix = transform;
        s_Data.VertexBufferPtr++;

        s_Data.QuadIndicesPtr[s_Data.QuadIndexCount + 0] = s_Data.IndicesOffset + 0;
        s_Data.QuadIndicesPtr[s_Data.QuadIndexCount + 1] = s_Data.IndicesOffset + 1;
        s_Data.QuadIndicesPtr[s_Data.QuadIndexCount + 2] = s_Data.IndicesOffset + 2;

        s_Data.QuadIndicesPtr[s_Data.QuadIndexCount + 3] = s_Data.IndicesOffset + 3;
        s_Data.QuadIndicesPtr[s_Data.QuadIndexCount + 4] = s_Data.IndicesOffset + 2;
        s_Data.QuadIndicesPtr[s_Data.QuadIndexCount + 5] = s_Data.IndicesOffset + 0;


        //s_Data.IndicesIndex += 6;
        s_Data.IndicesOffset += 4;
//        s_Data.m_EntityID++;
        s_Data.QuadIndexCount += 6;
    }

    void Renderer::DrawQuad(const glm::vec3& position, const glm::vec2& size, const float rotation, const  glm::vec4& color, const int entityID, float tf)
    {
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(position.x, position.y, position.z)) *
            glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f }) *
            glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

        DrawQuad(transform, color, entityID);
    }

    void Renderer::DrawQuad(float posX, float posY, float posZ, float sizeX, float sizeY, float rotation, float r, float g, float b, float t, int entityID, float tf)
    {
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(posX, posY, posZ)) *
            glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f }) *
            glm::scale(glm::mat4(1.0f), { sizeX, sizeY, 1.0f });

        glm::vec4 color = { r, g, b, t };

        DrawQuad(transform, color, entityID);

      
    }

    void Renderer::DrawQuad(float posX, float posY, float posZ, float sizeX, float sizeY, Texture2D& texture, float tilingFactor)
    {
        float textureIndex = 0.0f;
        
        for (uint32_t i = 1; i < s_Data.TextureIndex; i++)
        {
            if (s_Data.TextureSlots[i] == texture)
            {
                textureIndex = (float)i;
                break;
            }
        }

        if (textureIndex == 0.0f)
        {
            textureIndex = (float)s_Data.TextureIndex;

            s_Data.TextureSlots[s_Data.TextureIndex] = texture;
            s_Data.TextureIndex++;
        }

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(posX, posY, posZ)) *
            glm::rotate(glm::mat4(1.0f), glm::radians(texture.GetRotation()), { 0.0f, 0.0f, 1.0f }) *
            glm::scale(glm::mat4(1.0f), { sizeX, sizeY, 1.0f });

        s_Data.VertexBufferPtr->Position = transform * s_Data.VertexPositions[0];
        //s_Data.VertexBufferPtr->Color = { 1.0f, 1.0f, 1.0f, 1.0f };
        s_Data.VertexBufferPtr->Color = { 0.0f, 0.0f, 0.0f, 1.0f };
        s_Data.VertexBufferPtr->TexCoord = { 0.0f, 0.0f };
        s_Data.VertexBufferPtr->TexId = textureIndex;
        s_Data.VertexBufferPtr->TilingFactor = tilingFactor;
        s_Data.VertexBufferPtr->entityID = s_Data.m_EntityID;
        s_Data.VertexBufferPtr++;

        s_Data.VertexBufferPtr->Position = transform * s_Data.VertexPositions[1];
        //s_Data.VertexBufferPtr->Color = { 1.0f, 1.0f, 1.0f, 1.0f };
        s_Data.VertexBufferPtr->Color = { 0.0f, 0.0f, 0.0f, 1.0f };
        s_Data.VertexBufferPtr->TexCoord = { 1.0f, 0.0f };
        s_Data.VertexBufferPtr->TexId = textureIndex;
        s_Data.VertexBufferPtr->TilingFactor = tilingFactor;
        s_Data.VertexBufferPtr->entityID = s_Data.m_EntityID;
        s_Data.VertexBufferPtr++;

        s_Data.VertexBufferPtr->Position = transform * s_Data.VertexPositions[2];
        //s_Data.VertexBufferPtr->Color = { 1.0f, 1.0f, 1.0f, 1.0f };
        s_Data.VertexBufferPtr->Color = { 0.0f, 0.0f, 0.0f, 1.0f };
        s_Data.VertexBufferPtr->TexCoord = { 1.0f, 1.0f };
        s_Data.VertexBufferPtr->TexId = textureIndex;
        s_Data.VertexBufferPtr->TilingFactor = tilingFactor;
        s_Data.VertexBufferPtr->entityID = s_Data.m_EntityID;
        s_Data.VertexBufferPtr++;

        s_Data.VertexBufferPtr->Position = transform * s_Data.VertexPositions[3];
        //s_Data.VertexBufferPtr->Color = { 1.0f, 1.0f, 1.0f, 1.0f };
        s_Data.VertexBufferPtr->Color = { 0.0f, 0.0f, 0.0f, 1.0f };
        s_Data.VertexBufferPtr->TexCoord = { 0.0f, 1.0f };
        s_Data.VertexBufferPtr->TexId = textureIndex;
        s_Data.VertexBufferPtr->TilingFactor = tilingFactor;
        s_Data.VertexBufferPtr->entityID = s_Data.m_EntityID;
        s_Data.VertexBufferPtr++;

        s_Data.m_EntityID++;
        s_Data.QuadIndexCount += 6;
    }
    
    void Renderer::DrawQuad(glm::vec3& position, glm::vec2& size, Texture2D& texture, float tilingFactor)
    {
        float textureIndex = 0.0f;

        for (uint32_t i = 1; i < s_Data.TextureIndex; i++)
        {
            if (s_Data.TextureSlots[i] == texture)
            {
                textureIndex = (float)i;
                break;
            }
        }

        if (textureIndex == 0.0f)
        {
            textureIndex = (float)s_Data.TextureIndex;

            s_Data.TextureSlots[s_Data.TextureIndex] = texture;
            s_Data.TextureIndex++;
        }

        DrawQuad(position.x, position.y, position.z, size.x, size.y, texture, tilingFactor);

    }

    void Renderer::DrawTriangle(const glm::vec3& position, const glm::vec2& size, const float rotation, const glm::vec4& color, float tf)
    {
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(position.x, position.y, position.z)) *
            glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f }) *
            glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });


        s_Data.VertexBufferPtr->Position = transform * s_Data.VertexPositions[0];
        s_Data.VertexBufferPtr->Color = { color.r, color.g, color.b, color.t };
        s_Data.VertexBufferPtr->TexCoord = { 0.0f, 0.0f };
        s_Data.VertexBufferPtr->TexId = 32.0f;
        s_Data.VertexBufferPtr->TilingFactor = 1.0f;
        s_Data.VertexBufferPtr->entityID = s_Data.m_EntityID;
        s_Data.VertexBufferPtr++;

        s_Data.VertexBufferPtr->Position = transform * s_Data.VertexPositions[1];
        s_Data.VertexBufferPtr->Color = { color.r, color.g, color.b, color.t };
        s_Data.VertexBufferPtr->TexCoord = { 1.0f, 0.0f };
        s_Data.VertexBufferPtr->TexId = 32.0f;
        s_Data.VertexBufferPtr->TilingFactor = 1.0f;
        s_Data.VertexBufferPtr->entityID = s_Data.m_EntityID;
        s_Data.VertexBufferPtr++;

        s_Data.VertexBufferPtr->Position = transform * s_Data.VertexPositions[2];
        s_Data.VertexBufferPtr->Color = { color.r, color.g, color.b, color.t };
        s_Data.VertexBufferPtr->TexCoord = { 1.0f, 1.0f };
        s_Data.VertexBufferPtr->TexId = 32.0f;
        s_Data.VertexBufferPtr->TilingFactor = 1.0f;
        s_Data.VertexBufferPtr->entityID = s_Data.m_EntityID;
        s_Data.VertexBufferPtr++;


        s_Data.QuadIndicesPtr[s_Data.QuadIndexCount + 0] = s_Data.IndicesOffset + 0;
        s_Data.QuadIndicesPtr[s_Data.QuadIndexCount + 1] = s_Data.IndicesOffset + 1;
        s_Data.QuadIndicesPtr[s_Data.QuadIndexCount + 2] = s_Data.IndicesOffset + 2;

        s_Data.IndicesOffset += 3;
        s_Data.QuadIndexCount += 3;
    }

    void Renderer::DrawModel(const glm::mat4& transform, const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, int entityID)
    {
        for (auto& index : indices)
        {
            s_Data.QuadIndicesPtr[s_Data.QuadIndexCount] = s_Data.IndicesOffset + index;
            s_Data.QuadIndexCount += 1;
        }
        for (auto& vertex : vertices)
        {
            s_Data.VertexBufferPtr->Position = vertex.Position;
            s_Data.VertexBufferPtr->Color = vertex.Color;
            s_Data.VertexBufferPtr->TexCoord = vertex.TexCoord;
            s_Data.VertexBufferPtr->Normal = vertex.Normal;
            s_Data.VertexBufferPtr->TexId = 32.0f;
            s_Data.VertexBufferPtr->TilingFactor = 1.0f;
            s_Data.VertexBufferPtr->entityID = entityID;
            s_Data.VertexBufferPtr->Matrix = transform;
            s_Data.VertexBufferPtr++;
            s_Data.IndicesOffset++;
        }


    }

    void Renderer::DrawMesh(const glm::mat4& transform, Mesh& mesh, int entityID)
    {
        for (auto& index : mesh.GetIndices())
        {
            s_Data.QuadIndicesPtr[s_Data.QuadIndexCount] = s_Data.IndicesOffset + index;
            s_Data.QuadIndexCount += 1;
        }
        for (auto& vertex : mesh.GetVertices())
        {
            s_Data.VertexBufferPtr->Position = vertex.Position;
            s_Data.VertexBufferPtr->Color = vertex.Color;
            s_Data.VertexBufferPtr->TexCoord = vertex.TexCoord;
            s_Data.VertexBufferPtr->Normal = vertex.Normal;
            s_Data.VertexBufferPtr->TexId = 32.0f;
            s_Data.VertexBufferPtr->TilingFactor = 1.0f;
            s_Data.VertexBufferPtr->entityID = entityID;
            s_Data.VertexBufferPtr->Matrix = transform;
            s_Data.VertexBufferPtr++;
            s_Data.IndicesOffset++;
        }


    }

    void Renderer::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
    {
        glViewport(x, y, width, height);
    }
   
}
