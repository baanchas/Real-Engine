#include "repch.h"
#include "Renderer.h"

namespace RealEngine {

	Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData;

    static RendererData s_Data;

    void Renderer::BeginScene(Camera& camera, glm::mat4& transform)
    {
        m_SceneData->ViewProjectionMatrix = camera.GetProjection() * glm::inverse(transform);

        s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;
    }

    void Renderer::BeginScene(OrthographicCamera& camera)
	{
		m_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();

        s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;
	}

	void Renderer::EndScene()
	{
    
        uint32_t dataSize = (uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase;

        for (uint32_t i = 0; i < s_Data.TextureIndex; i++)
        {
            if (s_Data.TextureSlots[i] != nullptr)
                s_Data.TextureSlots[i]->Bind(i);
        }

        s_Data.VertexBuffer.SetData(s_Data.QuadVertexBufferBase, dataSize);
        
        DrawIndexed();
	}

    void Renderer::ShutDown()
    {
        delete[] s_Data.QuadVertexBufferBase;
    }

    void Renderer::Init()
    {
        
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        glEnable(GL_DEPTH_TEST);

        std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec4 a_Position;
			layout(location = 1) in vec4 a_Color;
			layout(location = 2) in vec2 a_TexCoord;
			layout(location = 3) in float a_TexID;
            layout(location = 4) in float a_TilingFactor;

			uniform mat4 u_ViewProjection;
            
            out vec4 v_Color;
            out vec2 v_TexCoord;
            out float v_TexID;
            out float v_TilingFactor;

			void main()
			{
                v_Color = a_Color;
                v_TexCoord = a_TexCoord;
                v_TexID = a_TexID;
                v_TilingFactor = a_TilingFactor;
				gl_Position = u_ViewProjection * a_Position;

            }
		)";

        std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;
            
            in vec4 v_Color;
            in vec2 v_TexCoord;
            in float v_TexID;
            in float v_TilingFactor;          

            uniform sampler2D u_Texture[32];

			void main()
			{
              	vec4 texColor = v_Color;
	            switch(int(v_TexID))
	            {
		            //case  0: texColor *= texture( u_Texture[0], v_TexCoord * v_TilingFactor); break;
		            case  1: texColor *= texture( u_Texture[1], v_TexCoord * v_TilingFactor); break;
		            case  2: texColor *= texture( u_Texture[2], v_TexCoord * v_TilingFactor); break;
		            case  3: texColor *= texture( u_Texture[3], v_TexCoord * v_TilingFactor); break;
		            case  4: texColor *= texture( u_Texture[4], v_TexCoord * v_TilingFactor); break;
		            case  5: texColor *= texture( u_Texture[5], v_TexCoord * v_TilingFactor); break;
		            case  6: texColor *= texture( u_Texture[6], v_TexCoord * v_TilingFactor); break;
		            case  7: texColor *= texture( u_Texture[7], v_TexCoord * v_TilingFactor); break;
		            case  8: texColor *= texture( u_Texture[8], v_TexCoord * v_TilingFactor); break;
		            case  9: texColor *= texture( u_Texture[9], v_TexCoord * v_TilingFactor); break;
		            case 10: texColor *= texture(u_Texture[10], v_TexCoord * v_TilingFactor); break;
		            case 11: texColor *= texture(u_Texture[11], v_TexCoord * v_TilingFactor); break;
		            case 12: texColor *= texture(u_Texture[12], v_TexCoord * v_TilingFactor); break;
		            case 13: texColor *= texture(u_Texture[13], v_TexCoord * v_TilingFactor); break;
		            case 14: texColor *= texture(u_Texture[14], v_TexCoord * v_TilingFactor); break;
		            case 15: texColor *= texture(u_Texture[15], v_TexCoord * v_TilingFactor); break;
		            case 16: texColor *= texture(u_Texture[16], v_TexCoord * v_TilingFactor); break;
		            case 17: texColor *= texture(u_Texture[17], v_TexCoord * v_TilingFactor); break;
		            case 18: texColor *= texture(u_Texture[18], v_TexCoord * v_TilingFactor); break;
		            case 19: texColor *= texture(u_Texture[19], v_TexCoord * v_TilingFactor); break;
		            case 20: texColor *= texture(u_Texture[20], v_TexCoord * v_TilingFactor); break;
		            case 21: texColor *= texture(u_Texture[21], v_TexCoord * v_TilingFactor); break;
		            case 22: texColor *= texture(u_Texture[22], v_TexCoord * v_TilingFactor); break;
		            case 23: texColor *= texture(u_Texture[23], v_TexCoord * v_TilingFactor); break;
		            case 24: texColor *= texture(u_Texture[24], v_TexCoord * v_TilingFactor); break;
		            case 25: texColor *= texture(u_Texture[25], v_TexCoord * v_TilingFactor); break;
		            case 26: texColor *= texture(u_Texture[26], v_TexCoord * v_TilingFactor); break;
		            case 27: texColor *= texture(u_Texture[27], v_TexCoord * v_TilingFactor); break;
		            case 28: texColor *= texture(u_Texture[28], v_TexCoord * v_TilingFactor); break;
		            case 29: texColor *= texture(u_Texture[29], v_TexCoord * v_TilingFactor); break;
		            case 30: texColor *= texture(u_Texture[30], v_TexCoord * v_TilingFactor); break;
		            case 31: texColor *= texture(u_Texture[31], v_TexCoord * v_TilingFactor); break;
	            }
	            color = texColor;
			}
		)";

