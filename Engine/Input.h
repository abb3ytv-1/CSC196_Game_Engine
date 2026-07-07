#pragma once
#include "Vector2.h"
#include <vector>

namespace nu {
	class Input {
	public:
		enum MouseButton{
			Left = 1,
			Middle = 2,
			Right = 3
		};

	public:
		bool Initialize();
		void Shutdown();

		void Update();

		bool GetKeyDown(int key) const {return a_keyStates[key];}
		bool GetPrevKeyDown(int key) const {return a_prevKeyStates[key];}
		bool GetKeyPress(int key) const {return !a_prevKeyStates[key] && a_keyStates[key];}
		bool GetKeyReleased(int key) const {return a_prevKeyStates[key] && !a_keyStates[key];}

		bool GetMouseDown(MouseButton button) const { return false; }

		Vector2 GetMousePotiion() { return a_mousePostion; }

	private:
		// keyboard
		std::vector<bool> a_keyStates;
		std::vector<bool> a_prevKeyStates;

		// mouse
		uint32_t a_buttonStates = 0;
		uint32_t a_prevButtonStates = 0;

		nu::Vector2 a_mousePostion{ 0, 0 };
	};
}