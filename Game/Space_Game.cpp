#include "Space_Game.h"

#include "../Engine/Enemy.h"
#include "../Engine/Engine.h"
#include "../Engine/File.h"
#include "../Engine/MathUtils.h"

#include "Assets.h"
#include "Bullet.h"
#include "Player.h"

#include <SDL3/SDL.h>

#include <memory>
#include <utility>

using namespace nu;

SpaceGame::SpaceGame() :
	a_text{ &a_font }
{}

int SpaceGame::Run() {
	if (!Initialize()) { return 1; }

	Renderer& renderer = engine.GetRenderer();

	while (!a_quit) {
		ProcessEvents();

		if (a_quit) { break; }

		engine.Update();

		float dt = engine.GetTime().GetDeltaTime();

		Update(dt);
		Draw(renderer);
	}

	Shutdown();

	return 0;
}

bool SpaceGame::Initialize() {
	if (!Game::Initialize()) { return false; }

	a_scene = &a_gameScene;

	if (!SetWorkingDirectory("Assets")) { return false; }

	if (!a_font.Load( "Fonts/New Moon.ttf", 20.0f )) { return false; }

	if (!a_text.Create( engine.GetRenderer(), "Hello World", Color{ 1.0f, 1.0f, 1.0f } )) { return false; }

	if (!LoadAudio()) { return false; }

	a_playerModel = CreatePlayerModel();
	a_enemyModel = CreateEnemyModel();
	a_bulletModel = CreateBulletModel();

	CreateActors();

	return true;
}

bool SpaceGame::LoadAudio() {
	Audio& audio = engine.GetAudio();

	bool loaded = true;

	loaded &= audio.AddSound( "bass", "bass.wav" );

	loaded &= audio.AddSound( "snare", "snare.wav" );

	loaded &= audio.AddSound( "clap", "clap.wav" );

	loaded &= audio.AddSound( "open-hat", "open-hat.wav" );

	loaded &= audio.AddSound( "cowbell", "cowbell.wav" );

	return loaded;
}

void SpaceGame::CreateActors() {
	auto playerActor = std::make_unique<Player>( Transform{ Vector2{ 960.0f, 540.0f }, 0.0f, 10.0f }, a_playerModel, 300.0f );

	a_player = playerActor.get();

	a_player->SetCollisionRadius(8.0f);

	a_gameScene.AddActor( std::move(playerActor) );

	AddEnemy( Vector2{ 200.0f, 200.0f }, 100.0f );

	AddEnemy( Vector2{ 1700.0f, 300.0f }, 125.0f );

	AddEnemy( Vector2{ 400.0f, 900.0f }, 75.0f );
}

void SpaceGame::AddEnemy( const Vector2& position, float speed ) {
	if (a_player == nullptr) { return; }

	auto enemy = std::make_unique<Enemy>( Transform{ position, 0.0f, 8.0f }, a_enemyModel, speed );

	enemy->SetTarget(*a_player);

	a_gameScene.AddActor( std::move(enemy) );
}

void SpaceGame::ProcessEvents() {
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_EVENT_QUIT) {
			a_quit = true;
		}

		if ( event.type == SDL_EVENT_KEY_DOWN && event.key.scancode == SDL_SCANCODE_ESCAPE ) { a_quit = true; }
	}
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
		HandleAudioInput();
		HandlePlayerInput(dt);
		HandleMouseInput();

		Game::Update(dt);

		CheckCollisions();
		break;

	case GameState::GameOver:
		break;

	default:
		break;
	}
}

void SpaceGame::HandleAudioInput() {
	Input& input = engine.GetInput();

	Audio& audio = engine.GetAudio();

	if (input.GetKeyPress(SDL_SCANCODE_1)) {
		audio.PlaySound("bass");
	}

	if (input.GetKeyPress(SDL_SCANCODE_2)) {
		audio.PlaySound("snare");
	}

	if (input.GetKeyPress(SDL_SCANCODE_3)) {
		audio.PlaySound("clap");
	}

	if (input.GetKeyPress(SDL_SCANCODE_4)) {
		audio.PlaySound("open-hat");
	}

	if (input.GetKeyPress(SDL_SCANCODE_5)) {
		audio.PlaySound("cowbell");
	}
}

