#pragma once

namespace RealEngine {

	enum class EventType
	{
		None = 0,
		WindowResized,
		KeyPressed,
		KeyReleased,
		KeyRepeated,
		MouseButtonPressed,
		MouseButtonReleased,
		MouseButtonRepeated,
		MouseScrolled,
		MouseMoved
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

	class KeyReleased
	{
	public:
		KeyReleased() {};
		~KeyReleased() {};

		int Key;
		int ScanCode;
		int Action;
		int Mods;
	};

	class MouseButtonPressed
	{
	public:
		MouseButtonPressed() {};
		~MouseButtonPressed() {};

		int Button = -1;
		int Action;
		int Mods;
	};

	class MouseButtonReleased
	{
	public:
		MouseButtonReleased() {};
		~MouseButtonReleased() {};

		int Button;
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

	class MouseMoved
	{
	public:
		MouseMoved() {};
		~MouseMoved() {};

		int xOffset;
		int yOffset;
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
			KeyReleased KeyReleased;
			MouseButtonPressed MouseButtonPressed;
			MouseButtonReleased MouseButtonReleased;
			MouseScrolled MouseScrolled;
			MouseMoved MouseMoved;
		};
	private:
	};
}
