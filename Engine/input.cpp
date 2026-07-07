#include "pch.h"
#include "Input.h"

#include<SDL3/SDL.h>

namespace nu {
	bool Input::Initialize() {
		int numKeys;
		const bool* keyState = SDL_GetKeyboardState(&numKeys);
		a_keyStates.resize(numKeys);

		std::copy(keyState, keyState + numKeys, a_keyStates.begin());
		a_prevKeyStates = a_keyStates;




		return true;
	}
	void Input::Shutdown() {
		//
	}

	void Input::Update() {
		a_prevKeyStates = a_keyStates;
		const bool* keyState = SDL_GetKeyboardState(NULL);
		std::copy(keyState, keyState + a_keyStates.size(), a_keyStates.begin());
		

		a_mousePostion = SDL_GetMouseState(&a_mousePostion.x, &a_mousePostion.y);
	}
}