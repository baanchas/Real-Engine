#pragma once

namespace RealEngine {

	struct WindowProps
	{
		std::string Title;
		unsigned int Width;
		unsigned int Height;
		bool VSync;

		WindowProps(
			const std::string& title = "Real Engine",
			unsigned int width = 1280,
			unsigned int height = 720,
			bool VSync = true
		)
			: Title(title), Width(width), Height(height), VSync(VSync)
		{
		}
	};

	class Window
	{
	public:
		Window();
		~Window();

		void OnUpdate();
		
		void Init(const WindowProps& props);
		
		void SetVSync(bool enabled);

		inline unsigned int GetWidth() const { return m_Properties.Width; }
		inline unsigned int GetHeight() const { return m_Properties.Height; }

		inline GLFWwindow* GetNativeWindow() const { return m_Window; }
				
	private:
		GLFWwindow* m_Window;
		WindowProps props;

		struct WindowProperties
		{
			std::string Title;
			unsigned int Width;
			unsigned int Height;
			bool VSync;
		};

		WindowProperties m_Properties;



		bool VSync;

	};

}