void SpaceGame::HandlePlayerInput(float dt) {
	if ( a_player == nullptr || a_player->IsDestroyed() ) { return; }

	Input& input = engine.GetInput();

	float rotation = a_player->GetTransform().rotation;

	if (input.GetKeyDown(SDL_SCANCODE_LEFT)) {
		rotation -= a_rotationSpeed * dt;
	}

	if (input.GetKeyDown(SDL_SCANCODE_RIGHT)) {
		rotation += a_rotationSpeed * dt;
	}

	a_player->SetRotation(rotation);

	Vector2 direction{ 0.0f, 0.0f };

	if (input.GetKeyDown(SDL_SCANCODE_W)) {
		direction.y -= 1.0f;
	}

	if (input.GetKeyDown(SDL_SCANCODE_S)) {
		direction.y += 1.0f;
	}

	if (input.GetKeyDown(SDL_SCANCODE_A)) {
		direction.x -= 1.0f;
	}

	if (input.GetKeyDown(SDL_SCANCODE_D)) {
		direction.x += 1.0f;
	}

	if (direction.LengthSqr() > 0.0f) {
		direction = direction.Normalized();
	}

	a_player->SetVelocity( direction * a_player->GetSpeed() );

	if (input.GetKeyPress(SDL_SCANCODE_SPACE)) {
		HandleShooting();
	}
}

void SpaceGame::HandleShooting() {
	if ( a_player == nullptr || a_player->IsDestroyed() ) { return; }

	float rotation = a_player->GetTransform().rotation;

	Vector2 forward{ 1.0f, 0.0f };

	forward = forward.Rotate( rotation * DegToRad );

	float spawnDistance = a_player->GetCollisionRadius() + 10.0f;

	Vector2 bulletPosition = a_player->GetTransform().position + (forward * spawnDistance);

	auto bullet = std::make_unique<Bullet>( Transform{ bulletPosition, rotation, 4.0f }, a_bulletModel, 700.0f, 2.0f );

	a_gameScene.AddActor( std::move(bullet) );
}

void SpaceGame::HandleMouseInput() {
	Input& input = engine.GetInput();

	if ( input.GetButtonPressed( Input::MouseButton::Left ) ) {
		Vector2 position = input.GetMousePosition();

		a_mousePoints.push_back(position);
		a_startsNewShape.push_back(true);
	}
	else if ( input.GetMouseDown( Input::MouseButton::Left ) ) {
		Vector2 position = input.GetMousePosition();

		if (!a_mousePoints.empty()) {
			Vector2 difference = position - a_mousePoints.back();

			if (difference.Length() > 10.0f) {
				a_mousePoints.push_back(position);
				a_startsNewShape.push_back(false);
			}
		}
	}
}

void SpaceGame::CheckCollisions() {
	for ( auto& actor : a_gameScene.GetActors() ) {
		Bullet* bullet = dynamic_cast<Bullet*>( actor.get() );

		if ( bullet == nullptr || bullet->IsDestroyed() ) { continue; }

		for ( auto& otherActor : a_gameScene.GetActors() ) {
			Enemy* enemy = dynamic_cast<Enemy*>( otherActor.get() );

			if ( enemy == nullptr || enemy->IsDestroyed() ) { continue; }

			if (bullet->IsColliding(*enemy)) {
				bullet->Destroy();
				enemy->Destroy();

				a_score++;

				break;
			}
		}
	}
}

void SpaceGame::Draw( const Renderer& renderer ) {
	renderer.SetColor(0, 0, 0, 255);
	renderer.Clear();

	renderer.SetColor(255, 255, 255, 255);

	for ( std::size_t i = 0; i + 1 < a_mousePoints.size(); i++ ) {
		if (!a_startsNewShape[i + 1]) {
			renderer.DrawLine( a_mousePoints[i].x, a_mousePoints[i].y, a_mousePoints[i + 1].x, a_mousePoints[i + 1].y );
		}
	}

	Game::Draw(renderer);

	a_text.Draw( renderer, 400.0f, 400.0f );

	renderer.Present();
}

void SpaceGame::Shutdown() {
	a_gameScene.RemoveAll();
	a_player = nullptr;

	Game::Shutdown();
}