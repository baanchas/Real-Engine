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

        //auto q3 = Renderer::CreateQuad(100.0f, 100.0f, 200.0f);
        //auto q4 = Renderer::CreateQuad(-200.0f, -200.0f, 200.0f);
        //auto q0 = Renderer::CreateQuadByVerticies(100.0f, 100.0f, 200.0f);
        //auto q1 = Renderer::CreateQuadByVerticies(-200.0f, -200.0f, 200.0f);

        //m_Quads[0] = q3;
        //m_Quads[1] = q4;

        //memcpy(m_Verticies, q0.data(), q0.size() * sizeof(Vertex));
        //memcpy(m_Verticies + q0.size(), q1.data(), q1.size() * sizeof(Vertex));

        //translationC = glm::vec3(0, 0, 0);
        //model = glm::translate(glm::mat4(1.0f), translationA); 
        Renderer::Init();
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

        ImGui::End();
    }

	void EditorLayer::OnEvent(Event& event)
	{
        m_CameraController.OnEvent(event);
   	}

	void EditorLayer::OnRender()
	{
        

        /*m_Quads[0].SetPosition(m_PosX, m_PosY, 200.0f);
        m_Quads[0].SetColor(glm::vec4(0.18f, w, 0.96f, 1.0f));

        m_Quads[1].SetPosition(m_PosX2, m_PosY2, 200.0f);
        m_Quads[1].SetColor(glm::vec4(w, 0.45f, 0.96f, 1.0f));

        model = glm::translate(glm::mat4(1.0f), translationC); // "Models" pos*/
           

        Renderer::BeginScene(m_CameraController.GetCamera());

        Renderer::DrawQuad(m_PosX, m_PosY, 200.0f);
        Renderer::DrawQuad(m_PosX2, m_PosY2, 200.0f);
        //Renderer::DrawIndexed();
        Renderer::EndScene();

        if (w > 1.0f)
            incr = -0.05f;
        else if (w < 0.0f)
            incr = 0.05f;

        w += incr;

		ImGui::Begin("Test");
		ImGui::DragFloat("dragfloat", &a, 1.0f);
		ImGui::End();

	}
}
