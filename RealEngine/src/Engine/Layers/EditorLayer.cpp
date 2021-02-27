#include "repch.h"

#include "EditorLayer.h"


namespace RealEngine {

    EditorLayer::EditorLayer()
        : m_CameraController(1280.0f, 720.0f)
	{
		ENGINE_INFO("Editor Layer is pushed");

        a = int(5);

        m_PosX = 100;
        m_PosY = 100;
        m_PosX2 = -200;
        m_PosY2 = -200;


        auto q0 = VertexBuffer::CreateQuad(100.0f, 100.0f, 200.0f);
        auto q1 = VertexBuffer::CreateQuad(-200.0f, -200.0f, 200.0f);

        memcpy(m_Vertices, q0.data(), q0.size() * sizeof(Vertex));
        memcpy(m_Vertices + q0.size(), q1.data(), q1.size() * sizeof(Vertex));


        translationC = glm::vec3(0, 0, 0);
        model = glm::translate(glm::mat4(1.0f), translationA); 


        std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec4 a_Position;
			layout(location = 1) in vec4 a_Color;
            
			uniform mat4 u_ViewProjection;
            uniform mat4 u_Transform;

            out vec4 v_Color;

			void main()
			{
                v_Color = a_Color;
				gl_Position = u_ViewProjection * u_Transform * a_Position;	
            }
		)";

        std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;
            
            in vec4 v_Color;
                        
			void main()
			{
            	color = v_Color;
			}
		)";

        m_Shader = new Shader(vertexSrc, fragmentSrc);
        m_VertexBuffer = new VertexBuffer(m_Vertices, sizeof(m_Vertices));
        m_IndexBuffer = new IndexBuffer(indices, 12);


        m_VertexBuffer->Bind();
        m_Shader->Bind();
        m_IndexBuffer->Bind();

		//m_Layout.Push<float>(2);
		//m_Layout.Push<float>(4);
		//m_VertexArray.Addbuffer(*m_VertexBuffer, m_Layout);
		m_VertexArray.AddVertexBuffer(*m_VertexBuffer);


	}

	EditorLayer::~EditorLayer()
	{
        delete m_Shader;
        delete m_VertexBuffer;
        delete m_IndexBuffer;
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
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(m_Vertices), m_Vertices);

        auto q0 = VertexBuffer::CreateQuad(m_PosX, m_PosY, 200.0f);
        auto q1 = VertexBuffer::CreateQuad(m_PosX2, m_PosY2, 200.0f);

        memcpy(m_Vertices, q0.data(), q0.size() * sizeof(Vertex));
        memcpy(m_Vertices + q0.size(), q1.data(), q1.size() * sizeof(Vertex));
       
        m_Vertices[0].Color[0] = w;
        m_Vertices[1].Color[0] = w;
        m_Vertices[2].Color[0] = w;
        m_Vertices[3].Color[0] = w;


        m_Vertices[4].Color[2] = w;
        m_Vertices[5].Color[2] = w;
        m_Vertices[6].Color[2] = w;
        m_Vertices[7].Color[2] = w;

        model = glm::translate(glm::mat4(1.0f), translationC); // "Models" pos
           

        Renderer::BeginScene(m_CameraController.GetCamera());

        Renderer::Draw(m_VertexArray, *m_IndexBuffer, *m_Shader, model);
        

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
