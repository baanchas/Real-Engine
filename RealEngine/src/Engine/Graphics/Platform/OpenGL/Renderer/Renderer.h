#pragma once

#include "OpenGL/VertexArray.h"
#include "OpenGL/IndexBuffer.h"
#include "OpenGL/Shader.h"
#include "OrthographicCamera.h"
#include "OpenGL/Texture2D.h"

namespace RealEngine {

	struct Quad
	{
		Vertex Vertex;

		glm::vec2 Size = glm::vec2(100.0, 100.0f);

		float Rotation = 0.0f;
		float TilingFactor = 1.0f;

		glm::mat4 Transform;

		void SetPosition(float x, float y, float z)
		{
			Vertex.Position = { x, y, z };
		}

		void SetPosition(glm::vec3 vec)
		{
			SetPosition(vec.x, vec.y, vec.z);
		}

		inline glm::vec3 GetPosition()
		{
			return Vertex.Position;
		}

		void SetColor(float x, float y, float z, float w)
		{
			Vertex.Color = { x, y, z, w };
		}

		void SetColor(glm::vec4 vec)
		{
			SetColor(vec.x, vec.y, vec.z, vec.w);
		}

		inline glm::vec4 GetColor()
		{
			return Vertex.Color;
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

		inline glm::vec2 GetSize()
		{
			return Size;
		}

		void SetRotation(float rotation)
		{
			Rotation = rotation;
		}

		inline float GetRotation()
		{
			return Rotation;
		}

		inline glm::mat4& GetTransform()
		{
			Transform = glm::translate(glm::mat4(1.0f), glm::vec3(Vertex.Position.x, Vertex.Position.y, Vertex.Position.z)) *
			glm::rotate(glm::mat4(1.0f), glm::radians(Rotation), { 0.0f, 0.0f, 1.0f }) *
			glm::scale(glm::mat4(1.0f), { Size.x, Size.y, 1.0f });

			return Transform;
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

		glm::vec4 QuadVertexPositions[4];
	};

	class Renderer
	{
	public:
		Renderer() {};
		~Renderer() { ShutDown(); };

		static void Init();

		static void BeginScene(Camera& camera, glm::mat4& transform);
		static void BeginScene(OrthographicCamera& camera);
		static void EndScene();
		static void ShutDown();

		static void Clear() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); };
		static void DrawIndexed();

		static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);

		static void DrawQuad(float posX, float posY, float posZ, float sizeX, float sizeY, float rotation, float r, float g, float b, float t, float tf = 1.0f);
		static void DrawQuad(glm::vec3& position, glm::vec2& size, float rotation, glm::vec4& color, float tf = 1.0f);
		static void DrawQuad(Quad& quad);

		static void DrawQuad(const glm::mat4& transform, const glm::vec4& color);
		static void DrawQuad(glm::mat4& transform, Texture2D* texture, float tilingFactor);

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