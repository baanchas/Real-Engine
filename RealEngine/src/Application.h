#pragma once

#include "Engine/Graphics/Window.h"
#include "Engine/Layers/LayersStack.h"
#include "Engine/Layers/Layer.h"
#include "Engine/Layers/EditorLayer.h"
#include "Engine/Layers/ExampleLayer.h"
#include "Engine/Layers/ImGuiLayer.h"


namespace RealEngine {

	class Application
	{
	public:
		
		~Application();

		void Run();
		void OnUpdate();
		void OnEvent();
		void OnRender();

		void PushLayer(Layer* layer)
		{
			m_LayerStack.PushLayer(layer);
		}
		
		void Exit();

		inline Window& GetWindow() { return *m_Window; }

		static Application& Get() { return *s_Instance; }
	private:
		Application();
	private:
		static Application* s_Instance;

		Window* m_Window;

		LayersStack m_LayerStack;
		ImGuiLayer* m_ImGuiLayer;

		bool m_Running;
		
	};

}
