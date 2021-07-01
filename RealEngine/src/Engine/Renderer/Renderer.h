#pragma once

#include "OpenGL/VertexArray.h"
#include "OpenGL/IndexBuffer.h"
#include "OpenGL/Shader.h"
#include "OpenGL/Texture2D.h"
#include "OrthographicCamera.h"
#include "EditorCamera.h"

namespace RealEngine {

	
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

		uint32_t QuadIndexCount = 0;
		int m_EntityID = 0;

		Vertex* QuadVertexBufferBase = nullptr;
		Vertex* QuadVertexBufferPtr = nullptr;

		std::array<Texture2D*, MaxTextureSlots> TextureSlots;
		uint32_t TextureIndex = 1;

		glm::vec4 QuadVertexPositions[4];
	};

	class Renderer
	{
	public:
		Renderer() {};
		~Renderer() { ShutDown(); };

		static void Init();

		static void BeginScene(Camera& camera, glm::mat4& transform);
		static void BeginScene(EditorCamera& camera);
		static void BeginScene(OrthographicCamera& camera);
		static void EndScene();
		static void ShutDown();

		static void Clear() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); };
		
		static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);

		static void DrawQuad(float posX, float posY, float posZ, float sizeX, float sizeY, float rotation, float r, float g, float b, float t, float tf = 1.0f);
		static void DrawQuad(glm::vec3& position, glm::vec2& size, float rotation, glm::vec4& color, float tf = 1.0f);

		static void DrawQuad(const glm::mat4& transform, const glm::vec4& color);
		static void DrawQuad(const glm::mat4& transform, const glm::vec4& color, int entityID);
		static void DrawQuad(const glm::mat4& transform, Texture2D* texture, float tilingFactor);

		static void DrawQuad(float posX, float posY, float posZ, float sizeX, float sizeY, Texture2D& texture, float tilingFactor);
		static void DrawQuad(glm::vec3& position, glm::vec2& size, Texture2D& texture, float tilingFactor);

	private:
		glm::mat4 ViewProjectionMatrix = glm::mat4(1.0f);

		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		static SceneData* m_SceneData;
	};

}