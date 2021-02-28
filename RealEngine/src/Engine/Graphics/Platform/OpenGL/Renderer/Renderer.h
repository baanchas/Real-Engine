#pragma once

#include "OpenGL/VertexArray.h"
#include "OpenGL/IndexBuffer.h"
#include "OpenGL/Shader.h"
#include "OrthographicCamera.h"

namespace RealEngine {

	struct Quad
	{
		Vertex Vertex_0;
		Vertex Vertex_1;
		Vertex Vertex_2;
		Vertex Vertex_3;

		void SetPosition(float x, float y, float size)
		{
			Vertex_0.Position = { x, y };

			Vertex_1.Position = { x + size, y };

			Vertex_2.Position = { x + size, y + size };

			Vertex_3.Position = { x, y + size };
		}

		void SetColor(glm::vec4 vec)
		{
			Vertex_0.Color = { vec.x, vec.y, vec.z, vec.w };
			Vertex_1.Color = { vec.x, vec.y, vec.z, vec.w };
			Vertex_2.Color = { vec.x, vec.y, vec.z, vec.w };
			Vertex_3.Color = { vec.x, vec.y, vec.z, vec.w };
		}

		void* GetData()
		{
			//return (void*)&Vertex_0 + (void*)&Vertex_0;
		}
	};

	struct RendererData
	{
		const uint32_t MaxQuads = 1000;
		const uint32_t MaxVertices = MaxQuads * 4;
		const uint32_t MaxIndices = MaxQuads * 6;

		VertexArray VertexArray;
		Shader Shader;
		IndexBuffer IndexBuffer;
		VertexBuffer VertexBuffer;

		uint32_t Indices[10000];

		uint32_t IndexCount;

		Vertex* QuadVertexBufferBase = nullptr;
		Vertex* QuadVertexBufferPtr = nullptr;
	};

	class Renderer
	{
	public:
		Renderer() {};
		~Renderer() {
			//delete s_Data.QuadVertexBufferBase;
			//delete s_Data.QuadVertexBufferPtr;
		};

		static void Init();

		static void BeginScene(OrthographicCamera& camera);
		static void EndScene();

		static void Clear() { glClear(GL_COLOR_BUFFER_BIT); };
		static void DrawIndexed();

		static void DrawQuad(float posX, float posY, float size);

		static Quad CreateQuad(float x, float y, float size);
		static std::array<Vertex, 4> CreateQuadByVerticies(float x, float y, float size);

	private:
		glm::mat4 ViewProjectionMatrix = glm::mat4(1.0f);

		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		static SceneData* m_SceneData;
	};

}