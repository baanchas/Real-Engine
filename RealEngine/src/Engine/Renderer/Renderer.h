#pragma once

#include "OpenGL/VertexArray.h"
#include "OpenGL/IndexBuffer.h"
#include "OpenGL/Shader.h"
#include "OpenGL/Texture2D.h"
#include "OrthographicCamera.h"
#include "EditorCamera.h"
#include "Models/Mesh.h"

namespace RealEngine {

	
	struct RendererData
	{
		const uint32_t MaxQuads = 25000;
		const uint32_t MaxVertices = 200000;
		const uint32_t MaxIndices = MaxVertices * 3;
		static const uint32_t MaxTextureSlots = 32;


		VertexArray VertexArray;
		Shader Shader;
		IndexBuffer IndexBuffer;
		VertexBuffer VertexBuffer;

		int m_EntityID = 0;

		Vertex* VertexBufferBase = nullptr;
		Vertex* VertexBufferPtr = nullptr;

		std::array<Texture2D, MaxTextureSlots> TextureSlots;
		uint32_t TextureIndex = 1;


		uint32_t QuadIndexCount = 0;

		uint32_t* QuadIndices = nullptr;
		uint32_t* QuadIndicesPtr = nullptr;

		int IndicesOffset = 0;
		int IndicesIndex = 0;

		glm::vec4 VertexPositions[4];
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
		static void SetClearColor(float r, float g, float b, float a) { glClearColor(r, g, b, a); };

		static void SetUniform3f(const std::string& name, const glm::vec3& position);

		static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);

		static void DrawQuad(const glm::mat4& transform, const glm::vec4& color, int entityID);
		static void DrawQuad(float posX, float posY, float posZ, float sizeX, float sizeY, float rotation, float r, float g, float b, float t, int entityID, float tf = 1.0f);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const float rotation, const glm::vec4& color, const int entityID, float tf = 1.0f);


		static void DrawQuad(const glm::mat4& transform, Texture2D& texture, float tilingFactor, float entityID);
		static void DrawQuad(float posX, float posY, float posZ, float sizeX, float sizeY, Texture2D& texture, float tilingFactor);
		static void DrawQuad(glm::vec3& position, glm::vec2& size, Texture2D& texture, float tilingFactor);

		static void DrawTriangle(const glm::vec3& position, const glm::vec2& size, const float rotation, const glm::vec4& color, float tf = 1.0f);

		static void DrawModel(const glm::mat4& transform, const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, int entityID);
		static void DrawMesh(const glm::mat4& transform, Mesh& mesh, int entityID);

	private:
		glm::mat4 ViewProjectionMatrix = glm::mat4(1.0f);

		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		static SceneData* m_SceneData;
	};

}