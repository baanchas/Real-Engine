#pragma once

namespace RealEngine {

	enum class EventType
	{
		None = 0,
		WindowResized,
		KeyPressed,
		KeyReleased,
		MouseButtonPressed,
		MouseButtonReleased,
		MouseScrolled
	};

	class WindowResized
	{
	public:
		WindowResized() {};
		~WindowResized() {};

		unsigned int Width = 0;
		unsigned int Height = 0;
	};

	class KeyPressed
	{
	public:
		KeyPressed() {};
		~KeyPressed() {};

		int Key;
		int ScanCode;
		int Action;
		int Mods;
	};

	class MouseScrolled
	{
	public:
		MouseScrolled() {};
		~MouseScrolled() {};

		int xOffset = 0;
		int yOffset = 0;
	};


	class Event
	{
	public:
		Event() {};
		~Event() {};

		EventType Type = EventType::None;

		union 
		{
			WindowResized WindowResized;
			KeyPressed KeyPressed;
			MouseScrolled MouseScrolled;
		};
	private:
	};
}
