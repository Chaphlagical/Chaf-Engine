#pragma once

#include <Engine/log.h>
#include <Engine/core.h>

namespace Chaf
{
	/*KeyCode*/
	typedef enum class KeyCode : uint16_t
	{
		// From glfw3.h
		Space = 32,
		Apostrophe = 39, /* ' */
		Comma = 44, /* , */
		Minus = 45, /* - */
		Period = 46, /* . */
		Slash = 47, /* / */

		D0 = 48, /* 0 */
		D1 = 49, /* 1 */
		D2 = 50, /* 2 */
		D3 = 51, /* 3 */
		D4 = 52, /* 4 */
		D5 = 53, /* 5 */
		D6 = 54, /* 6 */
		D7 = 55, /* 7 */
		D8 = 56, /* 8 */
		D9 = 57, /* 9 */

		Semicolon = 59, /* ; */
		Equal = 61, /* = */

		A = 65,
		B = 66,
		C = 67,
		D = 68,
		E = 69,
		F = 70,
		G = 71,
		H = 72,
		I = 73,
		J = 74,
		K = 75,
		L = 76,
		M = 77,
		N = 78,
		O = 79,
		P = 80,
		Q = 81,
		R = 82,
		S = 83,
		T = 84,
		U = 85,
		V = 86,
		W = 87,
		X = 88,
		Y = 89,
		Z = 90,

		LeftBracket = 91,  /* [ */
		Backslash = 92,  /* \ */
		RightBracket = 93,  /* ] */
		GraveAccent = 96,  /* ` */

		World1 = 161, /* non-US #1 */
		World2 = 162, /* non-US #2 */

		/* Function keys */
		Escape = 256,
		Enter = 257,
		Tab = 258,
		Backspace = 259,
		Insert = 260,
		Delete = 261,
		Right = 262,
		Left = 263,
		Down = 264,
		Up = 265,
		PageUp = 266,
		PageDown = 267,
		Home = 268,
		End = 269,
		CapsLock = 280,
		ScrollLock = 281,
		NumLock = 282,
		PrintScreen = 283,
		Pause = 284,
		F1 = 290,
		F2 = 291,
		F3 = 292,
		F4 = 293,
		F5 = 294,
		F6 = 295,
		F7 = 296,
		F8 = 297,
		F9 = 298,
		F10 = 299,
		F11 = 300,
		F12 = 301,
		F13 = 302,
		F14 = 303,
		F15 = 304,
		F16 = 305,
		F17 = 306,
		F18 = 307,
		F19 = 308,
		F20 = 309,
		F21 = 310,
		F22 = 311,
		F23 = 312,
		F24 = 313,
		F25 = 314,

		/* Keypad */
		KP0 = 320,
		KP1 = 321,
		KP2 = 322,
		KP3 = 323,
		KP4 = 324,
		KP5 = 325,
		KP6 = 326,
		KP7 = 327,
		KP8 = 328,
		KP9 = 329,
		KPDecimal = 330,
		KPDivide = 331,
		KPMultiply = 332,
		KPSubtract = 333,
		KPAdd = 334,
		KPEnter = 335,
		KPEqual = 336,

		LeftShift = 340,
		LeftControl = 341,
		LeftAlt = 342,
		LeftSuper = 343,
		RightShift = 344,
		RightControl = 345,
		RightAlt = 346,
		RightSuper = 347,
		Menu = 348
	} Key;

