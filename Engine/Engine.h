#pragma once

#include "Audio.h"
#include "GameTime.h"
#include "Input.h"
#include "Renderer.h"

namespace nu {
	class Engine {
	public:
		Engine() = default;

		bool Initialize();
		void Shutdown();
		void Update();

		Input& GetInput() {
			return a_input;
		}

		Renderer& GetRenderer() {
			return a_renderer;
		}

		Time& GetTime() {
			return a_time;
		}

		Audio& GetAudio() {
			return a_audio;
		}

	private:
		Input a_input;
		Renderer a_renderer;
		Time a_time;
		Audio a_audio;
	};

	extern Engine engine;
}