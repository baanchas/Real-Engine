#pragma once

#include "OpenGL/VertexArray.h"
#include "OpenGL/IndexBuffer.h"
#include "OpenGL/Shader.h"
#include "OpenGL/Texture2D.h"
#include "OpenGL/TextureCubeMap.h"
#include "OrthographicCamera.h"
#include "EditorCamera.h"
#include "Models/Mesh.h"

namespace RealEngine {

	
	struct RendererData
	{
		const uint32_t MaxQuads = 25000;
		const uint32_t MaxVertices = 200000;
		const uint32_t MaxIndices = MaxVertices * 3;
		static const uint32_t MaxTextureSlots = 27;

		// SkyBox

		VertexArray SkyboxVertexArray;
		Shader SkyboxShader;
		IndexBuffer SkyboxIndexBuffer;
		VertexBuffer SkyboxVertexBuffer;

		TextureCubeMap CubeMap;

		uint32_t SkyBoxCubeMapIndicesCount = 36;
		uint32_t SkyBoxCubeMapVerticesCount = 108;

		float SkyBoxCubeMapVertexPositions[108] = {
			-1.0f,  1.0f, -1.0f,
			-1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			 1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f,  1.0f,  1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			-1.0f,  1.0f, -1.0f,
			 1.0f,  1.0f, -1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			 1.0f, -1.0f,  1.0f
		};

		uint32_t SkyBoxCubeMapVertexIndices[36];

		// Main Scene

		VertexArray VertexArray;
		Shader Shader;
		IndexBuffer IndexBuffer;
		VertexBuffer VertexBuffer;

		Vertex* QuadVertexBufferBase = nullptr;
		Vertex* VertexBufferBase = nullptr;
		Vertex* VertexBufferPtr = nullptr;

		
		uint32_t* QuadIndexBufferBase = nullptr;
		uint32_t* QuadIndices = nullptr;
		uint32_t* QuadIndicesPtr = nullptr;
		
		std::array<Texture2D*, MaxTextureSlots> TextureSlots;
		
		uint32_t TextureIndex = 0;

		uint32_t IndicesOffset = 0;
		uint32_t QuadIndexCount = 0;
		uint32_t DrawCallsCount = 0;

		glm::vec4 VertexPositions[4];

		//std::vector<Light> SceneLights;
	};

	class Renderer
	{
	public:
		Renderer() {};
		~Renderer() { ShutDown(); };

		static void Init();

		//Scene

		static void BeginScene(Camera& camera, glm::mat4& transform);
		static void BeginScene(EditorCamera& camera);
		static void BeginSkyBoxScene(EditorCamera& camera);
		static void BeginSkyBoxScene(Camera& camera, glm::mat4& transform);
		static void BeginScene(OrthographicCamera& camera);
		static void EndScene();
		static void EndSkyBoxScene();
		static void ShutDown();

		static void BindScene(Shader& shader, VertexArray& va, VertexBuffer& vb, IndexBuffer& ib);
		static void UnbindScene(Shader& shader, VertexArray& va, VertexBuffer& vb, IndexBuffer& ib);
		static void BindTextures();

		static void StartBatch();
		static void ResetMainSceneBuffers();

		//Uniforms

		static void UploadUniformInt(const std::string& name, const int& value);
		static void UploadUniformFloat(const std::string& name, const float& value);
		static void UploadUniformVec3Float(const std::string& name, const glm::vec3& position);

		static void UploadUniformIntArray(const std::string& name, int* value, const int count);
		static void UploadUniformFloatArray(const std::string& name, const float* position, const int count);
		static void UploadUniformVec3Array(const std::string& name, const glm::vec3* position, const int count);

		static void UploadUniformMat4Float(const std::string& name, const glm::mat4& position);

		// Draw

		static void DrawTriangle(const glm::vec3& position, const glm::vec2& size, const float rotation, const glm::vec4& color, float tf = 1.0f);
		
		static void DrawQuad(const glm::mat4& transform, const glm::vec4& color, int entityID);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const float rotation, const glm::vec4& color, const int entityID, float tf = 1.0f);

		static void DrawQuad(const glm::mat4& transform, Texture2D* texture, float tilingFactor, float entityID);
		static void DrawQuad(const glm::vec3& position, glm::vec2& size, Texture2D* texture, float tilingFactor, float entityID);

		static void DrawMesh(const glm::mat4& transform, Mesh& mesh, int entityID);
		static void DrawMesh(const glm::mat4& transform, Mesh& mesh, std::vector<std::shared_ptr<Texture2D>>& textures, bool isTexturedMaterial[5], int entityID);
		static void DrawMesh(const glm::mat4& transform, Mesh& mesh, Material& material, int entityID);

		static void DrawLight(const glm::mat4& transform, Texture2D* texture, int entityID);
		
		static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);

	private:


	private:
		glm::mat4 ViewProjectionMatrix = glm::mat4(1.0f);

		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		static SceneData* m_SceneData;
	};

}