#include "Space_Game.h"

#include "../Engine/Enemy.h"
#include "../Engine/Engine.h"
#include "../Engine/File.h"
#include "../Engine/MathUtils.h"
#include "../Engine/ParticleSystem.h"
#include "../Engine/Random.h"

#include "Assets.h"
#include "Bullet.h"
#include "Player.h"

#include <SDL3/SDL.h>
#include <string>
#include <memory>
#include <utility>

using namespace nu;

SpaceGame::SpaceGame() :
	a_stateText{ &a_font }, a_hudText{ &a_font }
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
	if (!a_font.Load( "Fonts/New Moon.ttf", 100.0f )) { return false; }
	if (!a_stateText.Create(engine.GetRenderer(), "Fishy Adventure - Press Enter to Start", Color{ 0.45f, 0.85f, 1.0f })) { return false; }
	if (!a_hudText.Create(engine.GetRenderer(), "Score: 0 | Lives: 3", Color{ 1.0f, 1.0f, 1.0f })){return false;}
	if (!LoadAudio()) { return false; }

	a_playerModel = CreatePlayerModel();
	a_enemyModel = CreateEnemyModel();
	a_bulletModel = CreateBulletModel();
	a_gameState = GameState::StartGame;

	return true;
}

bool SpaceGame::LoadAudio() {
	Audio& audio = engine.GetAudio();

	bool loaded = true;

	loaded &= audio.AddSound( "bass", "Audio/bass.wav" );

	loaded &= audio.AddSound( "snare", "Audio/snare.wav" );

	loaded &= audio.AddSound( "clap", "Audio/clap.wav" );

	loaded &= audio.AddSound( "open-hat", "Audio/open-hat.wav" );

	loaded &= audio.AddSound( "cowbell", "Audio/cowbell.wav" );

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
	Input& input = engine.GetInput();

	switch (a_gameState) {
	case GameState::Title:
	case GameState::StartGame:
		if (input.GetKeyPress(SDL_SCANCODE_RETURN)) { StartNewGame(); }
		break;

	case GameState::StartLevel:
		a_gameState = GameState::Game;
		break;

	case GameState::Game:
		if (a_playerInvincibilityTimer > 0.0f) {
			a_playerInvincibilityTimer -= dt;
		}

		HandleAudioInput();
		HandlePlayerInput(dt);
		EmitPlayerParticle();
		HandleMouseInput();

		Game::Update(dt);

		CheckCollisions();
		break;

	case GameState::GameOver:
		if (input.GetKeyPress(SDL_SCANCODE_RETURN)) { StartNewGame(); }
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
				Vector2 explosionPosition =
					enemy->GetTransform().position;

				bullet->Destroy();
				enemy->Destroy();

				CreateExplosion( explosionPosition, Color{ 1.0f, 0.65f, 0.2f }, 100 );

				a_score++;
				UpdateHUDText();

				break;
			}

			if ( a_player == nullptr || a_player->IsDestroyed() || a_playerInvincibilityTimer > 0.0f ) { return; }

			for (auto& actor : a_gameScene.GetActors()) {
				Enemy* enemy = dynamic_cast<Enemy*>( actor.get() );

				if ( enemy == nullptr || enemy->IsDestroyed() ) { continue; }

				if (!a_player->IsColliding(*enemy)) { continue; }

				Vector2 collisionPosition = enemy->GetTransform().position;

				enemy->Destroy();

				CreateExplosion( collisionPosition, Color{ 1.0f, 0.2f, 0.2f }, 75 );

				a_lives--;
				UpdateHUDText();

				if (a_lives <= 0) { EndGame(); return; }

				// Return the player to the center.
				Renderer& renderer = engine.GetRenderer();

				a_player->SetPosition( Vector2{ renderer.GetWidth() * 0.5f, renderer.GetHeight() * 0.5f } );

				a_player->SetVelocity( Vector2{ 0.0f, 0.0f } );

				a_player->SetRotation(0.0f);

				// Prevent another immediate collision.
				a_playerInvincibilityTimer = 1.5f;

				break;
			}
		}
	}
}

