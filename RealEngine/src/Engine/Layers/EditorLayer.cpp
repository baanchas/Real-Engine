#include "repch.h"

#include "EditorLayer.h"


namespace RealEngine {

    EditorLayer::EditorLayer()
        : m_CameraController(1280.0f, 720.0f)
	{
		ENGINE_INFO("Editor Layer is pushed");
        
        Renderer::Init();

        texture.LoadFromFile("res/sprites/checkerboard.png");
        
        FrameBufferSpecification spec;
        spec.m_Width = Application::Get().GetWindow().GetWidth();
        spec.m_Height = Application::Get().GetWindow().GetHeight();
        m_FrameBuffer = new FrameBuffer(spec);
        
        m_PosX = 100.0f;
        m_PosY = 100.0f;
        m_PosX2 = -200.0f;
        m_PosY2 = -200.0f;
        m_PosX3 = -400.0f;
        m_PosY3 = -400.0f;
        

        quad.Size = glm::vec2(200.0f, 200.0f);
        quad.SetPosition(100.f, 100.f, 0.0f);
        quad.SetColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
        quad.SetRotation(45.0f);

	}

	EditorLayer::~EditorLayer()
	{
        delete m_FrameBuffer;
	}

    void EditorLayer::OnUpdate(float ts)
    {
        m_CameraController.OnUpdate(ts);
    }

	void EditorLayer::OnEvent(Event& event)
	{
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
            
            m_CameraController.OnBoundsResize(AvailableContentSize.x, AvailableContentSize.y);
        }
        uint32_t TextureID = m_FrameBuffer->GetColorAttachmentID();
        ImGui::Image((void*)TextureID, ImVec2{ m_ViewPortSize.x, m_ViewPortSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
        ImGui::End();

        //
        ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

        ImGui::PushID("Quad 1 - x");
        ImGui::DragFloat("Quad 1 - x", &m_PosX, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::PopID();

        ImGui::PushID("Quad 1 - y");
        ImGui::DragFloat("Quad 1 - y", &m_PosY, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::PopID();

        ImGui::PushID("Quad 2 - x");
        ImGui::DragFloat("Quad 2 - x", &m_PosX2, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::PopID();

        ImGui::PushID("Quad 2 - y");
        ImGui::DragFloat("Quad 2 - y", &m_PosY2, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::PopID();

        ImGui::PushID("Quad 3 - x");
        ImGui::DragFloat("Quad 3 - x", &m_PosX3, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::PopID();

        ImGui::PushID("Quad 3 - y");
        ImGui::DragFloat("Quad 3 - y", &m_PosY3, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::PopID();

        ImGui::End();
        //
    }

    void EditorLayer::OnRender()
    {
        m_FrameBuffer->Bind();
        texture.Bind();

        Renderer::Clear();
        Renderer::BeginScene(m_CameraController.GetCamera());

        glm::vec4 color;
        float inc = 1.0f;

        glm::vec3 pos = { 0.0f, 0.0f, 0.0f };
        glm::vec2 size = { 3000.0f, 3000.0f };

        Renderer::DrawQuad(pos.x, pos.y, pos.z, size.x, size.y, texture, 10.0f);

        quad.SetSize(200.f, 200.0f);
        quad.SetPosition(m_PosX3, m_PosY3, 0.0f);

        if (w > 1.0f)
            incr = -0.05f;
        else if (w < 0.0f)
            incr = 0.05f;

        w += incr;

        quad.SetColor(glm::vec4(w, 1.0f, 1.0f, 1.0f));
        quad.SetRotation(quad.GetRotation() - inc);

        Renderer::DrawQuad(quad);
        Renderer::DrawQuad(m_PosX2, m_PosY2, 0.0f, 200.0f, 200.0f, 0.0f, 0.2f, 0.4f, 1.0f, 1.0f);
        Renderer::DrawQuad(m_PosX, m_PosY, 0.0f, 200.0f, 200.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
        
        Renderer::EndScene();

        m_FrameBuffer->Unbind();

	}
}
