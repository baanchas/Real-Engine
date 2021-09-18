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
					tempEntity.AddComponent<SpriteRenderer>();
				}


				size_t foundMesh = line.find("Mesh Component");
				if (foundMesh != std::string::npos)
				{
					tempEntity.AddComponent<MeshComponent>();
				}

				/*size_t foundTexturedMesh = line.find("Mesh Textured Component");
				if (foundTexturedMesh != std::string::npos)
				{
					tempEntity.AddComponent<TexturedMeshComponent>();
				}*/

				size_t foundLight = line.find("Point Light Component");
				if (foundLight != std::string::npos)
				{
					tempEntity.AddComponent<PointLight>();
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

				std::string projectionType = "Projection Type: ";
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

				std::string OrthographicSize = "Orthographic Size: ";
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

				std::string OrthographicNear = "Orthographic Near: ";
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

				std::string OrthographicFar = "Orthographic Far: ";
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

				std::string PerspectiveFOV = "Perspective FOV: ";
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

				std::string PerspectiveNear = "Perspective Near: ";
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

				std::string PerspectiveFar = "Perspective Far: ";
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

				std::string PrimaryCamera = "Primary: ";
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

				std::string isFixedAR = "Is Fixed AR: ";
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
					if (tempEntity.HasComponent<SpriteRenderer>())
					{
						auto& src = tempEntity.GetComponent<SpriteRenderer>();
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

				// Mesh

				std::string Albedo = "Albedo: false: [ ";
				size_t foundAlbedoIterator = line.find(Albedo);
				if (foundAlbedoIterator != std::string::npos)
				{
					if (tempEntity.HasComponent<MeshComponent>())
					{
						auto& src = tempEntity.GetComponent<MeshComponent>();
						std::string undline = line.substr(foundAlbedoIterator + Albedo.length(), line.length() - (foundAlbedoIterator + Albedo.length()) - 2);
						src.isTexturedProperty[0] = false;
						if (undline.find(",") != std::string::npos)
						{

							src.ownMesh.m_Material.Albedo.x = std::stof(undline.substr(0, undline.find(",")));
							undline = undline.substr(undline.find(",") + 2);

							src.ownMesh.m_Material.Albedo.y = std::stof(undline.substr(0, undline.find(",")));
							undline = undline.substr(undline.find(",") + 2);

							src.ownMesh.m_Material.Albedo.z = std::stof(undline.substr(0, undline.length()));
						}
					}
				}

				std::string Metallic = "Metallic: false: ";
				size_t foundMetallicIterator = line.find(Metallic);
				if (foundMetallicIterator != std::string::npos)
				{
					if (tempEntity.HasComponent<MeshComponent>())
					{
						auto& src = tempEntity.GetComponent<MeshComponent>();
						float metallic = std::stof(line.substr(foundMetallicIterator + Metallic.length()));
						src.isTexturedProperty[1] = false;
						src.ownMesh.m_Material.Metallic = metallic;
					}
				}

				std::string Roughness = "Roughness: false: ";
				size_t foundRoughnessIterator = line.find(Roughness);
				if (foundRoughnessIterator != std::string::npos)
				{
					if (tempEntity.HasComponent<MeshComponent>())
					{
						auto& src = tempEntity.GetComponent<MeshComponent>();
						src.isTexturedProperty[2] = false;
						float roughness = std::stof(line.substr(foundRoughnessIterator + Roughness.length()));
						src.ownMesh.m_Material.Roughness = roughness;
					}
				}

				std::string AO = "AO: false: ";
				size_t foundAOIterator = line.find(AO);
				if (foundAOIterator != std::string::npos)
				{
					if (tempEntity.HasComponent<MeshComponent>())
					{
						auto& src = tempEntity.GetComponent<MeshComponent>();
						src.isTexturedProperty[3] = false;
						float ao = std::stof(line.substr(foundAOIterator + AO.length()));
						src.ownMesh.m_Material.AO = ao;
					}
				}


				// Textured Mesh

				std::string AlbedoT = "Albedo: true: ";
				size_t foundTextureAlbedoIterator = line.find(AlbedoT);
				if (foundTextureAlbedoIterator != std::string::npos)
				{
					if (tempEntity.HasComponent<MeshComponent>())
					{
						auto& src = tempEntity.GetComponent<MeshComponent>();
						src.isTexturedProperty[0] = true;
						std::string albedo = line.substr(foundTextureAlbedoIterator + AlbedoT.length());
						src.Textures[0]->LoadFromFile(albedo);
					}
				}

				std::string MetallicT = "Metallic: true: ";
				size_t foundTexturedMetallicIterator = line.find(MetallicT);
				if (foundTexturedMetallicIterator != std::string::npos)
				{
					if (tempEntity.HasComponent<MeshComponent>())
					{
						auto& src = tempEntity.GetComponent<MeshComponent>();
						src.isTexturedProperty[1] = true;
						std::string metallic = line.substr(foundTexturedMetallicIterator + MetallicT.length());
						src.Textures[1]->LoadFromFile(metallic);
					}
				}

				std::string RoughnessT = "Roughness: true: ";
				size_t foundTexturedRoughnessIterator = line.find(RoughnessT);
				if (foundTexturedRoughnessIterator != std::string::npos)
				{
					if (tempEntity.HasComponent<MeshComponent>())
					{
						auto& src = tempEntity.GetComponent<MeshComponent>();
						src.isTexturedProperty[2] = true;
						std::string roughness = line.substr(foundTexturedRoughnessIterator + RoughnessT.length());
						src.Textures[2]->LoadFromFile(roughness);
					}
				}

				std::string AOT = "AO: true: ";
				size_t foundTExturedAOIterator = line.find(AOT);
				if (foundTExturedAOIterator != std::string::npos)
				{
					if (tempEntity.HasComponent<MeshComponent>())
					{
						auto& src = tempEntity.GetComponent<MeshComponent>();
						src.isTexturedProperty[3] = true;
						std::string ao = line.substr(foundTExturedAOIterator + AOT.length());
						src.Textures[3]->LoadFromFile(ao);
					}
				}

				std::string NormalT = "Normal: true: ";
				size_t foundTExturedNormalIterator = line.find(NormalT);
				if (foundTExturedNormalIterator != std::string::npos)
				{
					if (tempEntity.HasComponent<MeshComponent>())
					{
						auto& src = tempEntity.GetComponent<MeshComponent>();
						src.isTexturedProperty[4] = true;
						std::string normal = line.substr(foundTExturedNormalIterator + NormalT.length());
						src.Textures[4]->LoadFromFile(normal);
					}
				}

				std::string path = "REM Path: ";
				size_t foundMeshIterator = line.find(path);
				if (foundMeshIterator != std::string::npos)
				{
					std::ifstream remPath;
					std::string undline = line.substr(foundMeshIterator + path.length());
					remPath.open(undline);

					if (remPath.is_open())
					{
						if (tempEntity.HasComponent<MeshComponent>())
						{
							auto& src = tempEntity.GetComponent<MeshComponent>();
							//REM::ReadFromFileToMesh("assets/test.rem", src.ownMesh);
							MeshLoader::REM::LoadMesh(undline, src.ownMesh);
						}
					}
				}
				

				// Light

				std::string pointLight = "Color: [ ";
				size_t foundPointLightIterator = line.find(pointLight);
				if (foundPointLightIterator != std::string::npos)
				{
					if (tempEntity.HasComponent<PointLight>())
					{
						auto& src = tempEntity.GetComponent<PointLight>();
						std::string undline = line.substr(foundSpriteIterator + Color.length(), line.length() - (foundSpriteIterator + Color.length()) - 2);
						if (undline.find(",") != std::string::npos)
						{
							src.Color.x = std::stof(undline.substr(0, undline.find(",")));
							undline = undline.substr(undline.find(",") + 2);

							src.Color.y = std::stof(undline.substr(0, undline.find(",")));
							undline = undline.substr(undline.find(",") + 2);

							src.Color.z = std::stof(undline.substr(0, undline.length()));
						}
					}
				}

				std::string colorIntensity = "Color Intensity: ";
				size_t foundPointLightIntensityIterator = line.find(colorIntensity);
				if (foundPointLightIntensityIterator != std::string::npos)
				{
					if (tempEntity.HasComponent<PointLight>())
					{
						auto& src = tempEntity.GetComponent<PointLight>();
						float intensity = std::stof(line.substr(foundPointLightIntensityIterator + colorIntensity.length()));
						src.ColorStrength = intensity;
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

			if (entity.HasComponent<SpriteRenderer>())
			{
				auto& sc = entity.GetComponent<SpriteRenderer>();
				out << "\t" << "Sprite Renderer Component:" << std::endl;
				out << "\t" << "\t" << "Color: " << "[ " << sc.Color.x << ", " << sc.Color.y << ", " << sc.Color.z << ", " << sc.Color.w << " ]" << std::endl;
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

			if (entity.HasComponent<MeshComponent>())
			{
				auto& tc = entity.GetComponent<MeshComponent>();
				out << "\t" << "Mesh Component:" << std::endl;
				out << "\t" << "\t" << "REM Path: " << tc.ownMesh.REMFilePath << std::endl;
				out << "\t" << "\t" << "Path: " << tc.ownMesh.FilePath << std::endl;

				if (tc.isTexturedProperty[0])
				{
					out << "\t" << "\t" << "Albedo: true: " << tc.Textures[0]->GetFilePath() << std::endl;
				}
				else 
				{
					out << "\t" << "\t" << "Albedo: false: " << "[ " << tc.ownMesh.m_Material.Albedo.x << ", " << tc.ownMesh.m_Material.Albedo.y << ", " << tc.ownMesh.m_Material.Albedo.z << " ]" << std::endl;
				}

				if (tc.isTexturedProperty[1])
				{
					out << "\t" << "\t" << "Metallic: true: " << tc.Textures[1]->GetFilePath() << std::endl;
				}
				else
				{
					out << "\t" << "\t" << "Metallic: false: " << tc.ownMesh.m_Material.Metallic << std::endl;
				}

				if (tc.isTexturedProperty[2])
				{
					out << "\t" << "\t" << "Roughness: true: " << tc.Textures[2]->GetFilePath() << std::endl;
				}
				else
				{
					out << "\t" << "\t" << "Roughness: false: " << tc.ownMesh.m_Material.Roughness << std::endl;
				}

				if (tc.isTexturedProperty[3])
				{
					out << "\t" << "\t" << "AO: true: " << tc.Textures[3]->GetFilePath() << std::endl;
				}
				else
				{
					out << "\t" << "\t" << "AO: false: " << tc.ownMesh.m_Material.AO << std::endl;
				}

				if (tc.isTexturedProperty[4])
				{
					out << "\t" << "\t" << "Normal: true: " << tc.Textures[4]->GetFilePath() << std::endl;
				}
			}

			if (entity.HasComponent<PointLight>())
			{
				auto& plc = entity.GetComponent<PointLight>();
				out << "\t" << "Point Light Component:" << std::endl;
				out << "\t" << "\t" << "Color: " << "[ " << plc.Color.r << ", " << plc.Color.g << ", " << plc.Color.b << " ]" <<std::endl;
				out << "\t" << "\t" << "Color Intensity: " << plc.ColorStrength << std::endl;
			}
		}

		out.close();
		ENGINE_INFO("Serialized");

	}
}