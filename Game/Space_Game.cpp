#include "Space_Game.h"

#include "Renderer.h"
#include "Scene.h"

bool SpaceGame::Initialize() {
	if (!nu::Game::Initialize()) {
		return false;
	}

	// Create a Scene only when one was not provided.
	if (a_scene == nullptr) {
		a_scene = new nu::Scene();
		a_ownsScene = true;
	}

	return true;
}

void SpaceGame::Shutdown() {
	if (a_ownsScene) {
		delete a_scene;
		a_scene = nullptr;
		a_ownsScene = false;
	}

	nu::Game::Shutdown();
}

void SpaceGame::Update(float dt) {
	switch (a_gameState) {
	case GameState::Title:
		break;

	case GameState::StartGame:
		break;

	case GameState::StartLevel:
		break;

	case GameState::Game:
		break;

	case GameState::GameOver:
		break;

	default:
		break;
	}

	nu::Game::Update(dt);
}

void SpaceGame::Draw(
	const nu::Renderer& renderer
) {
	switch (a_gameState) {
	case GameState::Title:
		// Draw title
		break;

	case GameState::StartGame:
		break;

	case GameState::StartLevel:
		break;

	case GameState::Game:
		// Draw score and lives
		break;

	case GameState::GameOver:
		// Draw game-over screen
		break;

	default:
		break;
	}

	nu::Game::Draw(renderer);
}