#include "repch.h"
#include "SceneHierarchyPanel.h"
#include <glm/gtc/type_ptr.hpp>
#include "Engine/Uilities/OpenGL/OpenGLFileDialogs.h"

namespace RealEngine {

	SceneHierarchyPanel::SceneHierarchyPanel(Scene* scene)
	{
		SetContext(scene);
	}

	void SceneHierarchyPanel::SetContext(Scene* scene)
	{
		m_Context = scene;
		m_SelectedEntity = { entt::null, m_Context };
	}

	void SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Scene Hierarchy");


		m_Context->m_Registry.each([&](auto entityID)
		{
			Entity entity{ entityID , m_Context };

			auto& tc = entity.GetComponent<TagComponent>().Tag;

			ImGuiTreeNodeFlags flags = ((m_SelectedEntity == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
			bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tc.c_str());
			if (ImGui::IsItemClicked())
			{
				m_SelectedEntity = entity;
			}

			if (ImGui::BeginPopupContextItem(0, 1))
			{
				if (ImGui::MenuItem("Delete Entity"))
				{
					ENGINE_WARNING("[{0}]::Entity with id {1} has been destroyed!", m_Context->GetTitle(), m_SelectedEntity.Get());
					if (m_SelectedEntity == entity)
					{
						m_SelectedEntity = { entt::null, m_Context };
					}
					m_Context->m_Registry.destroy(entity);
				}
				ImGui::EndPopup();
			}

			if (opened)
			{
				ImGui::TreePop();
			}
		});

		if (ImGui::BeginPopupContextWindow(0, 1, false))
		{
			if (ImGui::MenuItem("Create Empty Entity"))
			{
				m_Context->CreateEntity("Empty Entity");
			}
			
			ImGui::EndPopup();
		}
					
		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
		{
			m_SelectedEntity = {entt::null, m_Context };
		}
				
		ImGui::End();

		ImGui::Begin("Properties Panel");

		if (m_SelectedEntity)
		{
			DrawComponents(m_SelectedEntity);

			if (ImGui::Button("Add Component"))
			{
				ImGui::OpenPopup("AddComponent");
			}

			if (ImGui::BeginPopup("AddComponent"))
			{
				if (ImGui::MenuItem("Camera"))
				{
					m_SelectedEntity.AddComponent<CameraComponent>();
					ImGui::CloseCurrentPopup();
				}
	
				if (ImGui::MenuItem("Sprite Component"))
				{
					m_SelectedEntity.AddComponent<SpriteRendererComponent>();
					ImGui::CloseCurrentPopup();
				}

				if (ImGui::MenuItem("Texture Component"))
				{
					m_SelectedEntity.AddComponent<TextureRendererComponent>();
					ImGui::CloseCurrentPopup();
				}

				if (ImGui::MenuItem("Light"))
				{
					m_SelectedEntity.AddComponent<Light>();
					ImGui::CloseCurrentPopup();
				}


				if (ImGui::MenuItem("Mesh Component"))
				{
					m_SelectedEntity.AddComponent<MeshComponent>();
					ImGui::CloseCurrentPopup();
				}

				if (ImGui::MenuItem("Textured Mesh Component"))
				{
					m_SelectedEntity.AddComponent<TexturedMeshComponent>();
					ImGui::CloseCurrentPopup();
				}

				ImGui::EndPopup();
			}

		}

		ImGui::End();
	}

