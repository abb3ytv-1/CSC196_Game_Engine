#include "Space_Game.h"
#include "Engine.h"
#include "Scene.h"


using namespace nu;

bool SpaceGame::Initialize(){
    Game::Initialize();

    a_scene = new nu::Scene();

    return true;
}

void SpaceGame::Update(float dt){
    switch (a_gameState)
    {
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

    Game::Update(dt);
}

void SpaceGame::Draw(const nu::Renderer& renderer){
    switch (a_gameState)
    {
    case GameState::Title:
        // Draw title
        break;
    case GameState::StartGame:
    case GameState::StartLevel:
    case GameState::Game:
        // Draw score - lives
        break;
    case GameState::GameOver:
        // draw gameover
        break;
    default:
        break;
    }

    Game::Draw(renderer);
}
