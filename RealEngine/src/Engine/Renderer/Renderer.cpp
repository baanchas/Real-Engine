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
            "assets/skyboxes/hilly terrain/right.png",
            "assets/skyboxes/hilly terrain/left.png",
            "assets/skyboxes/hilly terrain/top.png",
            "assets/skyboxes/hilly terrain/bottom.png",
            "assets/skyboxes/hilly terrain/front.png",
            "assets/skyboxes/hilly terrain/back.png",
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


        s_Data.QuadIndexBufferBase = new uint32_t[s_Data.MaxIndices];

        s_Data.IndexBuffer.Create(s_Data.QuadIndexBufferBase, s_Data.MaxIndices);
        s_Data.IndexBuffer.Bind();
        s_Data.Shader.Bind();


        VertexBufferLayout m_Layout;
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

        UploadUniformIntArray("u_Texture", samplers, s_Data.MaxTextureSlots);

        s_Data.VertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
        s_Data.VertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
        s_Data.VertexPositions[2] = { 0.5f,  0.5f, 0.0f, 1.0f };
        s_Data.VertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };

    }

    void Renderer::BeginScene(Camera& camera, glm::mat4& transform)
    {
        m_SceneData->ViewProjectionMatrix = camera.GetProjection() * glm::inverse(transform);

        BindScene(s_Data.Shader, s_Data.VertexArray, s_Data.VertexBuffer, s_Data.IndexBuffer);

        ResetMainSceneBuffers();
    }

    void Renderer::BeginScene(EditorCamera& camera)
    {
        m_SceneData->ViewProjectionMatrix = camera.GetViewProjection();

        BindScene(s_Data.Shader, s_Data.VertexArray, s_Data.VertexBuffer, s_Data.IndexBuffer);

        ResetMainSceneBuffers();
    }

    void Renderer::BeginScene(OrthographicCamera& camera)
	{
		m_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
        
        BindScene(s_Data.Shader, s_Data.VertexArray, s_Data.VertexBuffer, s_Data.IndexBuffer);

        ResetMainSceneBuffers();
	}

    void Renderer::BeginSkyBoxScene(EditorCamera& camera)
    {
        BindScene(s_Data.SkyboxShader, s_Data.SkyboxVertexArray, s_Data.SkyboxVertexBuffer, s_Data.SkyboxIndexBuffer);

        s_Data.CubeMap.Bind();

        s_Data.SkyboxShader.UploadUniformMat4Float("u_Projection", camera.GetProjection());
        glm::mat4 view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
        s_Data.SkyboxShader.UploadUniformMat4Float("u_View", view);
        s_Data.SkyboxShader.UploadUniformInt("u_Skybox", s_Data.CubeMap.GetRendererID());
    }

    void Renderer::BeginSkyBoxScene(Camera& camera, glm::mat4& transform)
    {
        BindScene(s_Data.SkyboxShader, s_Data.SkyboxVertexArray, s_Data.SkyboxVertexBuffer, s_Data.SkyboxIndexBuffer);

        s_Data.CubeMap.Bind();

        s_Data.SkyboxShader.UploadUniformMat4Float("u_Projection", camera.GetProjection());
        glm::mat4 view = glm::mat4(glm::mat3(transform));
        s_Data.SkyboxShader.UploadUniformMat4Float("u_View", view);
        s_Data.SkyboxShader.UploadUniformInt("u_Skybox", s_Data.CubeMap.GetRendererID());
    }

	void Renderer::EndScene()
	{
        UploadUniformMat4Float("u_ViewProjection", m_SceneData->ViewProjectionMatrix);

        uint32_t dataSize = (uint8_t*)s_Data.VertexBufferPtr - (uint8_t*)s_Data.VertexBufferBase;
      
        s_Data.IndexBuffer.SetData((void*)s_Data.QuadIndices, s_Data.QuadIndexCount);
        s_Data.VertexBuffer.SetData((void*)s_Data.VertexBufferBase, dataSize);

        RenderCommand::DrawTriangles(s_Data.QuadIndexCount, GL_UNSIGNED_INT, nullptr);
        
    }

    void Renderer::EndSkyBoxScene()
    {
        s_Data.SkyboxIndexBuffer.SetData((void*)s_Data.SkyBoxCubeMapVertexIndices, s_Data.SkyBoxCubeMapIndicesCount);
        s_Data.SkyboxVertexBuffer.SetData((void*)s_Data.SkyBoxCubeMapVertexPositions, sizeof(float) * s_Data.SkyBoxCubeMapVerticesCount);

        RenderCommand::SetDepthFunc(GL_LEQUAL);
        RenderCommand::DrawTriangles(s_Data.SkyBoxCubeMapIndicesCount, GL_UNSIGNED_INT, nullptr);
        RenderCommand::SetDepthFunc(GL_LESS);

        UnbindScene(s_Data.SkyboxShader, s_Data.SkyboxVertexArray, s_Data.SkyboxVertexBuffer, s_Data.SkyboxIndexBuffer);
    }

    void Renderer::BindScene(Shader& shader, VertexArray& va, VertexBuffer& vb, IndexBuffer& ib)
    {
        shader.Bind();
        va.Bind();
        vb.Bind();
        ib.Bind();
    }

    void Renderer::UnbindScene(Shader& shader, VertexArray& va, VertexBuffer& vb, IndexBuffer& ib)
    {
        shader.UnBind();
        va.UnBind();
        vb.UnBind();
        ib.UnBind();
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

    void Renderer::StartBatch()
    {
        EndScene();
        ResetMainSceneBuffers();
    }

    void Renderer::ResetMainSceneBuffers()
    {
        s_Data.TextureIndex = 0;
        s_Data.QuadIndexCount = 0;
        s_Data.IndicesOffset = 0;

        s_Data.QuadIndices = s_Data.QuadIndexBufferBase;
        s_Data.QuadIndicesPtr = s_Data.QuadIndices;
        s_Data.VertexBufferBase = s_Data.QuadVertexBufferBase;
        s_Data.VertexBufferPtr = s_Data.VertexBufferBase;
    }

    void Renderer::ShutDown()
    {
        delete[] s_Data.VertexBufferBase;
        delete[] s_Data.QuadIndexBufferBase;
    }

    void Renderer::UploadUniformInt(const std::string& name, const int& value)
    {
        s_Data.Shader.UploadUniformInt(name, value);
    }

    void Renderer::UploadUniformFloat(const std::string& name, const float& value)
    {
        s_Data.Shader.UploadUniformFloat(name, value);
    }

    void Renderer::UploadUniformVec3Float(const std::string& name, const glm::vec3& position)
    {
        s_Data.Shader.UploadUniformVec3Float(name, position.x, position.y, position.z);
    }

    void Renderer::UploadUniformIntArray(const std::string& name, int* value, const int count)
    {
        s_Data.Shader.UploadUniformIntArray(name, value, count);
    }

    void Renderer::UploadUniformFloatArray(const std::string& name, const float* value, const int count)
    {
        s_Data.Shader.UploadUniformFloatArray(name, value, count);
    }

    void Renderer::UploadUniformVec3Array(const std::string& name, const glm::vec3* position, const int count)
    {
       s_Data.Shader.UploadUniformVec3Array(name, position, count);
    }

    void Renderer::UploadUniformMat4Float(const std::string& name, const glm::mat4& transform)
    {
        s_Data.Shader.UploadUniformMat4Float(name, transform);
    }

    void Renderer::DrawTriangle(const glm::vec3& position, const glm::vec2& size, const float rotation, const glm::vec4& color, float tf)
    {
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(position.x, position.y, position.z)) *
            glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f }) *
            glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

        UploadUniformMat4Float("u_MeshTransform", transform);

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

    void Renderer::DrawQuad(const glm::mat4& transform, const glm::vec4& color, int entityID)
    {
        UploadUniformMat4Float("u_MeshTransform", glm::mat4(1.0f));

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


        UploadUniformMat4Float("u_MeshTransform", glm::mat4(1.0f));

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

    void Renderer::DrawMesh(const glm::mat4& transform, Mesh& mesh, int entityID)
    {
        UploadUniformMat4Float("u_MeshTransform", transform);
        UploadUniformInt("u_EntityID", entityID);
        UploadUniformInt("u_IsTextured", 0);
        UploadUniformVec3Float("u_Albedo", { mesh.m_Material.Albedo.x, mesh.m_Material.Albedo.y, mesh.m_Material.Albedo.z });
        UploadUniformFloat("u_Metallness", mesh.m_Material.Metallic);
        UploadUniformFloat("u_Roughness", mesh.m_Material.Roughness);
        UploadUniformFloat("u_AO", mesh.m_Material.AO);

        s_Data.QuadIndicesPtr = mesh.IndicesPtr;
        s_Data.QuadIndexCount = mesh.QuadIndexCount;
        s_Data.VertexBufferBase = mesh.VerticesBase;
        s_Data.VertexBufferPtr = mesh.VerticesPtr;
        s_Data.QuadIndices = mesh.IndicesBase;
        s_Data.IndicesOffset = mesh.QuadVerticesCount;
        
        StartBatch();
    }

    void Renderer::DrawMesh(const glm::mat4& transform, Mesh& mesh, std::vector<std::shared_ptr<Texture2D>>& textures, bool isTexturedMaterial[5], int entityID)
    {
        UploadUniformMat4Float("u_MeshTransform", transform);
        UploadUniformInt("u_EntityID", entityID);
        UploadUniformVec3Float("u_Albedo", { mesh.m_Material.Albedo.x, mesh.m_Material.Albedo.y, mesh.m_Material.Albedo.z });
        UploadUniformFloat("u_Metallness", mesh.m_Material.Metallic);
        UploadUniformFloat("u_Roughness", mesh.m_Material.Roughness);
        UploadUniformFloat("u_AO", mesh.m_Material.AO);


        int samplers[5];

        for (uint32_t i = 0; i < 5; i++)
        {
            samplers[i] = s_Data.TextureIndex + i;
            if (textures[i] != nullptr)
                textures[i]->Bind(i + s_Data.TextureIndex);
        }
        
        int isTexturedMaterialUniform[5];
        for (uint32_t i = 0; i < 5; i++)
        {
            isTexturedMaterialUniform[i] = isTexturedMaterial[i];
        }

        UploadUniformIntArray("u_TexturedMap", samplers, 5);
        UploadUniformIntArray("u_isTexturedMaterial", isTexturedMaterialUniform, 5);

        s_Data.QuadIndicesPtr = mesh.IndicesPtr;
        s_Data.QuadIndexCount = mesh.QuadIndexCount;
        s_Data.VertexBufferBase = mesh.VerticesBase;
        s_Data.VertexBufferPtr = mesh.VerticesPtr;
        s_Data.QuadIndices = mesh.IndicesBase;
        s_Data.IndicesOffset += mesh.QuadVerticesCount;

        StartBatch();

        for (uint32_t i = 0; i < 5; i++)
        {
            if (textures[i] != nullptr)
                textures[i]->UnBind();
        }
    }

    void Renderer::DrawLight(const glm::mat4& transform, Texture2D* texture, int entityID)
    {
        UploadUniformMat4Float("u_MeshTransform", glm::mat4(1.0f));
        s_Data.TextureSlots[25] = texture;
        s_Data.TextureSlots[25]->Bind(25);

       // glm::mat4 t = transform * glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), { 0.0f, 0.0f, 1.0f });

        s_Data.VertexBufferPtr->Position = transform * (s_Data.VertexPositions[0] * 2.0f);
        s_Data.VertexBufferPtr->Color = { 1.0f, 1.0f, 1.0f, 1.0f };
        s_Data.VertexBufferPtr->TexCoord = { 0.0f, 0.0f };
        s_Data.VertexBufferPtr->Normal = { 0.0f, 0.0f, 1.0f };
        s_Data.VertexBufferPtr->IsFlatColored = 1.0f;
        s_Data.VertexBufferPtr->TexId = 25.0f;
        s_Data.VertexBufferPtr->TilingFactor = 1.0f;
        s_Data.VertexBufferPtr->entityID = entityID;
        s_Data.VertexBufferPtr++;

        s_Data.VertexBufferPtr->Position = transform * (s_Data.VertexPositions[1] * 2.0f);
        s_Data.VertexBufferPtr->Color = { 1.0f, 1.0f, 1.0f, 1.0f };
        s_Data.VertexBufferPtr->TexCoord = { 1.0f, 0.0f };
        s_Data.VertexBufferPtr->Normal = { 0.0f, 0.0f, 1.0f };
        s_Data.VertexBufferPtr->IsFlatColored = 1.0f;
        s_Data.VertexBufferPtr->TexId = 25.0f;
        s_Data.VertexBufferPtr->TilingFactor = 1.0f;
        s_Data.VertexBufferPtr->entityID = entityID;
        s_Data.VertexBufferPtr++;

        s_Data.VertexBufferPtr->Position = transform * (s_Data.VertexPositions[2] * 2.0f);
        s_Data.VertexBufferPtr->Color = { 1.0f, 1.0f, 1.0f, 1.0f };
        s_Data.VertexBufferPtr->TexCoord = { 1.0f, 1.0f };
        s_Data.VertexBufferPtr->Normal = { 0.0f, 0.0f, 1.0f };
        s_Data.VertexBufferPtr->IsFlatColored = 1.0f;
        s_Data.VertexBufferPtr->TexId = 25.0f;
        s_Data.VertexBufferPtr->TilingFactor = 1.0f;
        s_Data.VertexBufferPtr->entityID = entityID;
        s_Data.VertexBufferPtr++;

        s_Data.VertexBufferPtr->Position = transform * (s_Data.VertexPositions[3] * 2.0f);
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
