#include "repch.h"
#include "SceneSerializer.h"
#include "Renderer/SceneCamera.h"

namespace RealEngine {

	SceneSerializer::SceneSerializer(Scene* scene)
		: m_Scene(scene)
	{

	}

	SceneSerializer::~SceneSerializer()
	{
		delete m_Scene;
	}

	void SceneSerializer::Serialize(std::string& filePath)
	{
		std::ofstream out(filePath);
		if (out.is_open())
		{
			out << "Scene: " << m_Scene->GetTitle() << std::endl;
		}

		if (out.is_open(), std::ios::app)
		{
			m_Scene->m_Registry.each([&](auto entityId)
			{
				Entity entity = { entityId, m_Scene };
				if (entity)
				{
					SerializeEntity(entity, filePath);
				}
			});
		}

		out.close();

		ENGINE_INFO("Scene saved at {0}", filePath);
	}

	void SceneSerializer::Deserialize(std::string& filePath)
	{
		std::ifstream in;
		in.open(filePath);

		std::string line;

		Entity tempEntity;

		if (in.is_open())
		{
			while (getline(in, line))
			{

				std::string Scene = "Scene: ";
				size_t foundScene = line.find(Scene);
				if (foundScene != std::string::npos)
				{
					m_Scene->SetTitle(line.substr(foundScene + Scene.length(), line.length()));
				}

				size_t foundEntity = line.find("Entity:");
				if (foundEntity != std::string::npos)
				{
					tempEntity = m_Scene->CreateEntity();
				}

				size_t foundCamera = line.find("Camera Component:");
				if (foundCamera != std::string::npos)
				{
					tempEntity.AddComponent<CameraComponent>();
				}

				size_t foundSprite = line.find("Sprite Renderer");
				if (foundSprite != std::string::npos)
				{
					tempEntity.AddComponent<SpriteRendererComponent>();
				}

				// Tag

				std::string Tag = "Tag: ";
				size_t found = line.find(Tag);
				if (found != std::string::npos)
				{

					auto& tc = tempEntity.GetComponent<TagComponent>();
					tc.Tag = line.substr(found + Tag.length(), line.length());
				}

				// Transform

				std::string Postition = "Position: [ ";
				size_t foundPos = line.find(Postition);
				if (foundPos != std::string::npos)
				{

					if (tempEntity.HasComponent<TransformComponent>())
					{
						auto& tc = tempEntity.GetComponent<TransformComponent>();
						std::string undline = line.substr(foundPos + Postition.length(), line.length() - Postition.length() - 2);
						if (undline.find(",") != std::string::npos)
						{
							tc.Position.x = std::stof(undline.substr(0, undline.find(",")));
							undline = undline.substr(undline.find(",") + 2);

							tc.Position.y = std::stof(undline.substr(0, undline.find(",")));
							undline = undline.substr(undline.find(",") + 2);

							tc.Position.z = std::stof(undline.substr(0, undline.length()));

						}
					}
				}

				std::string Rotation = "Rotation: [ ";
				size_t foundRot = line.find(Rotation);
				if (foundRot != std::string::npos)
				{
					if (tempEntity.HasComponent<TransformComponent>())
					{
						auto& tc = tempEntity.GetComponent<TransformComponent>();
						std::string undline = line.substr(foundRot + Rotation.length(), line.length() - Rotation.length() - 2);
						if (undline.find(",") != std::string::npos)
						{
							tc.Rotation.x = std::stof(undline.substr(0, undline.find(",")));
							undline = undline.substr(undline.find(",") + 2);

							tc.Rotation.y = std::stof(undline.substr(0, undline.find(",")));
							undline = undline.substr(undline.find(",") + 2);

							tc.Rotation.z = std::stof(undline.substr(0, undline.length()));
						}
					}
				}

				std::string Scale = "Scale: [ ";
				size_t foundScale = line.find(Scale);
				if (foundScale != std::string::npos)
				{
					if (tempEntity.HasComponent<TransformComponent>())
					{
						auto& tc = tempEntity.GetComponent<TransformComponent>();
						std::string undline = line.substr(foundScale + Scale.length(), line.length() - Scale.length() - 2);
						if (undline.find(",") != std::string::npos)
						{
							tc.Scale.x = std::stof(undline.substr(0, undline.find(",")));
							undline = undline.substr(undline.find(",") + 2);

							tc.Scale.y = std::stof(undline.substr(0, undline.find(",")));
							undline = undline.substr(undline.find(",") + 2);

							tc.Scale.z = std::stof(undline.substr(0, undline.length()));
						}
					}
				}

				// Camera Stuff

				std::string projectionType = "Projection Type:";
				size_t foundPrType = line.find(projectionType);
				if (foundPrType != std::string::npos)
				{
					if (tempEntity.HasComponent<CameraComponent>())
					{
						auto& cc = tempEntity.GetComponent<CameraComponent>();
						int pType = std::stoi(line.substr(foundPrType + projectionType.length()));
						cc.Camera.SetProjectionType((RealEngine::SceneCamera::ProjectionType)pType);
					}
				}

				std::string OrthographicSize = "Orthographic Size:";
				size_t foundOrthSize = line.find(OrthographicSize);
				if (foundOrthSize != std::string::npos)
				{
					if (tempEntity.HasComponent<CameraComponent>())
					{
						auto& cc = tempEntity.GetComponent<CameraComponent>();
						float size = std::stof(line.substr(foundOrthSize + OrthographicSize.length()));
						cc.Camera.SetOrthographicSize(size);
					}
				}

				std::string OrthographicNear = "Orthographic Near:";
				size_t foundOrthNear = line.find(OrthographicNear);
				if (foundOrthNear != std::string::npos)
				{
					if (tempEntity.HasComponent<CameraComponent>())
					{
						auto& cc = tempEntity.GetComponent<CameraComponent>();
						float size = std::stof(line.substr(foundOrthNear + OrthographicNear.length()));
						cc.Camera.SetOrthographicNearClip(size);
					}
				}

				std::string OrthographicFar = "Orthographic Far:";
				size_t foundOrthFar = line.find(OrthographicFar);
				if (foundOrthFar != std::string::npos)
				{
					if (tempEntity.HasComponent<CameraComponent>())
					{
						auto& cc = tempEntity.GetComponent<CameraComponent>();
						float size = std::stof(line.substr(foundOrthFar + OrthographicFar.length()));
						cc.Camera.SetOrthographicFarClip(size);
					}
				}

				std::string PerspectiveFOV = "Perspective FOV:";
				size_t foundPFOV = line.find(PerspectiveFOV);
				if (foundPFOV != std::string::npos)
				{
					if (tempEntity.HasComponent<CameraComponent>())
					{
						auto& cc = tempEntity.GetComponent<CameraComponent>();
						float size = std::stof(line.substr(foundPFOV + PerspectiveFOV.length()));
						cc.Camera.SetPerspectiveFOV(size);
					}
				}

				std::string PerspectiveNear = "Perspective Near:";
				size_t foundPNear = line.find(PerspectiveNear);
				if (foundPNear != std::string::npos)
				{
					if (tempEntity.HasComponent<CameraComponent>())
					{
						auto& cc = tempEntity.GetComponent<CameraComponent>();
						float size = std::stof(line.substr(foundPNear + PerspectiveNear.length()));
						cc.Camera.SetPerspectiveNearClip(size);
					}
				}

				std::string PerspectiveFar = "Perspective Far:";
				size_t foundPFar = line.find(PerspectiveFar);
				if (foundPFar != std::string::npos)
				{
					if (tempEntity.HasComponent<CameraComponent>())
					{
						auto& cc = tempEntity.GetComponent<CameraComponent>();
						float size = std::stof(line.substr(foundPFar + PerspectiveFar.length()));
						cc.Camera.SetPerspectiveFarClip(size);
					}
				}

				std::string PrimaryCamera = "Primary:";
				size_t foundPrimary = line.find(PrimaryCamera);
				if (foundPrimary != std::string::npos)
				{
					if (tempEntity.HasComponent<CameraComponent>())
					{
						auto& cc = tempEntity.GetComponent<CameraComponent>();
						bool size = std::stof(line.substr(foundPrimary + PrimaryCamera.length()));
						cc.Primary = size;
					}
				}

				std::string isFixedAR = "Is Fixed AR:";
				size_t foundIsFixedAr = line.find(isFixedAR);
				if (foundIsFixedAr != std::string::npos)
				{
					if (tempEntity.HasComponent<CameraComponent>())
					{
						auto& cc = tempEntity.GetComponent<CameraComponent>();
						bool size = std::stof(line.substr(foundIsFixedAr + isFixedAR.length()));
						cc.FixedAspectRatio = size;
					}
				}

				// Sprite

				std::string Color = "Color: [ ";
				size_t foundSpriteIterator = line.find(Color);
				if (foundSpriteIterator != std::string::npos)
				{
					if (tempEntity.HasComponent<SpriteRendererComponent>())
					{
						auto& src = tempEntity.GetComponent<SpriteRendererComponent>();
						std::string undline = line.substr(foundSpriteIterator + Color.length(), line.length() - (foundSpriteIterator + Color.length()) - 2);
						if (undline.find(",") != std::string::npos)
						{
							src.Color.x = std::stof(undline.substr(0, undline.find(",")));
							undline = undline.substr(undline.find(",") + 2);

							src.Color.y = std::stof(undline.substr(0, undline.find(",")));
							undline = undline.substr(undline.find(",") + 2);

							src.Color.z = std::stof(undline.substr(0, undline.length()));
							undline = undline.substr(undline.find(",") + 2);

							src.Color.w = std::stof(undline.substr(0, undline.length()));
						}
					}
				}
			}
		}

		in.close();

		ENGINE_INFO("Scene {0} is loaded!", filePath);
	}