void SpaceGame::CreateExplosion(const Vector2& position, const Color& color, int particleCount) {
	ParticleSystem& particleSystem = engine.GetPS();

	for (int i = 0; i < particleCount; i++) {
		Particle particle;

		particle.position = position;
		particle.color = color;

		particle.lifespan = RandomFloat(0.5f, 2.0f);

		particle.velocity = Vector2 {
			RandomFloat(-600.0f, 600.0f),
			RandomFloat(-600.0f, 600.0f)
		};

		particleSystem.AddParticle(particle);
	}
}

void SpaceGame::EmitPlayerParticle() {
	if (a_player == nullptr || a_player->IsDestroyed()) { return; }

	// only create trail while moving
	if (a_player->GetVelocity().LengthSqr() <= 0.0f) { return; }

	float rotation = a_player->GetTransform().rotation;
	Vector2 forward{ 1.0f, 0.0f };
	forward = forward.Rotate(rotation * DegToRad);
	float trailDistance = a_player->GetCollisionRadius() + 20.0f;
	Particle particle;

	// Spawn the particle behind player
	particle.position = a_player->GetTransform().position - (forward * trailDistance);

	// Add a small random spread
	particle.position.x += RandomFloat(-5.0f, 5.0f);
	particle.position.y += RandomFloat(-5.0f, 5.0f);
	particle.color = Color{ 0.4f, 0.8f, 1.0f };
	particle.lifespan = RandomFloat(0.25f, 0.75f);

	// Move mostly away from the back of the fish
	particle.velocity = (forward * RandomFloat(-100.0f, -40.0f)) + Vector2{
		RandomFloat(-30.0f, 30.0f),
		RandomFloat(-30.0f, 30.0f)
	};

	engine.GetPS().AddParticle(particle);
}

void SpaceGame::StartNewGame() {
	a_gameScene.RemoveAll();
	a_player = nullptr;
	a_score = 0;
	a_lives = 3;
	a_mousePoints.clear();
	a_startsNewShape.clear();
	a_playerInvincibilityTimer = 0.0f;

	CreateActors();
	UpdateHUDText();

	a_gameState = GameState::Game;
}

void SpaceGame::EndGame() {
	a_gameScene.RemoveAll();
	a_player = nullptr;

	std::string message = "Game Over | Score: " +
		std::to_string(a_score) +
		" | Press Enter to Play Again";

	a_stateText.Create(engine.GetRenderer(), message, Color{ 1.0f, 0.25, 0.25f });

	a_gameState = GameState::GameOver;
}

void SpaceGame::UpdateHUDText() {
	std::string hud = "Score: " + std::to_string(a_score) + " | Lives: " + std::to_string(a_lives);

	a_hudText.Create(engine.GetRenderer(), hud, Color{ 1.0f, 1.0f, 1.0f });
}

void SpaceGame::Draw( const Renderer& renderer ) {
	renderer.SetColor(0, 0, 0, 255);
	renderer.Clear();

	switch (a_gameState) {
	case GameState::Title:
	case GameState::StartGame:
		a_stateText.Draw( renderer, 250.0f, 475.0f );
		break;

	case GameState::StartLevel:
		a_stateText.Draw( renderer, 800.0f, 500.0f );
		break;

	case GameState::Game:
		renderer.SetColor( 255, 255, 255, 255 );

		for ( std::size_t i = 0; i + 1 < a_mousePoints.size(); i++ ) {
			if (!a_startsNewShape[i + 1]) {
				renderer.DrawLine( a_mousePoints[i].x, a_mousePoints[i].y, a_mousePoints[i + 1].x, a_mousePoints[i + 1].y );
			}
		}

		Game::Draw(renderer);

		engine.GetPS().Draw(renderer);

		a_hudText.Draw( renderer, 20.0f, 20.0f );
		break;

	case GameState::GameOver:
		engine.GetPS().Draw(renderer);

		a_stateText.Draw( renderer, 250.0f, 475.0f );
		break;

	default:
		break;
	}

	renderer.Present();
}

void SpaceGame::Shutdown() {
	a_gameScene.RemoveAll();
	a_player = nullptr;

	Game::Shutdown();
}