#include "repch.h"

#include "EditorLayer.h"
#include "Scene/CameraController.h"
#include "Uilities/OpenGL/OpenGLFileDialogs.h"
#include "Math/Math.h"
#include <spdlog/spdlog.h>

#include <ImGuizmo.h>
#include <glm/gtc/type_ptr.hpp>

namespace RealEngine {

    EditorLayer::EditorLayer()
	{
		ENGINE_INFO("Editor Layer is pushed");
        
        Renderer::Init();
        
        FrameBufferSpecification FrameBufferSpec;
        FrameBufferSpec.Attachments = { FrameBufferTextureFormat::RGBA8, FrameBufferTextureFormat::RED_INTEGER, FrameBufferTextureFormat::Depth };
        FrameBufferSpec.m_Width = Application::Get().GetWindow().GetWidth();
        FrameBufferSpec.m_Height = Application::Get().GetWindow().GetHeight();
        m_FrameBuffer = new FrameBuffer(FrameBufferSpec);

        m_EditorCamera = EditorCamera(30.0f, 1.778f, 0.1f, 1000.0f);

        SpriteCheckerBoard.LoadFromFile("res/sprites/checkerboard.png");
                
        m_ActiveScene = new Scene();
        m_ActiveScene->SetTitle("Example");
        m_SceneHierarchyPanel.SetContext(m_ActiveScene);

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
            m_EditorCamera.OnUpdate(ts);
        }

    }

	void EditorLayer::OnEvent(Event& event)
	{
        m_ActiveScene->OnEvent(event);

        if (m_SceneWindowIsFocused)
        {
            m_EditorCamera.OnEvent(event);

        }

        if (event.Type == EventType::MouseButtonPressed)
        {
            if (event.MouseButtonPressed.Button == KeyCodes::Mouse::MOUSE_LEFT)
            {
                if (m_HoveredEntity && !ImGuizmo::IsOver() && !Input::IsKeyPressed(KeyCodes::LEFT_ALT))
                {
                    m_SceneHierarchyPanel.SetSelectedEntity(m_HoveredEntity);
                }
            }
        }

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
            case KeyCodes::Q:
            {
                m_GyzmoType = -1;
                break;
            }
            case KeyCodes::W:
            {
                m_GyzmoType = 0;
                break;
            }
            case KeyCodes::E:
            {
                m_GyzmoType = 1;
                break;
            }
            case KeyCodes::R:
            {
                m_GyzmoType = 2;
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
        style.WindowMinSize.x = 150.0f;
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

        auto viewPortOffset = ImGui::GetCursorPos();

        ImVec2 AvailableContentSize = ImGui::GetContentRegionAvail();
        if (m_ViewPortSize != *((glm::vec2*)&AvailableContentSize))
        {
            m_FrameBuffer->Resize(glm::vec2(AvailableContentSize.x, AvailableContentSize.y));
            m_ViewPortSize = { (uint32_t)AvailableContentSize.x, (uint32_t)AvailableContentSize.y };
        }
        m_EditorCamera.SetViewportSize(m_ViewPortSize.x, m_ViewPortSize.y);
        uint32_t TextureID = m_FrameBuffer->GetColorAttachmentID(0);
        ImGui::Image((void*)TextureID, ImVec2{ m_ViewPortSize.x, m_ViewPortSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

        auto windowSize = ImGui::GetWindowContentRegionMax();
        ImVec2 minBound = ImGui::GetWindowPos();
        minBound.x += viewPortOffset.x;
        minBound.y += viewPortOffset.y;

        ImVec2 maxBound = { minBound.x + windowSize.x, minBound.y + windowSize.y };
        m_ViewPortBounds[0] = { minBound.x, minBound.y };
        m_ViewPortBounds[1] = { maxBound.x, maxBound.y };

        auto [mouseX, mouseY] = ImGui::GetMousePos();

        mouseX -= m_ViewPortBounds[0].x;
        mouseY -= m_ViewPortBounds[0].y;
        glm::vec2 viewportSize = m_ViewPortBounds[1] - m_ViewPortBounds[0];
        mouseY = viewportSize.y - mouseY - 22;
        int mX = (int)mouseX;
        int mY = (int)mouseY;

        if (mX >= 0 && mY >= 0 && mX < (int)viewportSize.x && mY < (int)viewportSize.y)
        {
            int pixelData = m_FrameBuffer->ReadPixels(1, mX, mY);
            m_HoveredEntity = Entity{ (entt::entity)pixelData, m_ActiveScene };
        }
        else 
        {
            m_HoveredEntity = Entity{ entt::null, m_ActiveScene };
        }

        if (ImGui::IsWindowHovered())
        {
            m_SceneWindowIsFocused = true;
        }
        else
        {
            m_SceneWindowIsFocused = false;
        }

        Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
        if (selectedEntity && m_GyzmoType != -1 && !Input::IsKeyPressed(KeyCodes::LEFT_ALT))
        {
            ImGuizmo::SetOrthographic(false);
            ImGuizmo::SetDrawlist();

            float windowWidth = (float)ImGui::GetWindowWidth();
            float windowHeight = (float)ImGui::GetWindowHeight();
            ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

            /*Runtime Camera
            auto cameraEntity = m_ActiveScene->GetPrimaryCameraEntity();
            auto& cc = cameraEntity.GetComponent<CameraComponent>();
            const glm::mat4& cameraProjection = cc.Camera.GetProjection();
            glm::mat4 cameraView = glm::inverse(cameraEntity.GetComponent<TransformComponent>().GetTransform());*/

            auto& tc = selectedEntity.GetComponent<TransformComponent>();
            glm::mat4 transform = tc.GetTransform();

            const glm::mat4& cameraProjection = m_EditorCamera.GetProjection();
            glm::mat4 cameraView = m_EditorCamera.GetViewMatrix();

            ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
                (ImGuizmo::OPERATION)m_GyzmoType, ImGuizmo::LOCAL, glm::value_ptr(transform));

            if (ImGuizmo::IsUsing())
            {
                glm::vec3 position, rotation, scale;
                DecomposeTransform(transform, position, rotation, scale);

                glm::vec3 deltaRotation = rotation - tc.Rotation;
                tc.Position = position;
                tc.Rotation = tc.Rotation + deltaRotation;
                tc.Scale = scale;
            }
        }

        ImGui::End();

        m_SceneHierarchyPanel.OnImGuiRender();
    }

    void EditorLayer::OnRender()
    {
        m_FrameBuffer->Bind();
        
        Renderer::Clear();

        m_FrameBuffer->ClearAttachment(1, -1);

        m_ActiveScene->OnRenderEditor(m_EditorCamera);
        //m_ActiveScene->OnRenderRuntime();
        
        OnImGuiRender();

        m_FrameBuffer->UnBind();

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
