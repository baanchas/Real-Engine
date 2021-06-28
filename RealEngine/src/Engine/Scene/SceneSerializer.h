#pragma once

#include "Entity.h"
namespace RealEngine {

	class Scene;
	class Entity;

	class SceneSerializer 
	{
	public:
		SceneSerializer(Scene* scene);
		~SceneSerializer();

		void SetContext(Scene* scene) { m_Scene = scene; }

		void Serialize(std::string& filePath);
		void Deserialize(std::string& filePath);
		void SerializeEntity(Entity& entity, std::string& filePath);

	private:
		Scene* m_Scene = nullptr;

	};

}