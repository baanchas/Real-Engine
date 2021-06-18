#include "repch.h"
#include "SceneHierarchyPanel.h"
#include <glm/gtc/type_ptr.hpp>


namespace RealEngine {

	SceneHierarchyPanel::SceneHierarchyPanel(Scene* scene)
	{
		SetContext(scene);
	}

	void SceneHierarchyPanel::SetContext(Scene* scene)
	{
		m_Context = scene;
	}

	void SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Scene Hierarchy");

		m_Context->m_Registry.each([&](auto entityID)
		{
			Entity entity{ entityID , m_Context };

			auto& tc = entity.GetComponent<TagComponent>().Tag;

			ImGuiTreeNodeFlags flags = ((m_SelectedItem == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
			bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tc.c_str());
			if (ImGui::IsItemClicked())
			{
				m_SelectedItem = entity;
			}

			if (opened)
			{
				ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
				bool opened = ImGui::TreeNodeEx((void*)9817239, flags, tc.c_str());
				if (opened)
					ImGui::TreePop();
				ImGui::TreePop();
			}
		});


		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
		{
			m_SelectedItem = {entt::null, m_Context };
		}

		ImGui::End();

		ImGui::Begin("Properties Panel");

		if (m_SelectedItem)
		{
			DrawComponents(m_SelectedItem);
		}

		ImGui::End();
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

		if (entity.HasComponent<TransformComponent>())
		{
			if (ImGui::TreeNode("Transform"))
			{
				auto& transform = entity.GetComponent<TransformComponent>().Transform;

				ImGui::DragFloat3("Position", glm::value_ptr(transform[3]), 0.1f);

				ImGui::TreePop();
			}
		}

		if (entity.HasComponent<SpriteRendererComponent>())
		{
			if (ImGui::TreeNode("Color"))
			{
				auto& color = entity.GetComponent<SpriteRendererComponent>().Color;

				ImGui::ColorPicker4("Color", glm::value_ptr(color));

				ImGui::TreePop();
			}
		}

		if (entity.HasComponent<CameraComponent>())
		{
			if (ImGui::TreeNode("Camera Component"))
			{
				auto& camera = entity.GetComponent<CameraComponent>().Camera;

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

				ImGui::TreePop();
			}
		}
	}
}