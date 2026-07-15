#include "pch.h"
#include "Engine.h"
#include "framework.h"
#include <iostream>

namespace nu {
	Engine engine;

	bool Engine::Initialize(){
		a_renderer.Initialize("Game Engine", 1920, 1080);
		a_input.Initialize();

	}

	void Engine::Shutdown() {
		a_renderer.Shutdown();
	}
		
	void nu::Engine::Update() {
		a_input.GetInput();
		a_time.GetTime();
	}
}