	void SceneSerializer::SerializeEntity(Entity& entity, std::string& filePath)
	{
		std::ofstream out;

		out.open(filePath, std::ios::app);

		if (out.is_open())
		{
			out << "Entity: " << (uint32_t)entity.Get() << std::endl;

			if (entity.HasComponent<TagComponent>())
			{
				auto& tc = entity.GetComponent<TagComponent>();
				out << "\t" << "Tag Component:" << std::endl;
				out << "\t" << "\t" << "Tag: " << tc.Tag << std::endl;
			}

			if (entity.HasComponent<TransformComponent>())
			{
				auto& tc = entity.GetComponent<TransformComponent>();
				out << "\t" << "Transform Component:" << std::endl;
				out << "\t" << "\t" << "Position: " << "[ " << tc.Position.x << ", " << tc.Position.y << ", " << tc.Position.z << " ]" << std::endl;
				out << "\t" << "\t" << "Rotation: " << "[ " << tc.Rotation.x << ", " << tc.Rotation.y << ", " << tc.Rotation.z << " ]" << std::endl;
				out << "\t" << "\t" << "Scale: " << "[ " << tc.Scale.x << ", " << tc.Scale.y << ", " << tc.Scale.z << " ]" << std::endl;
			}

			if (entity.HasComponent<SpriteRendererComponent>())
			{
				auto& sc = entity.GetComponent<SpriteRendererComponent>();
				out << "\t" << "Sprite Renderer Component:" << std::endl;
				out << "\t" << "\t" << "Color: " << "[ " << sc.Color.x << ", " << sc.Color.y << ", " << sc.Color.z << ", " << sc.Color.w << " ]" << std::endl;
			}

			if (entity.HasComponent<ModelComponent>())
			{
				auto& mc = entity.GetComponent<ModelComponent>();
				out << "\t" << "Model Component:" << std::endl;
				for (auto element : mc.Vertices)
				{
					out << "\t" << "Vertex:" << std::endl;
					out << "\t" << "\t" << "VertexPosition: " << "[ " << element.Position.x << ", " << element.Position.y << ", " << element.Position.z << " ]" << std::endl;
					out << "\t" << "\t" << "VertexColor: " << "[ " << element.Color.r << ", " << element.Color.g << ", " << element.Color.b << ", " << element.Color.w << " ]" << std::endl;
					out << "\t" << "\t" << "VertexTextureCoordinate: " << "[ " << element.TexCoord.x << ", " << element.TexCoord.y << " ]" << std::endl;
					out << "\t" << "\t" << "VertexNormal: " << "[ " << element.Normal.x << ", " << element.Normal.y << ", " << element.Normal.z << " ]" << std::endl;
					out << "\t" << "\t" << "TexID: " << element.TexId << std::endl;
					out << "\t" << "\t" << "TilingFactor: " << element.TilingFactor << std::endl;
					out << "\t" << "\t" << "EntityID: " << element.entityID << std::endl;
					out << "\t" << "\t" << "TransformMatrix: " << "[ " << element.Matrix[0][0] << " " << element.Matrix[0][1] << " " << element.Matrix[0][2] << " " << element.Matrix[0][3] << " " << element.Matrix[1][0] << " " << element.Matrix[1][1] << " " << element.Matrix[1][2] << " " << element.Matrix[1][3] << " " << element.Matrix[2][0] << " " << element.Matrix[2][1] << " " << element.Matrix[2][2] << " " << element.Matrix[2][3] << " " << element.Matrix[3][0] << " " << element.Matrix[3][1] << " " << element.Matrix[3][2] << " " << element.Matrix[3][3] << " ]" << std::endl;

				}
			}

			if (entity.HasComponent<CameraComponent>())
			{
				auto& cc = entity.GetComponent<CameraComponent>();
				out << "\t" << "Camera Component:" << std::endl;
				out << "\t" << "\t" << "Projection Type: " << (int)cc.Camera.GetProjectionType() << std::endl;
				out << "\t" << "\t" << "Orthographic Size: " << cc.Camera.GetOrthographicSize() << std::endl;
				out << "\t" << "\t" << "Orthographic Near: " << cc.Camera.GetOrthographicNearClip() << std::endl;
				out << "\t" << "\t" << "Orthographic Far: " << cc.Camera.GetOrthographicFarClip() << std::endl;
				out << "\t" << "\t" << "Perspective FOV: " << cc.Camera.GetPerspectiveFOV() << std::endl;
				out << "\t" << "\t" << "Perspective Near: " << cc.Camera.GetPerspectiveNearClip() << std::endl;
				out << "\t" << "\t" << "Perspective Far: " << cc.Camera.GetPerspectiveFarClip() << std::endl;
				out << "\t" << "\t" << "Primary: " << cc.Primary << std::endl;
				out << "\t" << "\t" << "Is Fixed AR: " << cc.FixedAspectRatio << std::endl;
			}
		}

		out.close();
		ENGINE_INFO("Serialized");

	}
}