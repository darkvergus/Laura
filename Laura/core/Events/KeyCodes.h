#ifndef KEYCODES_H
#define KEYCODES_H

#include <imgui.h>

namespace Laura {

	using KeyCode = uint16_t;

	namespace Key
	{
		enum : KeyCode
		{
			// From glfw3.h
			SPACE = 32,
			APOSTROPHE = 39,	/* ' */
			COMMA = 44, /* , */
			MINUS = 45, /* - */
			PERIOD = 46, /* . */
			SLASH = 47, /* / */
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
			SEMICOLON = 59, /* ; */
			EQUAL = 61, /* = */
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
			LEFT_BRACKET = 91,  /* [ */
			BACKSLASH = 92,  /* \ */
			RIGHT_BRACKET = 93,  /* ] */
			GRAVE_ACCENT = 96,  /* ` */
			WORLD1 = 161, /* non-US #1 */
			WORLD2 = 162, /* non-US #2 */
			/* Function keys */
			ESCAPE = 256,
			ENTER = 257,
			TAB = 258,
			BACKSPACE = 259,
			INSERT = 260,
			DELETE_KEY = 261,
			RIGHT = 262,
			LEFT = 263,
			DOWN = 264,
			UP = 265,
			PAGE_UP = 266,
			PAGE_DOWN = 267,
			HOME = 268,
			END = 269,
			CAPS_LOCK = 280,
			SCROLL_LOCK = 281,
			NUM_LOCK = 282,
			PRINT_SCREEN = 283,
			PAUSE = 284,
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
			KP_DECIMAL = 330,
			KP_DIVIDE = 331,
			KP_MULTIPLY = 332,
			KP_SUBTRACT = 333,
			KP_ADD = 334,
			KP_ENTER = 335,
			KP_EQUAL = 336,
			LEFT_SHIFT = 340,
			LEFT_CONTROL = 341,
			LEFT_ALT = 342,
			LEFT_SUPER = 343,
			RIGHT_SHIFT = 344,
			RIGHT_CONTROL = 345,
			RIGHT_ALT = 346,
			RIGHT_SUPER = 347,
			MENU = 348
		};
	}

