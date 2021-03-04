#pragma once

#include "OpenGL/VertexArray.h"
#include "OpenGL/IndexBuffer.h"
#include "OpenGL/Shader.h"
#include "OrthographicCamera.h"
#include "OpenGL/Texture2D.h"

namespace RealEngine {

	struct Quad
	{
		Vertex Vertex_0;
		Vertex Vertex_1;
		Vertex Vertex_2;
		Vertex Vertex_3;

		glm::vec2 Size = glm::vec2(100.0, 100.0f);

		void SetPosition(float x, float y, float z)
		{
			Vertex_0.Position = { x, y, z };

			Vertex_1.Position = { x + Size.x, y, z };

			Vertex_2.Position = { x + Size.x, y + Size.y, z };

			Vertex_3.Position = { x, y + Size.y, z };
		}

		void SetPosition(glm::vec3 vec)
		{
			SetPosition(vec.x, vec.y, vec.z);
		}

		void SetColor(float x, float y, float z, float w)
		{
			Vertex_0.Color = { x, y, z, w };
			Vertex_1.Color = { x, y, z, w };
			Vertex_2.Color = { x, y, z, w };
			Vertex_3.Color = { x, y, z, w };
		}

		void SetColor(glm::vec4 vec)
		{
			SetColor(vec.x, vec.y, vec.z, vec.w);
		}

		void SetSize(float x, float y)
		{
			SetSize(glm::vec2(x, y));
		}

		void SetSize(glm::vec2 vec)
		{
			Size.x = vec.x;
			Size.y = vec.y;
		}
	};

	struct RendererData
	{
		const uint32_t MaxQuads = 1000;
		const uint32_t MaxVertices = MaxQuads * 4;
		const uint32_t MaxIndices = MaxQuads * 6;
		static const uint32_t MaxTextureSlots = 32;


		VertexArray VertexArray;
		Shader Shader;
		IndexBuffer IndexBuffer;
		VertexBuffer VertexBuffer;


		Vertex* QuadVertexBufferBase = nullptr;
		Vertex* QuadVertexBufferPtr = nullptr;

		std::array<Texture2D*, MaxTextureSlots> TextureSlots;
		uint32_t TextureIndex = 1;
	};

	class Renderer
	{
	public:
		Renderer() {};
		~Renderer() {

		};

		static void Init();

		static void BeginScene(OrthographicCamera& camera);
		static void EndScene();

		static void Clear() { glClear(GL_COLOR_BUFFER_BIT); };
		static void DrawIndexed();

		static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);

		static void DrawQuad(float posX, float posY, float posZ, float sizeX, float sizeY, float r, float g, float b, float t);
		static void DrawQuad(glm::vec3 position, glm::vec2 size, glm::vec4 color);
		static void DrawQuad(Quad quad);
		static void DrawQuad(float posX, float posY, float posZ, float sizeX, float sizeY, Texture2D& texture);

	private:
		glm::mat4 ViewProjectionMatrix = glm::mat4(1.0f);

		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		static SceneData* m_SceneData;
	};

}