#pragma once

#include "Application.h"

#include "Engine/Layers/Layer.h"
#include "OpenGL/Shader.h"
#include "OpenGL/VertexArray.h"
#include "OpenGL/VertexBuffer.h"
#include "OpenGL/IndexBuffer.h"
#include "OpenGL/VertexBufferLayout.h"
#include "OpenGL/Texture2D.h"

#include "Renderer/Renderer.h"
#include "Renderer/FrameBuffer.h"
#include "Scene/Scene.h"

#include "Renderer/EditorCamera.h"

#include "Engine/Editor/SceneHierarchyPanel.h"
#include "Engine/Editor/ContentBrowserPanel.h"

//#include "Models/Mesh.h"

namespace RealEngine {

	class CameraController;
	enum SceneMode
	{
		EDIT,
		PLAY,
	};

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
		void UIToolBar();
		void NewScene();
		void OpenScene();
		void OpenScene(std::string& path);
		void OpenScene(Scene* activeScene, std::string& path);
		void SaveScene();

		void OnScenePlay();
		void OnSceneEdit();

	private:
		Scene* m_ActiveScene;
		FrameBuffer* m_FrameBuffer;

		SceneMode m_SceneState = SceneMode::EDIT;

		SceneHierarchyPanel m_SceneHierarchyPanel = SceneHierarchyPanel(m_ActiveScene);
		ContentBrowserPanel m_ContentBrowserPanel;

		SceneSerializer m_Serializer = SceneSerializer(m_ActiveScene);

		EditorCamera m_EditorCamera;

		Entity m_HoveredEntity;

		bool m_SceneWindowIsFocused = false;

		int m_GyzmoType = -1;

		Texture2D m_IconPlay;
		Texture2D m_IconStop;

		Texture2D m_Folder;
		Texture2D m_File;

		glm::vec2 m_ViewPortSize;
		glm::vec2 m_ViewPortBounds[2];
	};

}