        s_Data.VertexArray.Create();
        s_Data.Shader.Create(vertexSrc, fragmentSrc);
        s_Data.VertexBuffer.Create(sizeof(Vertex) * s_Data.MaxVertices);

        VertexBufferLayout m_Layout;

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
        delete[] quadIndices;


        s_Data.VertexBuffer.Bind();
        s_Data.Shader.Bind();
        s_Data.IndexBuffer.Bind();

        m_Layout.Push<float>(3);
        m_Layout.Push<float>(4);
        m_Layout.Push<float>(2);
        m_Layout.Push<float>(1);
        m_Layout.Push<float>(1);
        s_Data.VertexArray.Addbuffer(s_Data.VertexBuffer, m_Layout);

        s_Data.QuadVertexBufferBase = new Vertex[s_Data.MaxVertices];

        std::cout << sizeof(Vertex) << std::endl;

        int32_t samplers[s_Data.MaxTextureSlots];
        for (uint32_t i = 0; i < s_Data.MaxTextureSlots; i++)
        {
            samplers[i] = i;
        }
        s_Data.Shader.SetUniformIntArray("u_Texture", samplers, s_Data.MaxTextureSlots);

        for (uint32_t i = 0; i < s_Data.TextureIndex; i++)
        {
            if (s_Data.TextureSlots[i] != nullptr)
                s_Data.TextureSlots[i]->SetRendererID(0);
        }

