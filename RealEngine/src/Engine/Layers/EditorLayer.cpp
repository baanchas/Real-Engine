#include "repch.h"

#include "EditorLayer.h"


namespace RealEngine {

    EditorLayer::EditorLayer()
        : m_CameraController(1280.0f, 720.0f)
	{
		ENGINE_INFO("Editor Layer is pushed");

        glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f);

        a = int(5);

        m_PosX = 100;
        m_PosY = 100;
        m_PosX2 = 100;
        m_PosY2 = 100;


        translationA = glm::vec3(0, 0, 0);
        translationB = glm::vec3(0, 0, 0);
        translationC = glm::vec3(0, 0, 0);
        model = glm::translate(glm::mat4(1.0f), translationA); 

   
        m_VertexBuffer.Bind();
        m_Shader.Bind();
        m_IndexBuffer.Bind();

		m_Layout.Push<float>(2);
		m_Layout.Push<float>(4);
		m_VertexArray.Addbuffer(m_VertexBuffer, m_Layout);


	}

	EditorLayer::~EditorLayer()
	{
	}

    void EditorLayer::OnUpdate(float ts)
    {
        m_CameraController.OnUpdate(ts);
    }

	void EditorLayer::OnEvent(Event& event)
	{
        m_CameraController.OnEvent(event);

        if (event.Type == EventType::KeyPressed)
        {
            if (event.KeyPressed.Key == KeyCodes::Keyboard::A && event.KeyPressed.Action == GLFW_PRESS)
            {
                ENGINE_INFO("LEFT MOUSE IS PRESSED!");
            }

            if (event.KeyPressed.Key == KeyCodes::Keyboard::A && event.KeyPressed.Action == GLFW_RELEASE)
            {
                ENGINE_INFO("LEFT MOUSE IS RELEASED!");
            }

            if (event.KeyPressed.Key == KeyCodes::Keyboard::A && event.KeyPressed.Action == GLFW_REPEAT)
            {
                ENGINE_INFO("LEFT MOUSE IS REPEATED!");
            }
        }

        if (event.Type == EventType::WindowResized)
        {
            ENGINE_INFO("Window Resized");
            ENGINE_INFO("{0} {1}", event.WindowResized.Width, event.WindowResized.Height);
        }

        if (event.Type == EventType::MouseScrolled)
        {
            ENGINE_INFO("Mouse Scrolled");
            ENGINE_INFO("{0}", event.MouseScrolled.yOffset);
        }
   	}

	void EditorLayer::OnRender()
	{
		//ImGui::ShowDemoWindow();
        m_VertexBuffer.Bind();

        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(positionsRect), positionsRect);

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

        //ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();


        positionsRect[2] = w;
        positionsRect[8] = w;
        positionsRect[14] = w;
        positionsRect[20] = w;

        positionsRect[27] = w;
        positionsRect[33] = w;
        positionsRect[39] = w;
        positionsRect[45] = w;

        positionsRect[0] = m_PosX;
        positionsRect[1] = m_PosY;
        positionsRect[6] = m_PosX + 200.0f;
        positionsRect[7] = m_PosY;
        positionsRect[12] = m_PosX + 200.0f;
        positionsRect[13] = m_PosY + 200.0f;
        positionsRect[18] = m_PosX;
        positionsRect[19] = m_PosY + 200.0f;


        positionsRect[24] = m_PosX2;
        positionsRect[25] = m_PosY2;
        positionsRect[30] = m_PosX2 + 200.0f;
        positionsRect[31] = m_PosY2;
        positionsRect[36] = m_PosX2 + 200.0f;
        positionsRect[37] = m_PosY2 + 200.0f;
        positionsRect[42] = m_PosX2;
        positionsRect[43] = m_PosY2 + 200.0f;

        m_Shader.Bind();
        {
            model = glm::translate(glm::mat4(1.0f), translationC); // "Models" pos
            //mvp = projection * view * model;
           

            m_Shader.SetUniformMat4f("u_MVP", model);
            m_Shader.SetUniformMat4f("u_ViewProjection", m_CameraController.GetCamera().GetViewProjectionMatrix());
        
            Renderer::Draw(m_VertexArray, m_IndexBuffer, m_Shader);
        }

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
