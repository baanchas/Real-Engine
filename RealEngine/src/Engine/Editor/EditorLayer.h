#pragma once

#include "Application.h"

#include "Engine/Layers/Layer.h"
#include "Engine/Editor/SceneHierarchyPanel.h"
#include "OpenGL/Shader.h"
#include "OpenGL/VertexArray.h"
#include "OpenGL/VertexBuffer.h"
#include "OpenGL/IndexBuffer.h"
#include "OpenGL/VertexBufferLayout.h"
#include "OpenGL/Texture2D.h"

#include "Renderer/Renderer.h"
#include "Renderer/FrameBuffer.h"
#include "Renderer/EditorCamera.h"
#include "Scene/Scene.h"
//#include "Models/Mesh.h"

namespace RealEngine {

	class CameraController;
	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		~EditorLayer();

		void OnUpdate(float ts) override;
		void OnEvent(Event& event) override;
		void OnImGuiRender() override;
		void OnRender() override;

	private:
		void NewScene();
		void OpenScene();
		void SaveScene();

	private:
		Texture2D SpriteCheckerBoard;

		FrameBuffer* m_FrameBuffer;

		Scene* m_ActiveScene;

		SceneHierarchyPanel m_SceneHierarchyPanel = SceneHierarchyPanel(m_ActiveScene);

		SceneSerializer m_Serializer = SceneSerializer(m_ActiveScene);

		EditorCamera m_EditorCamera;

		Entity m_HoveredEntity;

		Entity model;
		Entity model2;
		Entity model4;
		Entity model3;
		Entity model5;

		Mesh mesh;
		Mesh mesh2;

		std::vector<Vertex> verticesAxe;
		std::vector<glm::uint32_t> indicesAxe;

		glm::vec2 m_ViewPortSize;
		glm::vec2 m_ViewPortBounds[2];

		bool m_SceneWindowIsFocused = false;

		int m_GyzmoType = -1;
	};

}

