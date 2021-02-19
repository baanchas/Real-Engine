#include "repch.h"

#include "EditorLayer.h"


namespace RealEngine {

	EditorLayer::EditorLayer()
	{
		ENGINE_INFO("Editor Layer is pushed");

        a = int(5);

        m_PosX = 100;
        m_PosY = 100;
        m_PosX2 = 100;
        m_PosY2 = 100;


        translationA = glm::vec3(0, 0, 0);
        translationB = glm::vec3(0, 0, 0);
        translationC = glm::vec3(0, 0, 0);
        projection = glm::ortho(0.0f, 1280.0f, 0.0f, 720.0f, -1.0f, 1.0f); // projection
        view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)); // "Camera" pos
        model = glm::translate(glm::mat4(1.0f), translationA); // "Models" pos
        //glm::mat4 model = glm::translate(glm::mat4(1.0f), translationB); // "Models" pos
        mvp = projection * view * model;

   
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

    void EditorLayer::OnUpdate()
    {
        ENGINE_INFO("{0}", a);
    }

	void EditorLayer::OnEvent()
	{
	}

	void EditorLayer::OnRender()
	{
		//ImGui::ShowDemoWindow();
        m_VertexBuffer.Bind();

        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(positionsRect), positionsRect);

        ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.


        ImGui::PushID("float 4");
        ImGui::DragFloat("float 4", &m_PosX, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::PopID();

        ImGui::PushID("float 5");
        ImGui::DragFloat("float 5", &m_PosY, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::PopID();

        ImGui::PushID("float");
        ImGui::DragFloat("float", &m_PosX2, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::PopID();

        ImGui::PushID("float 1");
        ImGui::DragFloat("float 1", &m_PosY2, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
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
        positionsRect[12] = m_PosX + 200.0;
        positionsRect[13] = m_PosY + 200.0;
        positionsRect[18] = m_PosX;
        positionsRect[19] = m_PosY + 200.0;


        positionsRect[24] = m_PosX2;
        positionsRect[25] = m_PosY2;
        positionsRect[30] = m_PosX2 + 200.0f;
        positionsRect[31] = m_PosY2;
        positionsRect[36] = m_PosX2 + 200.0;
        positionsRect[37] = m_PosY2 + 200.0;
        positionsRect[42] = m_PosX2;
        positionsRect[43] = m_PosY2 + 200.0;

        m_Shader.Bind();
        {

            model = glm::translate(glm::mat4(1.0f), translationC); // "Models" pos
            mvp = projection * view * model;


            m_Shader.SetUniformMat4f("u_MVP", mvp);
           // m_Shader.SetUniform4f("u_Color", r, 1.0f, 0.3f, 1.0f);

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
