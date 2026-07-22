#pragma once

#include "Input.h"
#include "Renderer.h"
#include "Random.h"
#include "Vector2.h"
#include "Vector3.h"
#include "GameTime.h"
#include "MathUtils.h"
#include "Transform.h"
#include "Actor.h"
#include "AudioSystem.h"

#include <SDL3/SDL.h>
#include <iostream>
#include <vector>


namespace nu {
	class Engine {
	public:
		Engine() = default;

		bool Initialize();
		void Shutdown();
		void Update();

		Input& GetInput() { return a_input; }
		Renderer& GetRenderer() { return a_renderer; }
		Time& GetTime() { return a_time; }

		AudioSystem& GetAudio() { return a_audio; }

	private:
		Input a_input;
		Renderer a_renderer;
		Time a_time;
		AudioSystem a_audio;
	};

	extern Engine engine;
}