	inline std::ostream& operator<<(std::ostream& os, KeyCode keyCode)
	{
		os << static_cast<int32_t>(keyCode);
		return os;
	}

// From glfw3.h
#define CHAF_KEY_SPACE					::Chaf::Key::Space
#define CHAF_KEY_APOSTROPHE      ::Chaf::Key::Apostrophe    /* ' */
#define CHAF_KEY_COMMA				::Chaf::Key::Comma         /* , */
#define CHAF_KEY_MINUS					::Chaf::Key::Minus         /* - */
#define CHAF_KEY_PERIOD					::Chaf::Key::Period        /* . */
#define CHAF_KEY_SLASH					::Chaf::Key::Slash         /* / */
#define CHAF_KEY_0							::Chaf::Key::D0
#define CHAF_KEY_1							::Chaf::Key::D1
#define CHAF_KEY_2							::Chaf::Key::D2
#define CHAF_KEY_3							::Chaf::Key::D3
#define CHAF_KEY_4							::Chaf::Key::D4
#define CHAF_KEY_5							::Chaf::Key::D5
#define CHAF_KEY_6							::Chaf::Key::D6
#define CHAF_KEY_7							::Chaf::Key::D7
#define CHAF_KEY_8							::Chaf::Key::D8
#define CHAF_KEY_9							::Chaf::Key::D9
#define CHAF_KEY_SEMICOLON			::Chaf::Key::Semicolon     /* ; */
#define CHAF_KEY_EQUAL					::Chaf::Key::Equal         /* = */
#define CHAF_KEY_A							::Chaf::Key::A
#define CHAF_KEY_B							::Chaf::Key::B
#define CHAF_KEY_C							::Chaf::Key::C
#define CHAF_KEY_D							::Chaf::Key::D
#define CHAF_KEY_E							::Chaf::Key::E
#define CHAF_KEY_F							::Chaf::Key::F
#define CHAF_KEY_G							::Chaf::Key::G
#define CHAF_KEY_H							::Chaf::Key::H
#define CHAF_KEY_I							::Chaf::Key::I
#define CHAF_KEY_J							::Chaf::Key::J
#define CHAF_KEY_K							::Chaf::Key::K
#define CHAF_KEY_L							::Chaf::Key::L
#define CHAF_KEY_M							::Chaf::Key::M
#define CHAF_KEY_N							::Chaf::Key::N
#define CHAF_KEY_O							::Chaf::Key::O
#define CHAF_KEY_P							::Chaf::Key::P
#define CHAF_KEY_Q							::Chaf::Key::Q
#define CHAF_KEY_R							::Chaf::Key::R
#define CHAF_KEY_S							::Chaf::Key::S
#define CHAF_KEY_T							::Chaf::Key::T
#define CHAF_KEY_U							::Chaf::Key::U
#define CHAF_KEY_V							::Chaf::Key::V
#define CHAF_KEY_W							::Chaf::Key::W
#define CHAF_KEY_X							::Chaf::Key::X
#define CHAF_KEY_Y							::Chaf::Key::Y
#define CHAF_KEY_Z							::Chaf::Key::Z
#define CHAF_KEY_LEFT_BRACKET		::Chaf::Key::LeftBracket   /* [ */
#define CHAF_KEY_BACKSLASH			::Chaf::Key::Backslash     /* \ */
#define CHAF_KEY_RIGHT_BRACKET  ::Chaf::Key::RightBracket  /* ] */
#define CHAF_KEY_GRAVE_ACCENT	::Chaf::Key::GraveAccent   /* ` */
#define CHAF_KEY_WORLD_1				::Chaf::Key::World1        /* non-US #1 */
#define CHAF_KEY_WORLD_2				::Chaf::Key::World2        /* non-US #2 */

/* Function keys */
#define CHAF_KEY_ESCAPE					::Chaf::Key::Escape
#define CHAF_KEY_ENTER					::Chaf::Key::Enter
#define CHAF_KEY_TAB						::Chaf::Key::Tab
#define CHAF_KEY_BACKSPACE			::Chaf::Key::Backspace
#define CHAF_KEY_INSERT					::Chaf::Key::Insert
#define CHAF_KEY_DELETE					::Chaf::Key::Delete
#define CHAF_KEY_RIGHT					::Chaf::Key::Right
#define CHAF_KEY_LEFT						::Chaf::Key::Left
#define CHAF_KEY_DOWN					::Chaf::Key::Down
#define CHAF_KEY_UP						::Chaf::Key::Up
#define CHAF_KEY_PAGE_UP				::Chaf::Key::PageUp
#define CHAF_KEY_PAGE_DOWN       ::Chaf::Key::PageDown
#define CHAF_KEY_HOME					::Chaf::Key::Home
#define CHAF_KEY_END						::Chaf::Key::End
#define CHAF_KEY_CAPS_LOCK			::Chaf::Key::CapsLock
#define CHAF_KEY_SCROLL_LOCK		::Chaf::Key::ScrollLock
#define CHAF_KEY_NUM_LOCK			::Chaf::Key::NumLock
#define CHAF_KEY_PRINT_SCREEN		::Chaf::Key::PrintScreen
#define CHAF_KEY_PAUSE					::Chaf::Key::Pause
#define CHAF_KEY_F1							::Chaf::Key::F1
#define CHAF_KEY_F2							::Chaf::Key::F2
#define CHAF_KEY_F3							::Chaf::Key::F3
#define CHAF_KEY_F4							::Chaf::Key::F4
#define CHAF_KEY_F5							::Chaf::Key::F5
#define CHAF_KEY_F6							::Chaf::Key::F6
#define CHAF_KEY_F7							::Chaf::Key::F7
#define CHAF_KEY_F8							::Chaf::Key::F8
#define CHAF_KEY_F9							::Chaf::Key::F9
#define CHAF_KEY_F10						::Chaf::Key::F10
#define CHAF_KEY_F11						::Chaf::Key::F11
#define CHAF_KEY_F12						::Chaf::Key::F12
#define CHAF_KEY_F13						::Chaf::Key::F13
#define CHAF_KEY_F14						::Chaf::Key::F14
#define CHAF_KEY_F15						::Chaf::Key::F15
#define CHAF_KEY_F16						::Chaf::Key::F16
#define CHAF_KEY_F17						::Chaf::Key::F17
#define CHAF_KEY_F18						::Chaf::Key::F18
#define CHAF_KEY_F19						::Chaf::Key::F19
#define CHAF_KEY_F20						::Chaf::Key::F20
#define CHAF_KEY_F21						::Chaf::Key::F21
#define CHAF_KEY_F22						::Chaf::Key::F22
#define CHAF_KEY_F23						::Chaf::Key::F23
#define CHAF_KEY_F24						::Chaf::Key::F24
#define CHAF_KEY_F25						::Chaf::Key::F25

/* Keypad */
#define CHAF_KEY_KP_0						::Chaf::Key::KP0
#define CHAF_KEY_KP_1						::Chaf::Key::KP1
#define CHAF_KEY_KP_2						::Chaf::Key::KP2
#define CHAF_KEY_KP_3						::Chaf::Key::KP3
#define CHAF_KEY_KP_4						::Chaf::Key::KP4
#define CHAF_KEY_KP_5						::Chaf::Key::KP5
#define CHAF_KEY_KP_6						::Chaf::Key::KP6
#define CHAF_KEY_KP_7						::Chaf::Key::KP7
#define CHAF_KEY_KP_8						::Chaf::Key::KP8
#define CHAF_KEY_KP_9						::Chaf::Key::KP9
#define CHAF_KEY_KP_DECIMAL		::Chaf::Key::KPDecimal
#define CHAF_KEY_KP_DIVIDE			::Chaf::Key::KPDivide
#define CHAF_KEY_KP_MULTIPLY		::Chaf::Key::KPMultiply
#define CHAF_KEY_KP_SUBTRACT		::Chaf::Key::KPSubtract
#define CHAF_KEY_KP_ADD				::Chaf::Key::KPAdd
#define CHAF_KEY_KP_ENTER				::Chaf::Key::KPEnter
#define CHAF_KEY_KP_EQUAL			::Chaf::Key::KPEqual

#define CHAF_KEY_LEFT_SHIFT			::Chaf::Key::LeftShift
#define CHAF_KEY_LEFT_CONTROL	::Chaf::Key::LeftControl
#define CHAF_KEY_LEFT_ALT				::Chaf::Key::LeftAlt
#define CHAF_KEY_LEFT_SUPER			::Chaf::Key::LeftSuper
#define CHAF_KEY_RIGHT_SHIFT		::Chaf::Key::RightShift
#define CHAF_KEY_RIGHT_CONTROL	::Chaf::Key::RightControl
#define CHAF_KEY_RIGHT_ALT			::Chaf::Key::RightAlt
#define CHAF_KEY_RIGHT_SUPER		::Chaf::Key::RightSuper
#define CHAF_KEY_MENU					::Chaf::Key::Menu

////////////////////////////////////////////////////////////////////////////

/*Mouse Code*/
typedef enum class MouseCode : uint16_t
{
	// From glfw3.h
	Button0 = 0,
	Button1 = 1,
	Button2 = 2,
	Button3 = 3,
	Button4 = 4,
	Button5 = 5,
	Button6 = 6,
	Button7 = 7,

	ButtonLast = Button7,
	ButtonLeft = Button0,
	ButtonRight = Button1,
	ButtonMiddle = Button2
} Mouse;

inline std::ostream& operator<<(std::ostream& os, MouseCode mouseCode)
{
	os << static_cast<int32_t>(mouseCode);
	return os;
}

#define CHAF_MOUSE_BUTTON_0				::Chaf::Mouse::Button0
#define CHAF_MOUSE_BUTTON_1				::Chaf::Mouse::Button1
#define CHAF_MOUSE_BUTTON_2				::Chaf::Mouse::Button2
#define CHAF_MOUSE_BUTTON_3				::Chaf::Mouse::Button3
#define CHAF_MOUSE_BUTTON_4				::Chaf::Mouse::Button4
#define CHAF_MOUSE_BUTTON_5				::Chaf::Mouse::Button5
#define CHAF_MOUSE_BUTTON_6				::Chaf::Mouse::Button6
#define CHAF_MOUSE_BUTTON_7				::Chaf::Mouse::Button7
#define CHAF_MOUSE_BUTTON_LAST		::Chaf::Mouse::ButtonLast
#define CHAF_MOUSE_BUTTON_LEFT		::Chaf::Mouse::ButtonLeft
#define CHAF_MOUSE_BUTTON_RIGHT		::Chaf::Mouse::ButtonRight
#define CHAF_MOUSE_BUTTON_MIDDLE	::Chaf::Mouse::ButtonMiddle

////////////////////////////////////////////////////////////////////////////

/*Input Class*/

	class CHAF_API Input
	{
	public:
		template <typename T>
		static bool IsKeyPressed(T* window, KeyCode keycode);
		template <typename T>
		static bool IsMouseButtonPressed(T* window, MouseCode button);
		template <typename T>
		static std::pair<float, float> GetMousePosition(T* window);
		template <typename T>
		static float GetMouseX(T* window);
		template <typename T>
		static float GetMouseY(T* window);
		template <typename T>
		static void SetCursorHidden(T* window, const bool enable);
	};
}