        s_Data.QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f};
        s_Data.QuadVertexPositions[1] = {  0.5f, -0.5f, 0.0f, 1.0f};
        s_Data.QuadVertexPositions[2] = {  0.5f,  0.5f, 0.0f, 1.0f};
        s_Data.QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f};
    }

    void Renderer::DrawQuad(const glm::mat4& transform, const glm::vec4& color)
    {
       
        s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[0];
        s_Data.QuadVertexBufferPtr->Color = { color.x, color.y, color.z, color.w };
        s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f, 0.0f };
        s_Data.QuadVertexBufferPtr->TexId = 32.0f;
        s_Data.QuadVertexBufferPtr->TilingFactor = 1.0f;
        s_Data.QuadVertexBufferPtr++;

        s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[1];
        s_Data.QuadVertexBufferPtr->Color = { color.x, color.y, color.z, color.w };
        s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f, 0.0f };
        s_Data.QuadVertexBufferPtr->TexId = 32.0f;
        s_Data.QuadVertexBufferPtr->TilingFactor = 1.0f;
        s_Data.QuadVertexBufferPtr++;

        s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[2];
        s_Data.QuadVertexBufferPtr->Color = { color.x, color.y, color.z, color.w };
        s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f, 1.0f };
        s_Data.QuadVertexBufferPtr->TexId = 32.0f;
        s_Data.QuadVertexBufferPtr->TilingFactor = 1.0f;
        s_Data.QuadVertexBufferPtr++;

        s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[3];
        s_Data.QuadVertexBufferPtr->Color = { color.x, color.y, color.z, color.w };
        s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f, 1.0f };
        s_Data.QuadVertexBufferPtr->TexId = 32.0f;
        s_Data.QuadVertexBufferPtr->TilingFactor = 1.0f;
        s_Data.QuadVertexBufferPtr++;

    }

    void Renderer::DrawQuad(glm::mat4& transform, Texture2D* texture, float tilingFactor)
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

        s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[0];
        s_Data.QuadVertexBufferPtr->Color = { 1.0f, 1.0f, 1.0f, 1.0f };
        s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f, 0.0f };
        s_Data.QuadVertexBufferPtr->TexId = textureIndex;
        s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
        s_Data.QuadVertexBufferPtr++;

        s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[1];
        s_Data.QuadVertexBufferPtr->Color = { 1.0f, 1.0f, 1.0f, 1.0f };
        s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f, 0.0f };
        s_Data.QuadVertexBufferPtr->TexId = textureIndex;
        s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
        s_Data.QuadVertexBufferPtr++;

        s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[2];
        s_Data.QuadVertexBufferPtr->Color = { 1.0f, 1.0f, 1.0f, 1.0f };
        s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f, 1.0f };
        s_Data.QuadVertexBufferPtr->TexId = textureIndex;
        s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
        s_Data.QuadVertexBufferPtr++;

        s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[3];
        s_Data.QuadVertexBufferPtr->Color = { 1.0f, 1.0f, 1.0f, 1.0f };
        s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f, 1.0f };
        s_Data.QuadVertexBufferPtr->TexId = textureIndex;
        s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
        s_Data.QuadVertexBufferPtr++;
      
    }

    void Renderer::DrawQuad(glm::vec3& position, glm::vec2& size, float rotation, glm::vec4& color, float tf)
    {
        DrawQuad(position.x, position.y, position.z, size.x, size.y, rotation, color.x, color.y, color.z, color.w);
    }

    void Renderer::DrawQuad(float posX, float posY, float posZ, float sizeX, float sizeY, float rotation, float r, float g, float b, float t, float tf)
    {
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(posX, posY, posZ)) *
            glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f }) *
            glm::scale(glm::mat4(1.0f), { sizeX, sizeY, 1.0f });


        s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[0];
        s_Data.QuadVertexBufferPtr->Color = { r, g, b, t };
        s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f, 0.0f };
        s_Data.QuadVertexBufferPtr->TexId = 32.0f;
        s_Data.QuadVertexBufferPtr->TilingFactor = 1.0f;
        s_Data.QuadVertexBufferPtr++;

        s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[1];
        s_Data.QuadVertexBufferPtr->Color = { r, g, b, t };
        s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f, 0.0f };
        s_Data.QuadVertexBufferPtr->TexId = 32.0f;
        s_Data.QuadVertexBufferPtr->TilingFactor = 1.0f;
        s_Data.QuadVertexBufferPtr++;

        s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[2];
        s_Data.QuadVertexBufferPtr->Color = { r, g, b, t };
        s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f, 1.0f };
        s_Data.QuadVertexBufferPtr->TexId = 32.0f;
        s_Data.QuadVertexBufferPtr->TilingFactor = 1.0f;
        s_Data.QuadVertexBufferPtr++;

        s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[3];
        s_Data.QuadVertexBufferPtr->Color = { r, g, b, t };
        s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f, 1.0f };
        s_Data.QuadVertexBufferPtr->TexId = 32.0f;
        s_Data.QuadVertexBufferPtr->TilingFactor = 1.0f;

        s_Data.QuadVertexBufferPtr++;
    }

    void Renderer::DrawQuad(Quad& quad)
    {
        
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(quad.Vertex.Position.x, quad.Vertex.Position.y, quad.Vertex.Position.z)) *
            glm::rotate(glm::mat4(1.0f), glm::radians(quad.Rotation), { 0.0f, 0.0f, 1.0f }) *
            glm::scale(glm::mat4(1.0f), { quad.Size.x, quad.Size.y, 1.0f });


        s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[0];
        s_Data.QuadVertexBufferPtr->Color = { quad.Vertex.Color.x, quad.Vertex.Color.y, quad.Vertex.Color.z, quad.Vertex.Color.w };
        s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f, 0.0f };
        s_Data.QuadVertexBufferPtr->TexId = 32.0f;
        s_Data.QuadVertexBufferPtr->TilingFactor = 1.0f;
        s_Data.QuadVertexBufferPtr++;

        s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[1];
        s_Data.QuadVertexBufferPtr->Color = { quad.Vertex.Color.x, quad.Vertex.Color.y, quad.Vertex.Color.z, quad.Vertex.Color.w };
        s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f, 0.0f };
        s_Data.QuadVertexBufferPtr->TexId = 32.0f;
        s_Data.QuadVertexBufferPtr->TilingFactor = 1.0f;
        s_Data.QuadVertexBufferPtr++;

        s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[2];
        s_Data.QuadVertexBufferPtr->Color = { quad.Vertex.Color.x, quad.Vertex.Color.y, quad.Vertex.Color.z, quad.Vertex.Color.w };
        s_Data.QuadVertexBufferPtr->TexCoord = { 1.f, 1.0f };
        s_Data.QuadVertexBufferPtr->TexId = 32.0f;
        s_Data.QuadVertexBufferPtr->TilingFactor = 1.0f;
        s_Data.QuadVertexBufferPtr++;

        s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[3];
        s_Data.QuadVertexBufferPtr->Color = { quad.Vertex.Color.x, quad.Vertex.Color.y, quad.Vertex.Color.z, quad.Vertex.Color.w };
        s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f, 1.0f };
        s_Data.QuadVertexBufferPtr->TexId = 32.0f;
        s_Data.QuadVertexBufferPtr->TilingFactor = 1.0f;

        s_Data.QuadVertexBufferPtr++;
    }

    void Renderer::DrawQuad(float posX, float posY, float posZ, float sizeX, float sizeY, Texture2D& texture, float tilingFactor)
    {
        float textureIndex = 0.0f;
        
        for (uint32_t i = 1; i < s_Data.TextureIndex; i++)
        {
            if (s_Data.TextureSlots[i] == &texture)
            {
                textureIndex = (float)i;
                break;
            }
        }

        if (textureIndex == 0.0f)
        {
            textureIndex = (float)s_Data.TextureIndex;

            s_Data.TextureSlots[s_Data.TextureIndex] = &texture;
            s_Data.TextureIndex++;
        }

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(posX, posY, posZ)) *
            glm::rotate(glm::mat4(1.0f), glm::radians(texture.GetRotation()), { 0.0f, 0.0f, 1.0f }) *
            glm::scale(glm::mat4(1.0f), { sizeX, sizeY, 1.0f });

        s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[0];
        s_Data.QuadVertexBufferPtr->Color = { 1.0f, 1.0f, 1.0f, 1.0f };
        s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f, 0.0f };
        s_Data.QuadVertexBufferPtr->TexId = textureIndex;
        s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
        s_Data.QuadVertexBufferPtr++;

        s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[1];
        s_Data.QuadVertexBufferPtr->Color = { 1.0f, 1.0f, 1.0f, 1.0f };
        s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f, 0.0f };
        s_Data.QuadVertexBufferPtr->TexId = textureIndex;
        s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
        s_Data.QuadVertexBufferPtr++;

        s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[2];
        s_Data.QuadVertexBufferPtr->Color = { 1.0f, 1.0f, 1.0f, 1.0f };
        s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f, 1.0f };
        s_Data.QuadVertexBufferPtr->TexId = textureIndex;
        s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
        s_Data.QuadVertexBufferPtr++;

        s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[3];
        s_Data.QuadVertexBufferPtr->Color = { 1.0f, 1.0f, 1.0f, 1.0f };
        s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f, 1.0f };
        s_Data.QuadVertexBufferPtr->TexId = textureIndex;
        s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;

        //s_Data.TextureIndex++;
        s_Data.QuadVertexBufferPtr++;
    }
    
    void Renderer::DrawQuad(glm::vec3& position, glm::vec2& size, Texture2D& texture, float tilingFactor)
    {
        float textureIndex = 0.0f;

        for (uint32_t i = 1; i < s_Data.TextureIndex; i++)
        {
            if (s_Data.TextureSlots[i] == &texture)
            {
                textureIndex = (float)i;
                break;
            }
        }

        if (textureIndex == 0.0f)
        {
            textureIndex = (float)s_Data.TextureIndex;

            s_Data.TextureSlots[s_Data.TextureIndex] = &texture;
            s_Data.TextureIndex++;
        }

        DrawQuad(position.x, position.y, position.z, size.x, size.y, texture, tilingFactor);

    }


	void Renderer::DrawIndexed()
	{
       	s_Data.Shader.Bind();
        s_Data.Shader.SetUniformMat4f("u_ViewProjection", m_SceneData->ViewProjectionMatrix);

		s_Data.VertexArray.Bind();
        s_Data.IndexBuffer.Bind();

		glDrawElements(GL_TRIANGLES, s_Data.IndexBuffer.GetCount(), GL_UNSIGNED_INT, nullptr);
        glBindTexture(GL_TEXTURE_2D, 0);
	}

    void Renderer::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
    {
        glViewport(x, y, width, height);
    }

   
}
