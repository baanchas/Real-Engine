#include "repch.h"
#include "Renderer.h"
#include "RenderCommand.h"

namespace RealEngine {

	Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData;

    static RendererData s_Data;

    void Renderer::Init()
    {
        RenderCommand::Enable(GL_BLEND);
        RenderCommand::SetBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        RenderCommand::Enable(GL_DEPTH_TEST);
        RenderCommand::Enable(GL_MULTISAMPLE);

        // SkyBox Scene

        std::vector<std::string> faces
        {
            "assets/skyboxes/quarry01_blurred/right.png",
            "assets/skyboxes/quarry01_blurred/left.png",
            "assets/skyboxes/quarry01_blurred/top.png",
            "assets/skyboxes/quarry01_blurred/bottom.png",
            "assets/skyboxes/quarry01_blurred/front.png",
            "assets/skyboxes/quarry01_blurred/back.png"
        };

        s_Data.CubeMap.LoadCubeMapTexture(faces);

        for (uint32_t i = 0; i < 36; i++)
        {
            s_Data.SkyBoxCubeMapVertexIndices[i] = i;
        }

        s_Data.SkyboxVertexArray.Create();
        s_Data.SkyboxShader.Create("assets/shaders/SkyboxVertex.Shader", "assets/shaders/SkyboxFragment.Shader");
        s_Data.SkyboxVertexBuffer.Create(sizeof(float) * s_Data.SkyBoxCubeMapVerticesCount);
        s_Data.SkyboxIndexBuffer.Create(nullptr, s_Data.SkyBoxCubeMapIndicesCount);

        VertexBufferLayout m_SkyboxLayout;
        m_SkyboxLayout.Push<float>(3);
        s_Data.SkyboxVertexArray.Addbuffer(s_Data.SkyboxVertexBuffer, m_SkyboxLayout);

        // Main Scene

        s_Data.VertexArray.Create();
        s_Data.Shader.Create("assets/shaders/Vertex.Shader", "assets/shaders/Fragment.Shader");
        s_Data.VertexBuffer.Create(sizeof(Vertex) * s_Data.MaxVertices);
        s_Data.VertexBuffer.Bind();

        VertexBufferLayout m_Layout;

        s_Data.QuadIndexBufferBase = new uint32_t[s_Data.MaxIndices];

        s_Data.IndexBuffer.Create(s_Data.QuadIndexBufferBase, s_Data.MaxIndices);
        s_Data.IndexBuffer.Bind();
        s_Data.Shader.Bind();


        m_Layout.Push<float>(3);
        m_Layout.Push<float>(4);
        m_Layout.Push<float>(2);
        m_Layout.Push<float>(3);
        m_Layout.Push<float>(1);
        m_Layout.Push<float>(1);
        m_Layout.Push<float>(1);
        m_Layout.Push<int>(1);
        s_Data.VertexArray.Addbuffer(s_Data.VertexBuffer, m_Layout);

        s_Data.QuadVertexBufferBase = new Vertex[s_Data.MaxVertices];

        int32_t samplers[s_Data.MaxTextureSlots];

        for (uint32_t i = 0; i < s_Data.MaxTextureSlots; i++)
        {
            samplers[i] = i;
        }

        s_Data.Shader.SetUniformIntArray("u_Texture", samplers, s_Data.MaxTextureSlots);

        /*for (uint32_t i = 0; i < s_Data.MaxTextureSlots; i++)
        {
            s_Data.TextureSlots[i] = new Texture2D;
            s_Data.TextureSlots[i]->SetRendererID(0);
        }*/

        s_Data.VertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
        s_Data.VertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
        s_Data.VertexPositions[2] = { 0.5f,  0.5f, 0.0f, 1.0f };
        s_Data.VertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };

    }

    void Renderer::BeginScene(Camera& camera, glm::mat4& transform)
    {
        m_SceneData->ViewProjectionMatrix = camera.GetProjection() * glm::inverse(transform);

        s_Data.IndicesOffset = 0;
        s_Data.QuadIndexCount = 0;  

        s_Data.QuadIndicesPtr = s_Data.QuadIndices;
        s_Data.VertexBufferPtr = s_Data.VertexBufferBase;
    }

    void Renderer::BeginScene(EditorCamera& camera)
    {
        m_SceneData->ViewProjectionMatrix = camera.GetViewProjection();

        s_Data.VertexArray.Bind();
        s_Data.VertexBuffer.Bind();
        s_Data.IndexBuffer.Bind();
        s_Data.Shader.Bind();
        
        s_Data.CubeMap.Bind();
        s_Data.Shader.SetUniform1i("irradianceMap", s_Data.CubeMap.GetRendererID());

        s_Data.IndicesOffset = 0;
        s_Data.QuadIndexCount = 0;
        s_Data.DrawCallsCount = 0;
        

        s_Data.QuadIndices = s_Data.QuadIndexBufferBase;
        s_Data.QuadIndicesPtr = s_Data.QuadIndices;
        s_Data.VertexBufferBase = s_Data.QuadVertexBufferBase;
        s_Data.VertexBufferPtr = s_Data.VertexBufferBase;

    }

    void Renderer::BeginScene(OrthographicCamera& camera)
	{
		m_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
        
        s_Data.IndicesOffset = 0;
        s_Data.QuadIndexCount = 0;
        s_Data.DrawCallsCount = 0;

        s_Data.VertexBufferPtr = s_Data.VertexBufferBase;
	}

    void Renderer::BeginSkyBoxScene(EditorCamera& camera)
    {
        s_Data.SkyboxShader.Bind();
        s_Data.SkyboxVertexArray.Bind();
        s_Data.SkyboxIndexBuffer.Bind();
        s_Data.SkyboxVertexBuffer.Bind();
        s_Data.CubeMap.Bind();
        s_Data.SkyboxShader.SetUniformMat4f("projection", camera.GetProjection());
        glm::mat4 view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
        s_Data.SkyboxShader.SetUniformMat4f("view", view);
        s_Data.SkyboxShader.SetUniform1i("skybox", s_Data.CubeMap.GetRendererID());
    }

	void Renderer::EndScene()
	{
        s_Data.Shader.SetUniformMat4f("u_ViewProjection", m_SceneData->ViewProjectionMatrix);

        uint32_t dataSize = (uint8_t*)s_Data.VertexBufferPtr - (uint8_t*)s_Data.VertexBufferBase;
      
        s_Data.IndexBuffer.SetData((void*)s_Data.QuadIndices, s_Data.QuadIndexCount);
        s_Data.VertexBuffer.SetData((void*)s_Data.VertexBufferBase, dataSize);

        RenderCommand::DrawElements(GL_TRIANGLES, s_Data.QuadIndexCount, GL_UNSIGNED_INT, nullptr);
        
    }

    void Renderer::EndSceneCubeMap()
    {
        s_Data.SkyboxIndexBuffer.SetData((void*)s_Data.SkyBoxCubeMapVertexIndices, s_Data.SkyBoxCubeMapIndicesCount);
        s_Data.SkyboxVertexBuffer.SetData((void*)s_Data.SkyBoxCubeMapVertexPositions, sizeof(float) * s_Data.SkyBoxCubeMapVerticesCount);

        RenderCommand::SetDepthFunc(GL_LEQUAL);
        RenderCommand::DrawElements(GL_TRIANGLES, s_Data.SkyBoxCubeMapIndicesCount, GL_UNSIGNED_INT, nullptr);
        RenderCommand::SetDepthFunc(GL_LESS);

        s_Data.SkyboxShader.UnBind();
        s_Data.SkyboxVertexArray.UnBind();
        s_Data.SkyboxIndexBuffer.UnBind();
        s_Data.SkyboxVertexBuffer.UnBind();
    }

    void Renderer::StartBatch()
    {
        EndScene();
        ResetStats();

    }

    void Renderer::ResetStats()
    {
        s_Data.TextureIndex = 0;
        s_Data.QuadIndexCount = 0;
        s_Data.IndicesOffset = 0;

        s_Data.QuadIndices = s_Data.QuadIndexBufferBase;
        s_Data.QuadIndicesPtr = s_Data.QuadIndices;
        s_Data.VertexBufferBase = s_Data.QuadVertexBufferBase;
        s_Data.VertexBufferPtr = s_Data.VertexBufferBase;

        //s_Data.VertexBufferPtr = s_Data.VertexBufferBase;
        //s_Data.QuadIndicesPtr = s_Data.QuadIndices;

        s_Data.DrawCallsCount++;
    }

    void Renderer::ShutDown()
    {
        delete[] s_Data.VertexBufferBase;
    }

    void Renderer::SetUniform1i(const std::string& name, const int& value)
    {
        s_Data.Shader.SetUniform1i(name, value);
    }

    void Renderer::SetUniform1f(const std::string& name, const float& value)
    {
        s_Data.Shader.SetUniform1f(name, value);
    }

    void Renderer::SetUniform3f(const std::string& name, const glm::vec3& position)
    {
        s_Data.Shader.SetUniform3f(name, position.x, position.y, position.z);
    }

    void Renderer::SetUniform3fArray(const std::string& name, const glm::vec3* position, const int count)
    {
        s_Data.Shader.SetUniformVec3Array(name, position, count);
    }

    void Renderer::SetUniformMat4f(const std::string& name, const glm::mat4& transform)
    {
        s_Data.Shader.SetUniformMat4f(name, transform);
    }

    void Renderer::BindTextures()
    {
        for (uint32_t i = 0; i < s_Data.TextureIndex; i++)
        {
            if (s_Data.TextureSlots[i] != nullptr)
            {
                s_Data.TextureSlots[i]->Bind(i);
            }
        }
    }

    void Renderer::DrawQuad(const glm::mat4& transform, const glm::vec4& color, int entityID)
    {
        s_Data.Shader.SetUniformMat4f("u_MeshTransform", glm::mat4(1.0f));
        s_Data.Shader.SetUniform1i("u_IsTextured", 0);

        s_Data.VertexBufferPtr->Position = transform * s_Data.VertexPositions[0];
        s_Data.VertexBufferPtr->Color = { color.x, color.y, color.z, color.w };
        s_Data.VertexBufferPtr->TexCoord = { 0.0f, 0.0f };
        s_Data.VertexBufferPtr->Normal = { 0.0f, 0.0f, 1.0f };
        s_Data.VertexBufferPtr->IsFlatColored = 1.0f;
        s_Data.VertexBufferPtr->TexId = 32.0f;
        s_Data.VertexBufferPtr->TilingFactor = 1.0f;
        s_Data.VertexBufferPtr->entityID = entityID; 
        s_Data.VertexBufferPtr++;

        s_Data.VertexBufferPtr->Position = transform * s_Data.VertexPositions[1];
        s_Data.VertexBufferPtr->Color = { color.x, color.y, color.z, color.w };
        s_Data.VertexBufferPtr->TexCoord = { 1.0f, 0.0f };
        s_Data.VertexBufferPtr->Normal = { 0.0f, 0.0f, 1.0f };
        s_Data.VertexBufferPtr->IsFlatColored = 1.0f;
        s_Data.VertexBufferPtr->TexId = 32.0f;
        s_Data.VertexBufferPtr->TilingFactor = 1.0f;
        s_Data.VertexBufferPtr->entityID = entityID; 
        s_Data.VertexBufferPtr++;

        s_Data.VertexBufferPtr->Position = transform * s_Data.VertexPositions[2];
        s_Data.VertexBufferPtr->Color = { color.x, color.y, color.z, color.w };
        s_Data.VertexBufferPtr->TexCoord = { 1.0f, 1.0f };
        s_Data.VertexBufferPtr->Normal = { 0.0f, 0.0f, 1.0f };
        s_Data.VertexBufferPtr->IsFlatColored = 1.0f;
        s_Data.VertexBufferPtr->TexId = 32.0f;
        s_Data.VertexBufferPtr->TilingFactor = 1.0f;
        s_Data.VertexBufferPtr->entityID = entityID;
        s_Data.VertexBufferPtr++;

        s_Data.VertexBufferPtr->Position = transform * s_Data.VertexPositions[3];
        s_Data.VertexBufferPtr->Color = { color.x, color.y, color.z, color.w };
        s_Data.VertexBufferPtr->TexCoord = { 0.0f, 1.0f };
        s_Data.VertexBufferPtr->Normal = { 0.0f, 0.0f, 1.0f };
        s_Data.VertexBufferPtr->IsFlatColored = 1.0f;
        s_Data.VertexBufferPtr->TexId = 32.0f;
        s_Data.VertexBufferPtr->TilingFactor = 1.0f;
        s_Data.VertexBufferPtr->entityID = entityID;
        s_Data.VertexBufferPtr++;

        *s_Data.QuadIndicesPtr = s_Data.IndicesOffset + 0;
        s_Data.QuadIndicesPtr++;
        *s_Data.QuadIndicesPtr = s_Data.IndicesOffset + 1;
        s_Data.QuadIndicesPtr++;
        *s_Data.QuadIndicesPtr = s_Data.IndicesOffset + 2;
        s_Data.QuadIndicesPtr++;

        *s_Data.QuadIndicesPtr = s_Data.IndicesOffset + 3;
        s_Data.QuadIndicesPtr++;
        *s_Data.QuadIndicesPtr = s_Data.IndicesOffset + 2;
        s_Data.QuadIndicesPtr++;
        *s_Data.QuadIndicesPtr = s_Data.IndicesOffset + 0;
        s_Data.QuadIndicesPtr++;


        s_Data.IndicesOffset += 4;
        s_Data.QuadIndexCount += 6;
    }

    void Renderer::DrawQuad(const glm::mat4& transform, Texture2D* texture, float tilingFactor, float entityID)
    {
        float textureIndex = (float)s_Data.TextureIndex;

        s_Data.TextureSlots[s_Data.TextureIndex] = texture;
        s_Data.TextureIndex++;


        s_Data.Shader.SetUniformMat4f("u_MeshTransform", glm::mat4(1.0f));
        s_Data.Shader.SetUniform1i("u_IsTextured", 0);

        s_Data.VertexBufferPtr->Position = transform * s_Data.VertexPositions[0];
        s_Data.VertexBufferPtr->Color = { 1.0f, 1.0f, 1.0f, 1.0f };
        s_Data.VertexBufferPtr->TexCoord = { 0.0f, 0.0f };
        s_Data.VertexBufferPtr->Normal = { 0.0f, 0.0f, 1.0f };
        s_Data.VertexBufferPtr->IsFlatColored = 1.0f;
        s_Data.VertexBufferPtr->TexId = textureIndex;
        s_Data.VertexBufferPtr->TilingFactor = tilingFactor;
        s_Data.VertexBufferPtr->entityID = entityID;
        s_Data.VertexBufferPtr++;

        s_Data.VertexBufferPtr->Position = transform * s_Data.VertexPositions[1];
        s_Data.VertexBufferPtr->Color = { 1.0f, 1.0f, 1.0f, 1.0f };
        s_Data.VertexBufferPtr->TexCoord = { 1.0f, 0.0f };
        s_Data.VertexBufferPtr->Normal = { 0.0f, 0.0f, 1.0f };
        s_Data.VertexBufferPtr->IsFlatColored = 1.0f;
        s_Data.VertexBufferPtr->TexId = textureIndex;
        s_Data.VertexBufferPtr->TilingFactor = tilingFactor;
        s_Data.VertexBufferPtr->entityID = entityID;
        s_Data.VertexBufferPtr++;

        s_Data.VertexBufferPtr->Position = transform * s_Data.VertexPositions[2];
        s_Data.VertexBufferPtr->Color = { 1.0f, 1.0f, 1.0f, 1.0f };
        s_Data.VertexBufferPtr->TexCoord = { 1.0f, 1.0f };
        s_Data.VertexBufferPtr->Normal = { 0.0f, 0.0f, 1.0f };
        s_Data.VertexBufferPtr->IsFlatColored = 1.0f;
        s_Data.VertexBufferPtr->TexId = textureIndex;
        s_Data.VertexBufferPtr->TilingFactor = tilingFactor;
        s_Data.VertexBufferPtr->entityID = entityID;
        s_Data.VertexBufferPtr++;
      
        s_Data.VertexBufferPtr->Position = transform * s_Data.VertexPositions[3];
        s_Data.VertexBufferPtr->Color = { 1.0f, 1.0f, 1.0f, 1.0f };
        s_Data.VertexBufferPtr->TexCoord = { 0.0f, 1.0f };
        s_Data.VertexBufferPtr->Normal = { 0.0f, 0.0f, 1.0f };
        s_Data.VertexBufferPtr->IsFlatColored = 1.0f;
        s_Data.VertexBufferPtr->TexId = textureIndex;
        s_Data.VertexBufferPtr->TilingFactor = tilingFactor;
        s_Data.VertexBufferPtr->entityID = entityID;
        s_Data.VertexBufferPtr++;

        
        *s_Data.QuadIndicesPtr = s_Data.IndicesOffset + 0;
        s_Data.QuadIndicesPtr++;
        *s_Data.QuadIndicesPtr = s_Data.IndicesOffset + 1;
        s_Data.QuadIndicesPtr++;
        *s_Data.QuadIndicesPtr = s_Data.IndicesOffset + 2;
        s_Data.QuadIndicesPtr++;

        *s_Data.QuadIndicesPtr = s_Data.IndicesOffset + 3;
        s_Data.QuadIndicesPtr++;
        *s_Data.QuadIndicesPtr = s_Data.IndicesOffset + 2;
        s_Data.QuadIndicesPtr++;
        *s_Data.QuadIndicesPtr = s_Data.IndicesOffset + 0;
        s_Data.QuadIndicesPtr++;


        s_Data.IndicesOffset += 4;
        s_Data.QuadIndexCount += 6;
    }

    void Renderer::DrawQuad(const glm::vec3& position, const glm::vec2& size, const float rotation, const  glm::vec4& color, const int entityID, float tf)
    {
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(position.x, position.y, position.z)) *
            glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f }) *
            glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

        DrawQuad(transform, color, entityID);
    }

    void Renderer::DrawQuad(const glm::vec3& position, glm::vec2& size, Texture2D* texture, float tilingFactor, float entityID)
    {
        float textureIndex = (float)s_Data.TextureIndex;

        s_Data.TextureSlots[s_Data.TextureIndex] = texture;
        s_Data.TextureIndex++;

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
            glm::rotate(glm::mat4(1.0f), glm::radians(texture->GetRotation()), { 0.0f, 0.0f, 1.0f }) *
            glm::scale(glm::mat4(1.0f), { size, 1.0f });

        DrawQuad(transform, texture, tilingFactor, entityID);

    }

    void Renderer::DrawTriangle(const glm::vec3& position, const glm::vec2& size, const float rotation, const glm::vec4& color, float tf)
    {
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(position.x, position.y, position.z)) *
            glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f }) *
            glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

        s_Data.Shader.SetUniformMat4f("u_MeshTransform", transform);

        s_Data.VertexBufferPtr->Position = transform * s_Data.VertexPositions[0];
        s_Data.VertexBufferPtr->Color = { color.r, color.g, color.b, color.t };
        s_Data.VertexBufferPtr->TexCoord = { 0.0f, 0.0f };
        s_Data.VertexBufferPtr->TexId = 32.0f;
        s_Data.VertexBufferPtr->TilingFactor = 1.0f;
        s_Data.VertexBufferPtr++;

        s_Data.VertexBufferPtr->Position = transform * s_Data.VertexPositions[1];
        s_Data.VertexBufferPtr->Color = { color.r, color.g, color.b, color.t };
        s_Data.VertexBufferPtr->TexCoord = { 1.0f, 0.0f };
        s_Data.VertexBufferPtr->TexId = 32.0f;
        s_Data.VertexBufferPtr->TilingFactor = 1.0f;
        s_Data.VertexBufferPtr++;

        s_Data.VertexBufferPtr->Position = transform * s_Data.VertexPositions[2];
        s_Data.VertexBufferPtr->Color = { color.r, color.g, color.b, color.t };
        s_Data.VertexBufferPtr->TexCoord = { 1.0f, 1.0f };
        s_Data.VertexBufferPtr->TexId = 32.0f;
        s_Data.VertexBufferPtr->TilingFactor = 1.0f;
        s_Data.VertexBufferPtr++;


        s_Data.QuadIndicesPtr[s_Data.QuadIndexCount + 0] = s_Data.IndicesOffset + 0;
        s_Data.QuadIndicesPtr[s_Data.QuadIndexCount + 1] = s_Data.IndicesOffset + 1;
        s_Data.QuadIndicesPtr[s_Data.QuadIndexCount + 2] = s_Data.IndicesOffset + 2;

        s_Data.IndicesOffset += 3;
        s_Data.QuadIndexCount += 3;
    }

    void Renderer::DrawMesh(const glm::mat4& transform, Mesh& mesh, int entityID)
    {

        s_Data.Shader.SetUniformMat4f("u_MeshTransform", transform);
        s_Data.Shader.SetUniform1i("u_EntityID", entityID);
        s_Data.Shader.SetUniform1i("u_IsTextured", 0);
        s_Data.Shader.SetUniform3f("u_Albedo", mesh.m_Material.Albedo.x, mesh.m_Material.Albedo.y, mesh.m_Material.Albedo.z);
        s_Data.Shader.SetUniform1f("u_Metallness", mesh.m_Material.Metallic);
        s_Data.Shader.SetUniform1f("u_Roughness", mesh.m_Material.Roughness);
        s_Data.Shader.SetUniform1f("u_AO", mesh.m_Material.AO);

        s_Data.QuadIndicesPtr = mesh.IndicesPtr;
        s_Data.QuadIndexCount = mesh.QuadIndexCount;
        s_Data.VertexBufferBase = mesh.VerticesBase;
        s_Data.VertexBufferPtr = mesh.VerticesPtr;
        s_Data.QuadIndices = mesh.IndicesBase;
        s_Data.IndicesOffset = mesh.QuadVerticesCount;
        
        StartBatch();
        // TODO::Reconsider which method is better to Render;
        
        //for (auto& index : mesh.GetIndices())
        //{
        //    s_Data.QuadIndicesPtr[s_Data.QuadIndexCount] = s_Data.IndicesOffset + index;
        //    s_Data.QuadIndexCount += 1;
        //}
        //for (auto& vertex : mesh.GetVertices())
        //{
        //    s_Data.VertexBufferPtr->Position = vertex.Position;
        //    s_Data.VertexBufferPtr->Color = vertex.Color;
        //    s_Data.VertexBufferPtr->TexCoord = vertex.TexCoord;
        //    s_Data.VertexBufferPtr->Normal = vertex.Normal;
        //    s_Data.VertexBufferPtr->Albedo = vertex.Albedo; //mesh.m_Material.Albedo;
        //    s_Data.VertexBufferPtr->Metallic = vertex.Metallic; //mesh.m_Material.Metallic;
        //    s_Data.VertexBufferPtr->Roughness = vertex.Roughness;// mesh.m_Material.Roughness;
        //    s_Data.VertexBufferPtr->AO = vertex.AO;// mesh.m_Material.AO;
        //    s_Data.VertexBufferPtr->TexId = 32.0f;
        //    s_Data.VertexBufferPtr->TilingFactor = 1.0f;
        //    s_Data.VertexBufferPtr->entityID = entityID;
        //    s_Data.VertexBufferPtr->Matrix = transform;
        //    s_Data.VertexBufferPtr++;
        //    s_Data.IndicesOffset++;
        //}
    }

    void Renderer::DrawMesh(const glm::mat4& transform, Mesh& mesh, std::vector<Texture2D>& textures, int entityID)
    {
        s_Data.Shader.SetUniformMat4f("u_MeshTransform", transform);
        s_Data.Shader.SetUniform1i("u_EntityID", entityID);
        s_Data.Shader.SetUniform1i("u_IsTextured", 1);
      

        int samplers[5];

        for (uint32_t i = 0; i < 5; i++)
        {
            samplers[i] = s_Data.TextureIndex + i;
            textures[i].Bind(i + s_Data.TextureIndex);
        }

        s_Data.Shader.SetUniformIntArray("u_TexturedMap", samplers, 5);

        s_Data.QuadIndicesPtr = mesh.IndicesPtr;
        s_Data.QuadIndexCount = mesh.QuadIndexCount;
        s_Data.VertexBufferBase = mesh.VerticesBase;
        s_Data.VertexBufferPtr = mesh.VerticesPtr;
        s_Data.QuadIndices = mesh.IndicesBase;
        s_Data.IndicesOffset += mesh.QuadVerticesCount;

        StartBatch();

        for (uint32_t i = 0; i < 5; i++)
        {
            textures[i].UnBind();
        }
    }

    void Renderer::DrawMesh(const glm::mat4& transform, Mesh& mesh, Material& material, int entityID)
    {
        StartBatch();

        s_Data.Shader.SetUniformMat4f("MeshTransform", transform);
        s_Data.Shader.SetUniform1i("EntityID", entityID);


        s_Data.QuadIndicesPtr = mesh.IndicesPtr;
        s_Data.QuadIndexCount = mesh.QuadIndexCount;
        s_Data.VertexBufferBase = mesh.VerticesBase;
        s_Data.VertexBufferPtr = mesh.VerticesPtr;
        s_Data.QuadIndices = mesh.IndicesBase;
    }

    void Renderer::DrawLight(const glm::mat4& transform, const Light& light, Texture2D* texture, int entityID)
    {
        s_Data.Shader.SetUniformMat4f("u_MeshTransform", glm::mat4(1.0f));
        s_Data.TextureSlots[25] = texture;
        s_Data.TextureSlots[25]->Bind(25);

        s_Data.VertexBufferPtr->Position = transform * s_Data.VertexPositions[0];
        s_Data.VertexBufferPtr->Color = { 1.0f, 1.0f, 1.0f, 1.0f };
        s_Data.VertexBufferPtr->TexCoord = { 0.0f, 0.0f };
        s_Data.VertexBufferPtr->Normal = { 0.0f, 0.0f, 1.0f };
        s_Data.VertexBufferPtr->IsFlatColored = 1.0f;
        s_Data.VertexBufferPtr->TexId = 25.0f;
        s_Data.VertexBufferPtr->TilingFactor = 1.0f;
        s_Data.VertexBufferPtr->entityID = entityID;
        s_Data.VertexBufferPtr++;

        s_Data.VertexBufferPtr->Position = transform * s_Data.VertexPositions[1];
        s_Data.VertexBufferPtr->Color = { 1.0f, 1.0f, 1.0f, 1.0f };
        s_Data.VertexBufferPtr->TexCoord = { 1.0f, 0.0f };
        s_Data.VertexBufferPtr->Normal = { 0.0f, 0.0f, 1.0f };
        s_Data.VertexBufferPtr->IsFlatColored = 1.0f;
        s_Data.VertexBufferPtr->TexId = 25.0f;
        s_Data.VertexBufferPtr->TilingFactor = 1.0f;
        s_Data.VertexBufferPtr->entityID = entityID;
        s_Data.VertexBufferPtr++;

        s_Data.VertexBufferPtr->Position = transform * s_Data.VertexPositions[2];
        s_Data.VertexBufferPtr->Color = { 1.0f, 1.0f, 1.0f, 1.0f };
        s_Data.VertexBufferPtr->TexCoord = { 1.0f, 1.0f };
        s_Data.VertexBufferPtr->Normal = { 0.0f, 0.0f, 1.0f };
        s_Data.VertexBufferPtr->IsFlatColored = 1.0f;
        s_Data.VertexBufferPtr->TexId = 25.0f;
        s_Data.VertexBufferPtr->TilingFactor = 1.0f;
        s_Data.VertexBufferPtr->entityID = entityID;
        s_Data.VertexBufferPtr++;

        s_Data.VertexBufferPtr->Position = transform * s_Data.VertexPositions[3];
        s_Data.VertexBufferPtr->Color = { 1.0f, 1.0f, 1.0f, 1.0f };
        s_Data.VertexBufferPtr->TexCoord = { 0.0f, 1.0f };
        s_Data.VertexBufferPtr->Normal = { 0.0f, 0.0f, 1.0f };
        s_Data.VertexBufferPtr->IsFlatColored = 1.0f;
        s_Data.VertexBufferPtr->TexId = 25.0f;
        s_Data.VertexBufferPtr->TilingFactor = 1.0f;
        s_Data.VertexBufferPtr->entityID = entityID;
        s_Data.VertexBufferPtr++;


        *s_Data.QuadIndicesPtr = s_Data.IndicesOffset + 0;
        s_Data.QuadIndicesPtr++;
        *s_Data.QuadIndicesPtr = s_Data.IndicesOffset + 1;
        s_Data.QuadIndicesPtr++;
        *s_Data.QuadIndicesPtr = s_Data.IndicesOffset + 2;
        s_Data.QuadIndicesPtr++;

        *s_Data.QuadIndicesPtr = s_Data.IndicesOffset + 3;
        s_Data.QuadIndicesPtr++;
        *s_Data.QuadIndicesPtr = s_Data.IndicesOffset + 2;
        s_Data.QuadIndicesPtr++;
        *s_Data.QuadIndicesPtr = s_Data.IndicesOffset + 0;
        s_Data.QuadIndicesPtr++;


        s_Data.IndicesOffset += 4;
        s_Data.QuadIndexCount += 6;
    }

    void Renderer::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
    {
        glViewport(x, y, width, height);
    }
   
}
