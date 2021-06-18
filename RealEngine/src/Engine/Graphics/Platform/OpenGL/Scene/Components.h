#pragma once

#include "glm/glm.hpp"

#include "OpenGL/Renderer/SceneCamera.h"
#include "Entity.h"

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
		glm::mat4 Transform = glm::mat4(1.0f);

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::mat4& transform)
			: Transform(transform) {};

		operator const glm::mat4() { return Transform; }
	};

	struct SpriteRendererComponent
	{

		glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec4& color)
			: Color(color) {};

		operator const glm::vec4() { return Color; }

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
}