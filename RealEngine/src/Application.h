#pragma once

#include "Engine/Graphics/Window.h"

#include "Engine/Layers/LayersStack.h"
#include "Engine/Layers/Layer.h"

#include "Engine/Editor/EditorLayer.h"
#include "Engine/Layers/ExampleLayer.h"
#include "Engine/Layers/ImGuiLayer.h"

#include "Engine/Events/Event.h"

#include "Engine/TimeStep.h"

namespace RealEngine {

	class Application
	{
	public:
		
		~Application();

		void Run();
		void OnUpdate();
		void OnEvent(Event& event);
		void OnRender();

		void PushLayer(Layer* layer)
		{
			m_LayerStack.PushLayer(layer);
		}
		
		void Exit();

		inline Window& GetWindow() { return *m_Window; }
		inline ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }

		static Application& Get() { return *s_Instance; }

	private:
		Application();
	private:
		static Application* s_Instance;

		Window* m_Window;

		float m_LastFrameTime;
		Timestep m_TimeStep;

		LayersStack m_LayerStack;
		ImGuiLayer* m_ImGuiLayer;

		bool m_Running;
		bool m_Minimizied = false;
		
	};

}
