#pragma once

namespace KeyCodes {

	enum Keyboard
	{
		SPACE = 32,
		APOSTROPHE = 39, /* ' */
		COMMA,			 /* , */
		MINUS,
		PERIOD,			 /* . */
		SLASH,			 /* / */
		NUM0,
		NUM1,
		NUM2,
		NUM3,
		NUM4,
		NUM5,
		NUM6,
		NUM7,
		NUM8,
		NUM9,
		SEMICOLON = 59,			/* ; */
		EQUAL = 61,				/* = */
		A = 65,
		B,
		C,
		D,
		E,
		F,
		G,
		H,
		I,
		J,
		K,
		L,
		M,
		N,
		O,
		P,
		Q,
		R,
		S,
		T,
		U,
		V,
		W,
		X,
		Y,
		Z,
		LEFT_BRACKET,		/* [ */
		BACKSLASH,			/* \ */
		RIGHT_BRACKET,		/* ] */
		GRAVE_ACCENT = 96,		/* ` */
		WORLD_1 = 161,			/* non-US #1 */
		WORLD_2 = 162,			/* non-US #2 */

		/* Function keys */
		ESCAPE = 256,
		ENTER,
		TAB,
		BACKSPACE,
		INSERT,
		DEL,
		RIGHT,
		LEFT,
		DOWN,
		UP,
		PAGE_UP,
		PAGE_DOWN,
		HOME,
		END,
		CAPS_LOCK = 280,
		SCROLL_LOCK,
		NUM_LOCK,
		PRINT_SCREEN,
		PAUSE,
		F1 = 290,
		F2,
		F3,
		F4,
		F5,
		F6,
		F7,
		F8,
		F9,
		F10,
		F11,
		F12,
		F13,
		F14,
		F15,
		F16,
		F17,
		F18,
		F19,
		F20,
		F21,
		F22,
		F23,
		F24,
		F25,
		KP_0 = 320,
		KP_1,
		KP_2,
		KP_3,
		KP_4,
		KP_5,
		KP_6,
		KP_7,
		KP_8,
		KP_9,
		KP_DECIMAL,
		KP_DIVIDE,
		KP_MULTIPLY,
		KP_SUBTRACT,
		KP_ADD,
		KP_ENTER,
		KP_EQUAL,
		LEFT_SHIFT = 340,
		LEFT_CONTROL,
		LEFT_ALT,
		LEFT_SUPER,
		RIGHT_SHIFT,
		RIGHT_CONTROL,
		RIGHT_ALT,
		RIGHT_SUPER,
		MENU
	};

	#define GLFW_MOD_SHIFT           0x0001
	/*! @brief If this bit is set one or more Control keys were held down.
	 *
	 *  If this bit is set one or more Control keys were held down.
	 */
	#define GLFW_MOD_CONTROL         0x0002
	 /*! @brief If this bit is set one or more Alt keys were held down.
	  *
	  *  If this bit is set one or more Alt keys were held down.
	  */
	#define GLFW_MOD_ALT             0x0004
	  /*! @brief If this bit is set one or more Super keys were held down.
	   *
	   *  If this bit is set one or more Super keys were held down.
	   */
	#define GLFW_MOD_SUPER           0x0008
	   /*! @brief If this bit is set the Caps Lock key is enabled.
		*
		*  If this bit is set the Caps Lock key is enabled and the @ref
		*  GLFW_LOCK_KEY_MODS input mode is set.
		*/
	#define GLFW_MOD_CAPS_LOCK       0x0010
		/*! @brief If this bit is set the Num Lock key is enabled.
		 *
		 *  If this bit is set the Num Lock key is enabled and the @ref
		 *  GLFW_LOCK_KEY_MODS input mode is set.
		 */
	#define GLFW_MOD_NUM_LOCK        0x0020

	enum Mouse
	{
			 /*! @} */

			 /*! @defgroup buttons Mouse buttons
			  *  @brief Mouse button IDs.
			  *
			  *  See [mouse button input](@ref input_mouse_button) for how these are used.
			  *
			  *  @ingroup input
			  *  @{ */
		MOUSE_LEFT = 0,
		MOUSE_RIGHT,
		MOUSE_MIDDLE,
		BUTTON_4,
		BUTTON_5,
		BUTTON_6,
		BUTTON_7,
		BUTTTON_8,
	};
}