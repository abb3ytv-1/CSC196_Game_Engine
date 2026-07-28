#include "pch.h"
#include "Game.h"
#include "Scene.h"

namespace nu {
	void Game::Update(float dt) {
		a_scene->Update(dt);
	}

	void Game::Draw(const Renderer& renderer) {
		a_scene->Draw(renderer)
	}
}