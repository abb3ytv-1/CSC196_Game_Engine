#include "../Engine/Engine.h"

#include "Assets.h"
#include "Bullet.h"

#include <memory>
#include <vector>

using namespace nu;

int main() {
	/*
	* Things to add later:
	* - Actor tags
	* - Additional scene management
	*/

	// INITIALIZE

	if (!nu::engine.Initialize()) {
		return 1;
	}

	nu::Renderer& renderer =
		nu::engine.GetRenderer();

	nu::Input& input =
		nu::engine.GetInput();

	nu::AudioSystem& audio =
		nu::engine.GetAudio();

	// Set the working directory so assets can be found.

	if (!nu::SetWorkingDirectory("Assets")) {
		nu::engine.Shutdown();
		return 1;
	}

	// FONT AND TEXT

	nu::Font* font = new nu::Font();

	if (!font->Load("Fonts/New Moon.ttf", 20.0f)) {
		delete font;
		nu::engine.Shutdown();
		return 1;
	}

	nu::Text* text = new nu::Text(font);

	if (
		!text->Create(
			renderer,
			"Hello World",
			nu::Color{ 1.0f, 1.0f, 1.0f }
		)
		) {
		delete text;
		delete font;
		nu::engine.Shutdown();
		return 1;
	}

	// AUDIO

	audio.LoadSound("bass", "bass.wav");
	audio.LoadSound("snare", "snare.wav");
	audio.LoadSound("clap", "clap.wav");
	audio.LoadSound("open-hat", "open-hat.wav");
	audio.LoadSound("cowbell", "cowbell.wav");

	// FISH MODEL POINTS

	std::vector<nu::Vector2> bodyPoints{
		nu::Vector2{ -6.0f, 0.0f },
		nu::Vector2{ -3.0f, -3.0f },
		nu::Vector2{ 3.0f, -3.0f },
		nu::Vector2{ 7.0f, 0.0f },
		nu::Vector2{ 3.0f, 3.0f },
		nu::Vector2{ -3.0f, 3.0f },
		nu::Vector2{ -6.0f, 0.0f }
	};

	std::vector<nu::Vector2> tailPoints{
		nu::Vector2{ -6.0f, 0.0f },
		nu::Vector2{ -10.0f, -4.0f },
		nu::Vector2{ -9.0f, 0.0f },
		nu::Vector2{ -10.0f, 4.0f },
		nu::Vector2{ -6.0f, 0.0f }
	};

	std::vector<nu::Vector2> topFinPoints{
		nu::Vector2{ -2.0f, -3.0f },
		nu::Vector2{ 0.0f, -6.0f },
		nu::Vector2{ 2.0f, -3.0f },
		nu::Vector2{ -2.0f, -3.0f }
	};

	std::vector<nu::Vector2> sideFinPoints{
		nu::Vector2{ 0.0f, 1.0f },
		nu::Vector2{ 2.0f, 4.0f },
		nu::Vector2{ 3.0f, 1.0f },
		nu::Vector2{ 0.0f, 1.0f }
	};

	std::vector<nu::Vector2> eyePoints{
		nu::Vector2{ 4.0f, -1.0f },
		nu::Vector2{ 5.0f, -1.0f },
		nu::Vector2{ 5.0f, 0.0f },
		nu::Vector2{ 4.0f, 0.0f },
		nu::Vector2{ 4.0f, -1.0f }
	};

	// PLAYER MODEL

	nu::Mesh bodyMesh{
		bodyPoints,
		nu::Color{ 0.45f, 0.85f, 0.80f }
	};

	nu::Mesh tailMesh{
		tailPoints,
		nu::Color{ 1.0f, 0.72f, 0.25f }
	};

	nu::Mesh topFinMesh{
		topFinPoints,
		nu::Color{ 0.65f, 0.45f, 0.90f }
	};

	nu::Mesh sideFinMesh{
		sideFinPoints,
		nu::Color{ 0.95f, 0.45f, 0.70f }
	};

	nu::Mesh eyeMesh{
		eyePoints,
		nu::Color{ 1.0f, 1.0f, 1.0f }
	};

	nu::Model fishModel;

	fishModel.AddMesh(bodyMesh);
	fishModel.AddMesh(tailMesh);
	fishModel.AddMesh(topFinMesh);
	fishModel.AddMesh(sideFinMesh);
	fishModel.AddMesh(eyeMesh);

	// ENEMY MODEL

	nu::Mesh enemyBodyMesh{
		bodyPoints,
		nu::Color{ 1.0f, 0.2f, 0.2f }
	};

	nu::Mesh enemyTailMesh{
		tailPoints,
		nu::Color{ 0.7f, 0.0f, 0.0f }
	};

	nu::Mesh enemyTopFinMesh{
		topFinPoints,
		nu::Color{ 0.8f, 0.1f, 0.1f }
	};

	nu::Mesh enemySideFinMesh{
		sideFinPoints,
		nu::Color{ 0.6f, 0.0f, 0.0f }
	};

	nu::Mesh enemyEyeMesh{
		eyePoints,
		nu::Color{ 0.0f, 0.0f, 0.0f }
	};

	nu::Model enemyModel;

	enemyModel.AddMesh(enemyBodyMesh);
	enemyModel.AddMesh(enemyTailMesh);
	enemyModel.AddMesh(enemyTopFinMesh);
	enemyModel.AddMesh(enemySideFinMesh);
	enemyModel.AddMesh(enemyEyeMesh);

	// BULLET MODEL

	nu::Model bulletModel;
	bulletModel.AddMesh(bulletMesh);

	// SCENE AND ACTORS

	nu::Scene scene;

	auto playerActor = std::make_unique<nu::Actor>(
		nu::Transform{
			nu::Vector2{ 960.0f, 540.0f },
			0.0f,
			10.0f
		},
		fishModel
	);

	// Scene owns the player.
	// This pointer only provides access to that player.
	nu::Actor* player = playerActor.get();

	player->SetCollisionRadius(8.0f);

	scene.AddActor(std::move(playerActor));

	auto enemyOne = std::make_unique<nu::Enemy>(
		nu::Transform{
			nu::Vector2{ 200.0f, 200.0f },
			0.0f,
			8.0f
		},
		enemyModel,
		100.0f
	);

	enemyOne->SetTarget(*player);
	scene.AddActor(std::move(enemyOne));

	auto enemyTwo = std::make_unique<nu::Enemy>(
		nu::Transform{
			nu::Vector2{ 1700.0f, 300.0f },
			0.0f,
			8.0f
		},
		enemyModel,
		125.0f
	);

	enemyTwo->SetTarget(*player);
	scene.AddActor(std::move(enemyTwo));

	auto enemyThree = std::make_unique<nu::Enemy>(
		nu::Transform{
			nu::Vector2{ 400.0f, 900.0f },
			0.0f,
			8.0f
		},
		enemyModel,
		75.0f
	);

	enemyThree->SetTarget(*player);
	scene.AddActor(std::move(enemyThree));

	// MOUSE DRAWING CONTAINERS

	std::vector<nu::Vector2> points;
	std::vector<bool> startsNewShape;

	bool quit = false;

	// MAIN LOOP

	while (!quit) {
		// EVENTS

		SDL_Event event;

		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_EVENT_QUIT) {
				quit = true;
			}

			if (
				event.type == SDL_EVENT_KEY_DOWN &&
				event.key.scancode == SDL_SCANCODE_ESCAPE
				) {
				quit = true;
			}
		}

		// UPDATE ENGINE

		nu::engine.Update();

		float dt =
			nu::engine.GetTime().GetDeltaTime();

		// AUDIO INPUT

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

		// PLAYER ROTATION

		float rotation =
			player->GetTransform().rotation;

		float rotationSpeed = 180.0f;

		if (input.GetKeyDown(SDL_SCANCODE_LEFT)) {
			rotation -= rotationSpeed * dt;
		}

		if (input.GetKeyDown(SDL_SCANCODE_RIGHT)) {
			rotation += rotationSpeed * dt;
		}

		player->SetRotation(rotation);

		// PLAYER SHOOTING

		if (input.GetKeyPress(SDL_SCANCODE_SPACE)) {
			nu::Vector2 forward{ 1.0f, 0.0f };

			forward = forward.Rotate(
				rotation * nu::DegToRad
			);

			float spawnDistance =
				player->GetCollisionRadius() +
				10.0f;

			nu::Vector2 bulletPosition =
				player->GetTransform().position +
				(forward * spawnDistance);

			auto bullet =
				std::make_unique<Bullet>(
					nu::Transform{
						bulletPosition,
						rotation,
						4.0f
					},
					bulletModel,
					700.0f,
					2.0f
				);

			scene.AddActor(std::move(bullet));
		}

		// PLAYER MOVEMENT

		nu::Vector2 direction{ 0.0f, 0.0f };

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

		float movementSpeed = 300.0f;

		player->SetVelocity(
			direction * movementSpeed
		);

		// UPDATE ALL ACTORS

		scene.Update(dt);

		// BULLET AND ENEMY COLLISION

		for (auto& actor : scene.GetActors()) {
			Bullet* bullet =
				dynamic_cast<Bullet*>(actor.get());

			if (
				bullet == nullptr ||
				bullet->IsDestroyed()
				) {
				continue;
			}

			for (
				auto& otherActor :
				scene.GetActors()
				) {
				nu::Enemy* enemy =
					dynamic_cast<nu::Enemy*>(
						otherActor.get()
						);

				if (
					enemy == nullptr ||
					enemy->IsDestroyed()
					) {
					continue;
				}

				if (bullet->IsColliding(*enemy)) {
					bullet->Destroy();
					enemy->Destroy();

					break;
				}
			}
		}

		// MOUSE DRAWING

		if (
			input.GetButtonPressed(
				nu::Input::MouseButton::Left
			)
			) {
			nu::Vector2 position =
				input.GetMousePosition();

			points.push_back(position);
			startsNewShape.push_back(true);
		}
		else if (
			input.GetMouseDown(
				nu::Input::MouseButton::Left
			)
			) {
			nu::Vector2 position =
				input.GetMousePosition();

			if (!points.empty()) {
				nu::Vector2 difference =
					position - points.back();

				if (difference.Length() > 10.0f) {
					points.push_back(position);
					startsNewShape.push_back(false);
				}
			}
		}

		// RENDER

		renderer.SetColor(0, 0, 0, 255);
		renderer.Clear();

		renderer.SetColor(255, 255, 255, 255);

		for (
			std::size_t i = 0;
			i + 1 < points.size();
			i++
			) {
			if (!startsNewShape[i + 1]) {
				renderer.DrawLine(
					points[i].x,
					points[i].y,
					points[i + 1].x,
					points[i + 1].y
				);
			}
		}

		scene.Draw(renderer);

		text->Draw(
			renderer,
			400.0f,
			400.0f
		);

		renderer.Present();
	}

	// SHUTDOWN

	scene.RemoveAll();

	// Text and font must be deleted before SDL shuts down.
	delete text;
	delete font;

	nu::engine.Shutdown();

	return 0;
}