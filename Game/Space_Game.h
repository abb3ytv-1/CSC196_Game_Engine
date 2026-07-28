#pragma once
#include "Game.h"

class SpaceGame : public nu::Game {
public:
	enum class GameState {
		Title,
		StartGame,
		StartLevel,
		Game,
		GameOver
	};

public:
	SpaceGame() = default;
	SpaceGame(Scene* scene) : 
		nu::Game(scene){}

	bool Initialize() override;

	void Update(float dt) override;
	void Draw(const class Renderer& renderer) override;

private:
	int a_score{ 0 };
	int a_lives{ 0 };

	GameState a_gameState = GameState::Title;
};
