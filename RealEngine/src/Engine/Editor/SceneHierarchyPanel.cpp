#include "repch.h"
#include "SceneHierarchyPanel.h"

#include <imgui/imgui_internal.h>

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
					m_SelectedEntity.AddComponent<SpriteRenderer>();
					ImGui::CloseCurrentPopup();
				}

				if (ImGui::MenuItem("Texture Component"))
				{
					m_SelectedEntity.AddComponent<TextureRenderer>();
					ImGui::CloseCurrentPopup();
				}

				if (ImGui::MenuItem("Light"))
				{
					m_SelectedEntity.AddComponent<PointLight>();
					ImGui::CloseCurrentPopup();
				}


				if (ImGui::MenuItem("Mesh Component"))
				{
					m_SelectedEntity.AddComponent<MeshComponent>();
					ImGui::CloseCurrentPopup();
				}

				ImGui::EndPopup();
			}

		}

		ImGui::End();
	}
	
	static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetvalue = 0.0f, float columnWidth = 70.0f)
	{
		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());

		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y + 3.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.0f, 0.0f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.8f, 0.0f, 0.0f, 1.0f));

		if (ImGui::Button("X", buttonSize))
			values.x = resetvalue;

		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.6f, 0.2f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 0.7f, 0.2f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 0.6f, 0.2f, 1.0f));

		if (ImGui::Button("Y", buttonSize))
			values.y = resetvalue;

		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.8f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 0.0f, 1.0f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 0.0f, 0.8f, 1.0f));


		if (ImGui::Button("Z", buttonSize))
			values.z = resetvalue;

		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();
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
				//ImGui::DragFloat3("Position", glm::value_ptr(transform.Position), 0.1f);

				DrawVec3Control("Position", transform.Position);

				glm::vec3 rotation = glm::degrees(transform.Rotation);
				DrawVec3Control("Rotation", rotation);
				transform.Rotation = glm::radians(rotation);

				DrawVec3Control("Scale", transform.Scale, 1.0f);

				ImGui::TreePop();
			}
		}

		if (entity.HasComponent<PointLight>())
		{
			DrawComponent<PointLight>("Light", entity, [](auto& component)
			{
				ImGui::Text("Color:");
				ImGui::ColorEdit3("Color", glm::value_ptr(component.Color));

				auto& strengh = component.ColorStrength;

				ImGui::Text("Light Intensity:");
				ImGui::SliderFloat("slider float", &strengh, 0.0f, 255.0f);
			});
		}

		if (entity.HasComponent<SpriteRenderer>())
		{
			DrawComponent<SpriteRenderer>("Sprite Renderer", entity, [](auto& component)
			{
				ImGui::ColorEdit4("Color", glm::value_ptr(component.Color));
			});
		}

		if (entity.HasComponent<TextureRenderer>())
		{
			DrawComponent<TextureRenderer>("Texture Renderer", entity, [](auto& component)
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
			DrawComponent<MeshComponent>("Textured Mesh", entity, [this](auto& component)
			{
				auto& filePath = component.ownMesh.FilePath;
				auto& remfilePath = component.ownMesh.REMFilePath;
				auto& mc = component.ownMesh;

				if (!filePath.empty())
				{
					ImGui::Text(filePath.c_str());
				}
				else
				{
					ImGui::Text(remfilePath.c_str());
				}

				ImGui::SameLine(ImGui::GetWindowWidth() - 60.0f);

				if (ImGui::Button("Open..."))
				{
					std::string filePath = FileDialogs::OpenFile("Mesh (*.fbx;*rem;)\0*.fbx;*rem;\0");

					if (!filePath.empty())
					{
						Mesh mesh;
						if (filePath.substr(filePath.length() - 4) == ".fbx")
						{
							MeshLoader::FBX::LoadMesh(filePath, mesh);
						}
						else
						{
							MeshLoader::REM::LoadMesh(filePath, mesh);
						}
						component.ownMesh = mesh;
					}
				}

				ImGui::Checkbox("Textured Albedo", &component.isTexturedProperty[0]);
				if (component.isTexturedProperty[0])
				{
					ImGui::Text(component.Textures[0]->GetFilePath().c_str());

					ImGui::SameLine(ImGui::GetWindowWidth() - 60.0f);

					ImGui::PushID("Albedo");
					if (ImGui::Button("Open..."))
					{
						std::string filePath = FileDialogs::OpenFile("Image (*.jpg;*.png;*.tga;*.exr;)\0*.jpg;*.png;*.tga;*.exr;\0");

						if (!filePath.empty())
						{
							auto texture = component.Textures[0];

							texture->LoadFromFile(filePath);
						}
					}

					ImGui::PopID();
				}
				else
				{
					auto& mc = component.ownMesh;

					ImGui::DragFloat3("Albedo", glm::value_ptr(mc.m_Material.Albedo), 0.1f);
				}


				ImGui::Checkbox("Textured Metallness", &component.isTexturedProperty[1]);
				if (component.isTexturedProperty[1])
				{
					ImGui::Text(component.Textures[1]->GetFilePath().c_str());

					ImGui::SameLine(ImGui::GetWindowWidth() - 60.0f);
					ImGui::PushID("Metallness");
					if (ImGui::Button("Open..."))
					{
						std::string filePath = FileDialogs::OpenFile("Image (*.jpg;*.png;*.tga;*.exr;)\0*.jpg;*.png;*.tga;*.exr;\0");

						if (!filePath.empty())
						{
							auto texture = component.Textures[1];

							texture->LoadFromFile(filePath);
						}
					}
					ImGui::PopID();
				}
				else
				{
					auto& mc = component.ownMesh;

					ImGui::DragFloat("Metallnes", &mc.m_Material.Metallic, 0.01f, 0.0f, 1.0f);
				}

				ImGui::Checkbox("Textured Roughness", &component.isTexturedProperty[2]);
				if (component.isTexturedProperty[2])
				{
					ImGui::Text(component.Textures[2]->GetFilePath().c_str());

					ImGui::SameLine(ImGui::GetWindowWidth() - 60.0f);

					ImGui::PushID("Roughness");
					if (ImGui::Button("Open..."))
					{
						std::string filePath = FileDialogs::OpenFile("Image (*.jpg;*.png;*.tga;*.exr;)\0*.jpg;*.png;*.tga;*.exr;\0");

						if (!filePath.empty())
						{
							auto texture = component.Textures[2];

							texture->LoadFromFile(filePath);
						}
					}
					ImGui::PopID();
				}
				else
				{
					auto& mc = component.ownMesh;

					ImGui::DragFloat("Roughness", &mc.m_Material.Roughness, 0.01f, 0.0f, 1.0f);

				}


				ImGui::Checkbox("Textured AO", &component.isTexturedProperty[3]);
				if (component.isTexturedProperty[3])
				{
					ImGui::Text(component.Textures[3]->GetFilePath().c_str());

					ImGui::SameLine(ImGui::GetWindowWidth() - 60.0f);

					ImGui::PushID("AO");
					if (ImGui::Button("Open..."))
					{
						std::string filePath = FileDialogs::OpenFile("Image (*.jpg;*.png;*.tga;*.exr;)\0*.jpg;*.png;*.tga;*.exr;\0");

						if (!filePath.empty())
						{
							auto texture = component.Textures[3];

							texture->LoadFromFile(filePath);
						}
					}
					ImGui::PopID();
				}
				else
				{
					auto& mc = component.ownMesh;

					ImGui::DragFloat("AO", &mc.m_Material.AO, 0.1f);
				}


				ImGui::Checkbox("Textured Normal", &component.isTexturedProperty[4]);
				if (component.isTexturedProperty[4])
				{
					ImGui::Text(component.Textures[4]->GetFilePath().c_str());

					ImGui::SameLine(ImGui::GetWindowWidth() - 60.0f);

					ImGui::PushID("Normal");
					if (ImGui::Button("Open..."))
					{
						std::string filePath = FileDialogs::OpenFile("Image (*.jpg;*.png;*.tga;*.exr;)\0*.jpg;*.png;*.tga;*.exr;\0");

						if (!filePath.empty())
						{
							auto texture = component.Textures[4];

							texture->LoadFromFile(filePath);
						}
					}
					ImGui::PopID();
				}
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