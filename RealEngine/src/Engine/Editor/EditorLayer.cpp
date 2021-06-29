#include "repch.h"

#include "EditorLayer.h"
#include "Scene/CameraController.h"
#include "Uilities/OpenGL/OpenGLFileDialogs.h"

namespace RealEngine {

    EditorLayer::EditorLayer()
	{
		ENGINE_INFO("Editor Layer is pushed");
        
        Renderer::Init();
        
        FrameBufferSpecification FrameBufferSpec;
        FrameBufferSpec.m_Width = Application::Get().GetWindow().GetWidth();
        FrameBufferSpec.m_Height = Application::Get().GetWindow().GetHeight();
        m_FrameBuffer = new FrameBuffer(FrameBufferSpec);

        SpriteCheckerBoard.LoadFromFile("res/sprites/checkerboard.png");
                
        m_ActiveScene = new Scene();
        m_SceneHierarchyPanel.SetContext(m_ActiveScene);
        
        m_Serializer.SetContext(m_ActiveScene);

	}

	EditorLayer::~EditorLayer()
	{
        delete m_FrameBuffer;
        delete m_ActiveScene;
	}

    void EditorLayer::OnUpdate(float ts)
    {
        m_ActiveScene->OnUpdate(ts);

        m_ActiveScene->OnViewportResize(m_ViewPortSize.x, m_ViewPortSize.y);

        if (m_SceneWindowIsFocused)
        {
        
        }

    }

	void EditorLayer::OnEvent(Event& event)
	{
        m_ActiveScene->OnEvent(event);

        if (event.Type == EventType::KeyPressed)
        {
            bool control = Input::IsKeyPressed(KeyCodes::LEFT_CONTROL) || Input::IsKeyPressed(KeyCodes::RIGHT_CONTROL);
            bool shift = Input::IsKeyPressed(KeyCodes::LEFT_SHIFT) || Input::IsKeyPressed(KeyCodes::RIGHT_SHIFT);

            switch (event.KeyPressed.Key)
            {
            case KeyCodes::N:
            {
                if (control)
                {
                    NewScene();
                }
                break;
            }
            case KeyCodes::O:
            {
                if (control)
                {
                    OpenScene();
                }
                break;
            }
            case KeyCodes::S:
            {
                if (control && shift)
                {
                    SaveScene();
                }
                break;
            }
            }
        }
   	}

    void EditorLayer::OnImGuiRender()
    {
        bool dockspaceOpen = true;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

        ImGuiWindowFlags Windowflags = ImGuiWindowFlags_MenuBar | ImGuiDockNodeFlags_None | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

        ImGuiViewport* Viewport = ImGui::GetMainViewport();

        ImGui::SetNextWindowPos(Viewport->Pos);
        ImGui::SetNextWindowSize(Viewport->Size);
        ImGui::SetNextWindowViewport(Viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

        ImGui::Begin("DockSpace Demo", &dockspaceOpen, Windowflags);
        ImGui::PopStyleVar();
        ImGui::PopStyleVar(2);
       
        ImGuiIO& io = ImGui::GetIO();
        ImGuiStyle& style = ImGui::GetStyle();
        style.WindowMinSize.x = 370.0f;
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("New", "Ctrl+N"))
                {
                    NewScene();
                }

                if (ImGui::MenuItem("Open...", "Ctrl+O"))
                {
                    OpenScene();
                }
                
                if (ImGui::MenuItem("Save as...", "Ctrl+Shift+S"))
                {
                    SaveScene();
                }
                
                if (ImGui::MenuItem("Exit"))
                {
                    Application::Get().Exit();
                }

                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }
        ImGui::End();

        // Render scene to ImGui window

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

        ImGui::Begin("Scene");
        ImGui::PopStyleVar();

        ImVec2 AvailableContentSize = ImGui::GetContentRegionAvail();
        if (m_ViewPortSize != *((glm::vec2*)&AvailableContentSize))
        {
            m_FrameBuffer->Resize(glm::vec2(AvailableContentSize.x, AvailableContentSize.y));
            m_ViewPortSize = { (uint32_t)AvailableContentSize.x, (uint32_t)AvailableContentSize.y };
        }
        uint32_t TextureID = m_FrameBuffer->GetColorAttachmentID();
        ImGui::Image((void*)TextureID, ImVec2{ m_ViewPortSize.x, m_ViewPortSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

        if (ImGui::IsWindowFocused() || ImGui::IsWindowHovered())
        {
            m_SceneWindowIsFocused = true;
        }
        else
        {
            m_SceneWindowIsFocused = false;
        }

        ImGui::End();

        m_SceneHierarchyPanel.OnImGuiRender();
    }

    void EditorLayer::OnRender()
    {
        m_FrameBuffer->Bind();
        
        Renderer::Clear();

        m_ActiveScene->OnRender();

        m_FrameBuffer->Unbind();

	}

    void EditorLayer::NewScene()
    {
        m_ActiveScene = new Scene();
        m_ActiveScene->OnViewportResize((uint32_t)m_ViewPortSize.x, (uint32_t)m_ViewPortSize.y);

        m_SceneHierarchyPanel.SetContext(m_ActiveScene);
        m_Serializer.SetContext(m_ActiveScene);
    }

    void EditorLayer::OpenScene()
    {
        std::string filePath = FileDialogs::OpenFile("Engine Scene (*.rl)\0*.rl\0");

        if (!filePath.empty())
        {
            m_ActiveScene = new Scene();
            m_ActiveScene->OnViewportResize((uint32_t)m_ViewPortSize.x, (uint32_t)m_ViewPortSize.y);

            m_SceneHierarchyPanel.SetContext(m_ActiveScene);

            m_Serializer.SetContext(m_ActiveScene);
            m_Serializer.Deserialize(filePath);
        }
    }

    void EditorLayer::SaveScene()
    {
        std::string filePath = FileDialogs::SaveFile("Engine Scene (*.rl)\0*.rl\0");
        if (!filePath.empty())
        {
            m_Serializer.SetContext(m_ActiveScene);
            m_Serializer.Serialize(filePath);
        }
    }
}
