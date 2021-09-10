#pragma once

#include "glm/glm.hpp"

#include "OpenGL/Texture2D.h"
#include "OpenGL/VertexBuffer.h"
#include "Renderer/SceneCamera.h"
#include "Entity.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Models/Mesh.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace RealEngine{

	class ScriptableEntity;

	struct TagComponent
	{
		std::string Tag = "";

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string tag)
			: Tag(tag) {};

		operator const std::string() { return Tag; }
	};

	struct TransformComponent
	{
		glm::vec3 Position = glm::vec3{ 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation = glm::vec3{ 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale = glm::vec3{ 1.0f, 1.0f, 1.0f };
		

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::vec3& position)
			: Position(position) {};

		glm::mat4 GetTransform() {
			
			glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));

			glm::mat4 transform = glm::translate(glm::mat4(1.0f), Position)
			* rotation
			* glm::scale(glm::mat4(1.0f), Scale);

			return transform;
		}

	};

	struct SpriteRenderer
	{

		glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };

		SpriteRenderer() = default;
		SpriteRenderer(const SpriteRenderer&) = default;
		SpriteRenderer(const glm::vec4& color)
			: Color(color) {};

		operator const glm::vec4() { return Color; }

	};

	struct TextureRenderer
	{
		Texture2D* Texture = nullptr;

		TextureRenderer() = default;
		TextureRenderer(const TextureRenderer&) = default;
		TextureRenderer(Texture2D& texture)
			: Texture(&texture) {};

		operator const Texture2D*() { return Texture; }
	};

	struct PointLight
	{
		glm::vec3 Color = { 1.0f, 1.0f, 1.0f };
		float ColorStrength = 1.0f;


		PointLight() = default;
		PointLight(const PointLight&) = default;
		PointLight(const glm::vec3 position, const glm::vec3 color)
			: Color(color) {};

		operator const glm::vec3() { return Color; }
	};

	struct Spotlight
	{
		glm::vec3 Direction;
		float CutOff;


		Spotlight() = default;
		Spotlight(const Spotlight&) = default;
		Spotlight(const glm::vec3 direction)
			: Direction(direction) {};

		operator const glm::vec3() { return Direction; }
	};

	struct CameraComponent
	{
		SceneCamera Camera;
		bool Primary = true;
		bool FixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
		
	};

	struct NativeScriptComponent
	{
		ScriptableEntity* Instance = nullptr;

		std::function<void()> InstantiateFunction;
		std::function<void()> DestroyInstanceFunction;
		
		std::function<void(ScriptableEntity*)> OnCreateFunction;
		std::function<void(ScriptableEntity*)> OnDestroyFunction;
		std::function<void(ScriptableEntity*, float)> OnUpdateFunction;
		std::function<void(ScriptableEntity*, Event)> OnEventFunction;

		template<typename T>
		void Bind()
		{
			InstantiateFunction = [&]() { Instance = new T(); };
			DestroyInstanceFunction = [&]() { delete (T*)Instance; Instance = nullptr; };

			OnCreateFunction = [](ScriptableEntity* instance) { ((T*)instance)->OnCreate(); };
			OnDestroyFunction = [](ScriptableEntity* instance) { ((T*)instance)->OnDestroy(); };
			OnUpdateFunction = [](ScriptableEntity* instance, float ts) { ((T*)instance)->OnUpdate(ts); };
			OnEventFunction = [](ScriptableEntity* instance, Event event) { ((T*)instance)->OnEvent(event); };
		}
	};

	/*struct MeshComponent
	{
		Mesh ownMesh;

		MeshComponent() = default;
		MeshComponent(const MeshComponent&) = default;
		MeshComponent(Mesh& mesh)
			: ownMesh(mesh) {}
	};*/

	struct MeshComponent
	{
		Mesh ownMesh;

		std::vector<std::shared_ptr<Texture2D>> Textures;
		bool isTexturedProperty[5];

		MeshComponent()
		{ 
			
		};
		MeshComponent(const MeshComponent&) = default;
		MeshComponent(Mesh& mesh)
			: ownMesh(mesh) {}

	};
}