	template<typename T, typename UIFunction>
	static void DrawComponent(const std::string& name, Entity entity, UIFunction function)
	{
		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_Framed;

		if (entity.HasComponent<T>())
		{
			auto& sourceComponent = entity.GetComponent<T>();

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
			bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, name.c_str());
			ImGui::SameLine(ImGui::GetWindowWidth() - 40.0f);
			if (ImGui::Button("+", ImVec2{ 20, 20 }))
			{
				ImGui::OpenPopup("ComponentSettings");
			}

			bool removeComponent = false;

			if (ImGui::BeginPopup("ComponentSettings"))
			{
				if (ImGui::MenuItem("Remove Component"))
				{
					removeComponent = true;
				}

				ImGui::EndPopup();
			}

			if (open)
			{
				function(sourceComponent);

				ImGui::TreePop();
			}

			if (removeComponent)
			{
				entity.RemoveComponent<T>();
			}

			ImGui::PopStyleVar();
		}
	}

	void SceneHierarchyPanel::DrawComponents(Entity entity)
	{
		if (entity.HasComponent<TagComponent>())
		{
			auto& tag = entity.GetComponent<TagComponent>().Tag;

			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy(buffer, tag.c_str());

			if (ImGui::InputText("Tag", buffer, sizeof(buffer)))
			{
				tag = std::string(buffer);
			}
		}

		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_Framed;

		if (entity.HasComponent<TransformComponent>())
		{
			bool open = ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), treeNodeFlags, "Transform");
			
			if (open)
			{
				auto& transform = entity.GetComponent<TransformComponent>();
				ImGui::DragFloat3("Position", glm::value_ptr(transform.Position), 0.1f);

				glm::vec3 rotation = glm::degrees(transform.Rotation);
				ImGui::DragFloat3("Rotation", glm::value_ptr(rotation), 0.1f);
				transform.Rotation = glm::radians(rotation);

				ImGui::DragFloat3("Scale", glm::value_ptr(transform.Scale), 0.1f);

				ImGui::TreePop();
			}
		}

		if (entity.HasComponent<Light>())
		{
			DrawComponent<Light>("Light", entity, [](auto& component)
			{
				ImGui::Text("Color:");
				ImGui::ColorPicker3("Color", glm::value_ptr(component.Color));

				ENGINE_TRACE("{0} {1} {2}", component.Color.x, component.Color.y, component.Color.z);

				auto& strengh = component.ColorStrength;
				ImGui::Text("Color Strengh:");
				ImGui::SliderFloat("slider float", &strengh, 0.0f, 255.0f);
			});
		}

		if (entity.HasComponent<SpriteRendererComponent>())
		{
			DrawComponent<SpriteRendererComponent>("Sprite Renderer", entity, [](auto& component)
			{
				ImGui::ColorPicker4("Color", glm::value_ptr(component.Color));
			});
		}

		if (entity.HasComponent<TextureRendererComponent>())
		{
			DrawComponent<TextureRendererComponent>("Texture Renderer", entity, [](auto& component)
			{
				if (component.Texture != nullptr)
				{
					ImGui::Text(component.Texture->GetFilePath().c_str());
				}
				ImGui::SameLine(ImGui::GetWindowWidth() - 60.0f);
				if (ImGui::Button("Open..."))
				{
					std::string filePath = FileDialogs::OpenFile("Image (*.jpg;*.png;)\0*.jpg;*.png;\0");
						
					if (!filePath.empty())
					{
						component.Texture->LoadFromFile(filePath);
					}
				}
			});
		}

		if (entity.HasComponent<MeshComponent>())
		{
			DrawComponent<MeshComponent>("Mesh", entity, [](auto& component)
			{
				auto& filePath = component.ownMesh.FilePath;

				auto& mc = component.ownMesh;

				ImGui::Text(filePath.c_str());

				ImGui::SameLine(ImGui::GetWindowWidth() - 60.0f);

				if (ImGui::Button("Open..."))
				{
					std::string filePath = FileDialogs::OpenFile("Mesh (*.fbx)\0*.fbx\0");

					if (!filePath.empty())
					{
						Mesh mesh;
						mesh.m_Material.Albedo = glm::vec3{ 0.8f, 0.8f, 0.8f };
						mesh.m_Material.Metallic = 1.0f;
						mesh.m_Material.Roughness = 0.5f;
						mesh.m_Material.AO = 1.0f;
						MeshLoader::LoadMeshFromFBX(filePath, mesh);

						mc = mesh;
					}
				}

				ImGui::DragFloat3("Albedo", glm::value_ptr(mc.m_Material.Albedo), 0.1f);

				ImGui::DragFloat("Metallnes", &mc.m_Material.Metallic, 0.1f);

				ImGui::DragFloat("Roughness", &mc.m_Material.Roughness, 0.1f);

				ImGui::DragFloat("AO", &mc.m_Material.AO, 0.1f);
			});
		}

		if (entity.HasComponent<TexturedMeshComponent>())
		{
			DrawComponent<TexturedMeshComponent>("Textured Mesh", entity, [](auto& component)
			{
				auto& filePath = component.ownMesh.FilePath;

				ImGui::Text(filePath.c_str());

				ImGui::SameLine(ImGui::GetWindowWidth() - 60.0f);

				if (ImGui::Button("Open..."))
				{
					std::string filePath = FileDialogs::OpenFile("Mesh (*.fbx)\0*.fbx\0");

					if (!filePath.empty())
					{
						Mesh mesh;
						MeshLoader::LoadMeshFromFBX(filePath, mesh);

						component.ownMesh = mesh;
					}
				}

				ImGui::Text(component.Textures[0].GetFilePath().c_str());

				ImGui::SameLine(ImGui::GetWindowWidth() - 60.0f);

				ImGui::PushID("Albedo");
				if (ImGui::Button("Open..."))
				{
					std::string filePath = FileDialogs::OpenFile("Image (*.jpg;*.png;)\0*.jpg;*.png;\0");

					if (!filePath.empty())
					{
						auto& texture = component.Textures[0];

						texture.LoadFromFile(filePath);
					}
				}

				ImGui::PopID();

				ImGui::Text(component.Textures[1].GetFilePath().c_str());

				ImGui::SameLine(ImGui::GetWindowWidth() - 60.0f);
				ImGui::PushID("Metallness");
				if (ImGui::Button("Open..."))
				{
					std::string filePath = FileDialogs::OpenFile("Image (*.jpg;*.png;)\0*.jpg;*.png;\0");

					if (!filePath.empty())
					{
						auto& texture = component.Textures[1];

						texture.LoadFromFile(filePath);
					}
				}
				ImGui::PopID();

				ImGui::Text(component.Textures[2].GetFilePath().c_str());

				ImGui::SameLine(ImGui::GetWindowWidth() - 60.0f);

				ImGui::PushID("Roughness");
				if (ImGui::Button("Open..."))
				{
					std::string filePath = FileDialogs::OpenFile("Image (*.jpg;*.png;)\0*.jpg;*.png;\0");

					if (!filePath.empty())
					{
						auto& texture = component.Textures[2];

						texture.LoadFromFile(filePath);
					}
				}
				ImGui::PopID();

				ImGui::Text(component.Textures[3].GetFilePath().c_str());

				ImGui::SameLine(ImGui::GetWindowWidth() - 60.0f);

				ImGui::PushID("AO");
				if (ImGui::Button("Open..."))
				{
					std::string filePath = FileDialogs::OpenFile("Image (*.jpg;*.png;)\0*.jpg;*.png;\0");

					if (!filePath.empty())
					{
						auto& texture = component.Textures[3];

						texture.LoadFromFile(filePath);
					}
				}
				ImGui::PopID();

				ImGui::Text(component.Textures[4].GetFilePath().c_str());

				ImGui::SameLine(ImGui::GetWindowWidth() - 60.0f);

				ImGui::PushID("Normal");
				if (ImGui::Button("Open..."))
				{
					std::string filePath = FileDialogs::OpenFile("Image (*.jpg;*.png;)\0*.jpg;*.png;\0");

					if (!filePath.empty())
					{
						auto& texture = component.Textures[4];

						texture.LoadFromFile(filePath);
					}
				}
				ImGui::PopID();
			});
		}

		if (entity.HasComponent<CameraComponent>())
		{
			DrawComponent<CameraComponent>("Camera", entity, [](auto& component)
			{
				auto& camera = component.Camera;

				const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };
				const char* currentProjectionTyoeString = projectionTypeStrings[(int)camera.GetProjectionType()];

				if (ImGui::BeginCombo("Projection", currentProjectionTyoeString))
				{
					for (int i = 0; i < 2; i++)
					{
						bool isSelected = currentProjectionTyoeString == projectionTypeStrings[i];
						if (ImGui::Selectable(projectionTypeStrings[i], isSelected))
						{
							currentProjectionTyoeString = projectionTypeStrings[i];
							camera.SetProjectionType((SceneCamera::ProjectionType)i);
						}

						if (isSelected)
						{
							ImGui::SetItemDefaultFocus();
						}
					}

					ImGui::EndCombo();
				}

				if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective)
				{
					float fov = glm::degrees(camera.GetPerspectiveFOV());
					if (ImGui::DragFloat("Vertical FOV", &fov))
					{
						camera.SetPerspectiveFOV(fov);
					}

					float nearClip = camera.GetPerspectiveNearClip();
					if (ImGui::DragFloat("Near Clip", &nearClip))
					{
						camera.SetPerspectiveNearClip(nearClip);
					}

					float farClip = camera.GetPerspectiveFarClip();
					if (ImGui::DragFloat("Far Clip", &farClip))
					{
						camera.SetPerspectiveFarClip(farClip);
					}
				}

				if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
				{
					float orthoSize = camera.GetOrthographicSize();
					if (ImGui::DragFloat("Size", &orthoSize))
					{
						camera.SetOrthographicSize(orthoSize);
					}

					float nearClip = camera.GetOrthographicNearClip();
					if (ImGui::DragFloat("Near Clip", &nearClip))
					{
						camera.SetOrthographicNearClip(nearClip);
					}

					float farClip = camera.GetOrthographicFarClip();
					if (ImGui::DragFloat("Far Clip", &farClip))
					{
						camera.SetOrthographicFarClip(farClip);
					}
				}
			});
		}
	}
}