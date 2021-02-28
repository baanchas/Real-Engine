#include "repch.h"

#include "EditorLayer.h"


namespace RealEngine {

    EditorLayer::EditorLayer()
        : m_CameraController(1280.0f, 720.0f)
	{
		ENGINE_INFO("Editor Layer is pushed");

        a = int(5);

        m_PosX = 100.0f;
        m_PosY = 100.0f;
        m_PosX2 = -200.0f;
        m_PosY2 = -200.0f;

        m_PosX3 = -400.0f;
        m_PosY3 = -400.0f;
        
        Renderer::Init();

        quad.Size = 200.0f;
        quad.SetPosition(100.f, 100.f, 0.0f);
        quad.SetColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	}

	EditorLayer::~EditorLayer()
	{
	}

    void EditorLayer::OnUpdate(float ts)
    {
        m_CameraController.OnUpdate(ts);

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
    }

	void EditorLayer::OnEvent(Event& event)
	{
        m_CameraController.OnEvent(event);
   	}

	void EditorLayer::OnRender()
	{

        Renderer::BeginScene(m_CameraController.GetCamera());

        Renderer::DrawQuad(m_PosX, m_PosY, 0.0f, 200.0f, 200.f, 1.0f, 1.0f, 1.0f, 1.0f);
        
        Renderer::DrawQuad(m_PosX2, m_PosY2, 0.0f, 200.0f, 200.f, 0.2f, 0.4f, 0.3f, 1.0f);

        Renderer::DrawQuad(glm::vec3(-400.0f, 0.0f, 0.0f), glm::vec2(100.0f, 50.0f), glm::vec4(0.7f, 0.1f, 1.0f, 1.0f));
        
        quad.SetPosition(m_PosX3, m_PosY3, 0.0f);
        quad.SetColor(glm::vec4(w, 1.0f, 1.0f, 1.0f));
        Renderer::DrawQuad(quad);

        Renderer::EndScene();

        if (w > 1.0f)
            incr = -0.05f;
        else if (w < 0.0f)
            incr = 0.05f;

        w += incr;

	}
}
