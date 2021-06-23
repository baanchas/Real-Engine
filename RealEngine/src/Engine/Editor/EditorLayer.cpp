#include "repch.h"

#include "EditorLayer.h"
#include "OpenGL/Scene/CameraController.h"

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
        
        //square = m_ActiveScene->CreateEntity("sqaure");
        //square.AddComponent<SpriteRendererComponent>();
        //auto& trc = square.GetComponent<SpriteRendererComponent>();
        //trc.Color = glm::vec4{0.0f, 0.0f, 0.0f, 0.0f};


        square2 = m_ActiveScene->CreateEntity("sqaure2");
        square2.AddComponent<SpriteRendererComponent>();
        auto& trc2 = square2.GetComponent<SpriteRendererComponent>();
        trc2.Color = glm::vec4{ 0.0f, 1.0f, 0.0f, 1.0f };
        //auto& trc2 = square2.GetComponent<TextureRendererComponent>().Texture;
        //trc2 = &SpriteCheckerBoard;
        auto& tcsq2 = square2.GetComponent<TransformComponent>();
        tcsq2.Position = glm::vec3{ -0.35f, 0.0f, 0.0f };
        tcsq2.Rotation = glm::vec3{ 0.0f, -45.0f, 0.0f };

        square3 = m_ActiveScene->CreateEntity("sqaure3");
        square3.AddComponent<SpriteRendererComponent>();
        auto& trc3 = square3.GetComponent<SpriteRendererComponent>();
        trc3.Color = glm::vec4{ 0.0f, 0.0f, 1.0f, 1.0f };
        //square3.AddComponent<SpriteRendererComponent>();
        //auto& trc3 = square3.GetComponent<SpriteRendererComponent>().Color;
        //trc3 = glm::vec4{0.1f, 1.0f, 0.0f, 1.0f};
        auto& tcsq3 = square3.GetComponent<TransformComponent>();
        tcsq3.Position = glm::vec3{ 0.0f, 0.5f, -0.35f };
        tcsq3.Rotation = glm::vec3{ 90.0f, 0.0f, 45.0f };


        m_CameraEntity = m_ActiveScene->CreateEntity("Camera 1");
        m_CameraEntity.AddComponent<CameraComponent>();
        auto& cctc = m_CameraEntity.GetComponent<TransformComponent>();
        cctc.Position = glm::vec3{ 0.0f, 0.85f, 1.7f };
        cctc.Rotation = glm::vec3{ -25.5f, 0.0f, 0.0f };
        m_CameraEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();

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


        if (Input::IsKeyPressed(KeyCodes::A))
        {
            m_CameraEntity2.GetComponent<CameraComponent>().Primary = true;
            m_CameraEntity.GetComponent<CameraComponent>().Primary = false;
        }

        if (Input::IsKeyPressed(KeyCodes::D))
        {
            m_CameraEntity2.GetComponent<CameraComponent>().Primary = false;
            m_CameraEntity.GetComponent<CameraComponent>().Primary = true;
        }

        if (m_SceneWindowIsFocused)
        {
            //CameraController.OnUpdate(ts);
        }

    }

	void EditorLayer::OnEvent(Event& event)
	{
        m_ActiveScene->OnEvent(event);
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

        // DockSpace
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
                if (ImGui::MenuItem("Exit")) Application::Get().Exit();
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }
        ImGui::End();

        //-------- Render scene to ImGui window

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
        //


        m_SceneHierarchyPanel.OnImGuiRender();
        //
    }

    void EditorLayer::OnRender()
    {
        m_FrameBuffer->Bind();
        
        Renderer::Clear();

        m_ActiveScene->OnRender();
           
        m_FrameBuffer->Unbind();

	}
}
