#include "repch.h"

#include "EditorLayer.h"


namespace RealEngine {

    EditorLayer::EditorLayer()
        : m_CameraController(1280.0f, 720.0f)
	{
		ENGINE_INFO("Editor Layer is pushed");
        
        Renderer::Init();
        
        FrameBufferSpecification FrameBufferSpec;
        FrameBufferSpec.m_Width = Application::Get().GetWindow().GetWidth();
        FrameBufferSpec.m_Height = Application::Get().GetWindow().GetHeight();
        m_FrameBuffer = new FrameBuffer(FrameBufferSpec);

        SpriteCheckerBoard.LoadFromFile("res/sprites/checkerboard.png");
                
        m_ActiveScene = new Scene();
        
        
        square = m_ActiveScene->CreateEntity();
       // m_CheckerBoardEntity = m_ActiveScene->CreateEntity();
        square.AddComponent<SpriteRendererComponent>(glm::vec4{ 0.0f, 1.0f, 1.0f, 1.0f});

        m_CameraEntity = m_ActiveScene->CreateEntity();
        m_CameraEntity.AddComponent<CameraComponent>();

        m_CameraEntity2 = m_ActiveScene->CreateEntity();
        m_CameraEntity2.AddComponent<CameraComponent>();

        m_CameraEntity2.GetComponent<CameraComponent>().Primary = false;
	}

	EditorLayer::~EditorLayer()
	{
        delete m_FrameBuffer;
        delete m_ActiveScene;
	}

    void EditorLayer::OnUpdate(float ts)
    {
        m_ActiveScene->OnViewportResize(m_ViewPortSize.x, m_ViewPortSize.y);

        if (m_SceneWindowIsFocused)
        {
            m_CameraController.OnUpdate(ts);
        }

    }

	void EditorLayer::OnEvent(Event& event)
	{
        if (m_SceneWindowIsFocused)
            m_CameraController.OnEvent(event);

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
        ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

        ImGui::PushID("Quad 1 - x");
        ImGui::DragFloat("Quad 1 - x", &m_PosX, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::PopID();

        ImGui::PushID("Quad 1 - y");
        ImGui::DragFloat("Quad 1 - y", &m_PosY, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::PopID();

        ImGui::End();
        //
    }

    void EditorLayer::OnRender()
    {
        m_FrameBuffer->Bind();

        Renderer::Clear();

        m_ActiveScene->OnUpdate();
    
        m_FrameBuffer->Unbind();

	}
}