	inline ImGuiKey KeyCodeToImGuiKey(KeyCode code)
	{
		switch (code)
		{
		case Key::TAB: return ImGuiKey_Tab;
		case Key::LEFT: return ImGuiKey_LeftArrow;
		case Key::RIGHT: return ImGuiKey_RightArrow;
		case Key::UP: return ImGuiKey_UpArrow;
		case Key::DOWN: return ImGuiKey_DownArrow;
		case Key::PAGE_UP: return ImGuiKey_PageUp;
		case Key::PAGE_DOWN: return ImGuiKey_PageDown;
		case Key::HOME: return ImGuiKey_Home;
		case Key::END: return ImGuiKey_End;
		case Key::INSERT: return ImGuiKey_Insert;
		case Key::DELETE_KEY: return ImGuiKey_Delete;
		case Key::BACKSPACE: return ImGuiKey_Backspace;
		case Key::SPACE: return ImGuiKey_Space;
		case Key::ENTER: return ImGuiKey_Enter;
		case Key::ESCAPE: return ImGuiKey_Escape;
		case Key::APOSTROPHE: return ImGuiKey_Apostrophe;
		case Key::COMMA: return ImGuiKey_Comma;
		case Key::MINUS: return ImGuiKey_Minus;
		case Key::PERIOD: return ImGuiKey_Period;
		case Key::SLASH: return ImGuiKey_Slash;
		case Key::SEMICOLON: return ImGuiKey_Semicolon;
		case Key::EQUAL: return ImGuiKey_Equal;
		case Key::LEFT_BRACKET: return ImGuiKey_LeftBracket;
		case Key::BACKSLASH: return ImGuiKey_Backslash;
		case Key::RIGHT_BRACKET: return ImGuiKey_RightBracket;
		case Key::GRAVE_ACCENT: return ImGuiKey_GraveAccent;
		case Key::CAPS_LOCK: return ImGuiKey_CapsLock;
		case Key::SCROLL_LOCK: return ImGuiKey_ScrollLock;
		case Key::NUM_LOCK: return ImGuiKey_NumLock;
		case Key::PRINT_SCREEN: return ImGuiKey_PrintScreen;
		case Key::PAUSE: return ImGuiKey_Pause;
		case Key::KP0: return ImGuiKey_Keypad0;
		case Key::KP1: return ImGuiKey_Keypad1;
		case Key::KP2: return ImGuiKey_Keypad2;
		case Key::KP3: return ImGuiKey_Keypad3;
		case Key::KP4: return ImGuiKey_Keypad4;
		case Key::KP5: return ImGuiKey_Keypad5;
		case Key::KP6: return ImGuiKey_Keypad6;
		case Key::KP7: return ImGuiKey_Keypad7;
		case Key::KP8: return ImGuiKey_Keypad8;
		case Key::KP9: return ImGuiKey_Keypad9;
		case Key::KP_DECIMAL: return ImGuiKey_KeypadDecimal;
		case Key::KP_DIVIDE: return ImGuiKey_KeypadDivide;
		case Key::KP_MULTIPLY: return ImGuiKey_KeypadMultiply;
		case Key::KP_SUBTRACT: return ImGuiKey_KeypadSubtract;
		case Key::KP_ADD: return ImGuiKey_KeypadAdd;
		case Key::KP_ENTER: return ImGuiKey_KeypadEnter;
		case Key::KP_EQUAL: return ImGuiKey_KeypadEqual;
		case Key::LEFT_SHIFT: return ImGuiKey_LeftShift;
		case Key::LEFT_CONTROL: return ImGuiKey_LeftCtrl;
		case Key::LEFT_ALT: return ImGuiKey_LeftAlt;
		case Key::LEFT_SUPER: return ImGuiKey_LeftSuper;
		case Key::RIGHT_SHIFT: return ImGuiKey_RightShift;
		case Key::RIGHT_CONTROL: return ImGuiKey_RightCtrl;
		case Key::RIGHT_ALT: return ImGuiKey_RightAlt;
		case Key::RIGHT_SUPER: return ImGuiKey_RightSuper;
		case Key::MENU: return ImGuiKey_Menu;
		case Key::D0: return ImGuiKey_0;
		case Key::D1: return ImGuiKey_1;
		case Key::D2: return ImGuiKey_2;
		case Key::D3: return ImGuiKey_3;
		case Key::D4: return ImGuiKey_4;
		case Key::D5: return ImGuiKey_5;
		case Key::D6: return ImGuiKey_6;
		case Key::D7: return ImGuiKey_7;
		case Key::D8: return ImGuiKey_8;
		case Key::D9: return ImGuiKey_9;
		case Key::A: return ImGuiKey_A;
		case Key::B: return ImGuiKey_B;
		case Key::C: return ImGuiKey_C;
		case Key::D: return ImGuiKey_D;
		case Key::E: return ImGuiKey_E;
		case Key::F: return ImGuiKey_F;
		case Key::G: return ImGuiKey_G;
		case Key::H: return ImGuiKey_H;
		case Key::I: return ImGuiKey_I;
		case Key::J: return ImGuiKey_J;
		case Key::K: return ImGuiKey_K;
		case Key::L: return ImGuiKey_L;
		case Key::M: return ImGuiKey_M;
		case Key::N: return ImGuiKey_N;
		case Key::O: return ImGuiKey_O;
		case Key::P: return ImGuiKey_P;
		case Key::Q: return ImGuiKey_Q;
		case Key::R: return ImGuiKey_R;
		case Key::S: return ImGuiKey_S;
		case Key::T: return ImGuiKey_T;
		case Key::U: return ImGuiKey_U;
		case Key::V: return ImGuiKey_V;
		case Key::W: return ImGuiKey_W;
		case Key::X: return ImGuiKey_X;
		case Key::Y: return ImGuiKey_Y;
		case Key::Z: return ImGuiKey_Z;
		case Key::F1: return ImGuiKey_F1;
		case Key::F2: return ImGuiKey_F2;
		case Key::F3: return ImGuiKey_F3;
		case Key::F4: return ImGuiKey_F4;
		case Key::F5: return ImGuiKey_F5;
		case Key::F6: return ImGuiKey_F6;
		case Key::F7: return ImGuiKey_F7;
		case Key::F8: return ImGuiKey_F8;
		case Key::F9: return ImGuiKey_F9;
		case Key::F10: return ImGuiKey_F10;
		case Key::F11: return ImGuiKey_F11;
		case Key::F12: return ImGuiKey_F12;
		case Key::F13: return ImGuiKey_F13;
		case Key::F14: return ImGuiKey_F14;
		case Key::F15: return ImGuiKey_F15;
		case Key::F16: return ImGuiKey_F16;
		case Key::F17: return ImGuiKey_F17;
		case Key::F18: return ImGuiKey_F18;
		case Key::F19: return ImGuiKey_F19;
		case Key::F20: return ImGuiKey_F20;
		case Key::F21: return ImGuiKey_F21;
		case Key::F22: return ImGuiKey_F22;
		case Key::F23: return ImGuiKey_F23;
		case Key::F24: return ImGuiKey_F24;
		default: return ImGuiKey_None;
		}
	}

}

#endif // KEYCODES_H