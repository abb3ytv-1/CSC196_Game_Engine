#include "../Engine/Engine.h"

#include <iostream>
#include <memory>
#include <string>
#include <vector>

using namespace nu;

int main() {
	/*
	* Things to add later:
	* - Bullet class
	* - Player shooting
	* - Enemy collisions
	* - Actor tags
	* - Additional scene management
	*/

	// INITIALIZE

	if (!engine.Initialize()) {
		return 1;
	}

	Renderer& renderer = engine.GetRenderer();
	Input& input = engine.GetInput();
	AudioSystem& audio = engine.GetAudio();

	// FILESYSTEM TESTING

	std::cout << "Directory Operations:\n";
	std::cout
		<< "Working directory: "
		<< GetWorkingDirectory()
		<< '\n';

	std::cout << "Setting directory to 'Assets'...\n";

	if (!SetWorkingDirectory("Assets")) {
		std::cerr
			<< "Could not open the Assets directory.\n";

		engine.Shutdown();
		return 1;
	}

	std::cout
		<< "New directory: "
		<< GetWorkingDirectory()
		<< "\n\n";

	std::cout << "Files in Directory:\n";

	auto filenames =
		GetFilesInDirectory(GetWorkingDirectory());

	for (const auto& filename : filenames) {
		std::cout << filename << '\n';
	}

	std::cout << '\n';

	if (!filenames.empty()) {
		std::string value =
			GetFilename(filenames[0]);

		std::cout
			<< "Filename: "
			<< value
			<< '\n';

		value = GetFileExtension(filenames[0]);

		std::cout
			<< "Extension: "
			<< value
			<< '\n';

		value =
			GetFilenameNoExtension(filenames[0]);

		std::cout
			<< "Filename No Extension: "
			<< value
			<< "\n\n";
	}

	std::cout << "Text File Reading:\n";

	std::string text;

	if (ReadTextFile("test.txt", text)) {
		std::cout << text << '\n';
	}
	else {
		std::cerr << "Could not read test.txt\n";
	}

	std::cout << "Text File Writing:\n";

	WriteTextFile(
		"test.txt",
		"\nHello, World!",
		true
	);

	if (ReadTextFile("test.txt", text)) {
		std::cout << text << '\n';
	}

	// AUDIO

	audio.LoadSound("bass", "bass.wav");
	audio.LoadSound("snare", "snare.wav");
	audio.LoadSound("clap", "clap.wav");
	audio.LoadSound("open-hat", "open-hat.wav");
	audio.LoadSound("cowbell", "cowbell.wav");

	// FISH MODEL POINTS

	std::vector<Vector2> bodyPoints{
		Vector2{ -6.0f, 0.0f },
		Vector2{ -3.0f, -3.0f },
		Vector2{ 3.0f, -3.0f },
		Vector2{ 7.0f, 0.0f },
		Vector2{ 3.0f, 3.0f },
		Vector2{ -3.0f, 3.0f },
		Vector2{ -6.0f, 0.0f }
	};

	std::vector<Vector2> tailPoints{
		Vector2{ -6.0f, 0.0f },
		Vector2{ -10.0f, -4.0f },
		Vector2{ -9.0f, 0.0f },
		Vector2{ -10.0f, 4.0f },
		Vector2{ -6.0f, 0.0f }
	};

	std::vector<Vector2> topFinPoints{
		Vector2{ -2.0f, -3.0f },
		Vector2{ 0.0f, -6.0f },
		Vector2{ 2.0f, -3.0f },
		Vector2{ -2.0f, -3.0f }
	};

	std::vector<Vector2> sideFinPoints{
		Vector2{ 0.0f, 1.0f },
		Vector2{ 2.0f, 4.0f },
		Vector2{ 3.0f, 1.0f },
		Vector2{ 0.0f, 1.0f }
	};

	std::vector<Vector2> eyePoints{
		Vector2{ 4.0f, -1.0f },
		Vector2{ 5.0f, -1.0f },
		Vector2{ 5.0f, 0.0f },
		Vector2{ 4.0f, 0.0f },
		Vector2{ 4.0f, -1.0f }
	};

	// PLAYER MODEL

	Mesh bodyMesh{
		bodyPoints,
		Color{ 0.45f, 0.85f, 0.80f }
	};

	Mesh tailMesh{
		tailPoints,
		Color{ 1.0f, 0.72f, 0.25f }
	};

	Mesh topFinMesh{
		topFinPoints,
		Color{ 0.65f, 0.45f, 0.90f }
	};

	Mesh sideFinMesh{
		sideFinPoints,
		Color{ 0.95f, 0.45f, 0.70f }
	};

	Mesh eyeMesh{
		eyePoints,
		Color{ 1.0f, 1.0f, 1.0f }
	};

	Model fishModel;

	fishModel.AddMesh(bodyMesh);
	fishModel.AddMesh(tailMesh);
	fishModel.AddMesh(topFinMesh);
	fishModel.AddMesh(sideFinMesh);
	fishModel.AddMesh(eyeMesh);

	// ENEMY MODEL

	Mesh enemyBodyMesh{
		bodyPoints,
		Color{ 1.0f, 0.2f, 0.2f }
	};

	Mesh enemyTailMesh{
		tailPoints,
		Color{ 0.7f, 0.0f, 0.0f }
	};

	Mesh enemyTopFinMesh{
		topFinPoints,
		Color{ 0.8f, 0.1f, 0.1f }
	};

	Mesh enemySideFinMesh{
		sideFinPoints,
		Color{ 0.6f, 0.0f, 0.0f }
	};

	Mesh enemyEyeMesh{
		eyePoints,
		Color{ 0.0f, 0.0f, 0.0f }
	};

	Model enemyModel;

	enemyModel.AddMesh(enemyBodyMesh);
	enemyModel.AddMesh(enemyTailMesh);
	enemyModel.AddMesh(enemyTopFinMesh);
	enemyModel.AddMesh(enemySideFinMesh);
	enemyModel.AddMesh(enemyEyeMesh);

	// SCENE AND ACTORS

	Scene scene;

	auto playerActor = std::make_unique<Actor>(
		Transform{
			Vector2{ 960.0f, 540.0f },
			0.0f,
			10.0f
		},
		fishModel
	);

	// Keep a non-owning pointer for input and enemy targets.
	// Scene owns the actual Actor.
	Actor* player = playerActor.get();

	scene.AddActor(std::move(playerActor));

	auto enemyOne = std::make_unique<Enemy>(
		Transform{
			Vector2{ 200.0f, 200.0f },
			0.0f,
			8.0f
		},
		enemyModel,
		100.0f
	);

	enemyOne->SetTarget(*player);
	scene.AddActor(std::move(enemyOne));

	auto enemyTwo = std::make_unique<Enemy>(
		Transform{
			Vector2{ 1700.0f, 300.0f },
			0.0f,
			8.0f
		},
		enemyModel,
		125.0f
	);

	enemyTwo->SetTarget(*player);
	scene.AddActor(std::move(enemyTwo));

	auto enemyThree = std::make_unique<Enemy>(
		Transform{
			Vector2{ 400.0f, 900.0f },
			0.0f,
			8.0f
		},
		enemyModel,
		75.0f
	);

	enemyThree->SetTarget(*player);
	scene.AddActor(std::move(enemyThree));

	// MOUSE DRAWING CONTAINERS

	std::vector<Vector2> points;
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
				event.key.scancode ==
				SDL_SCANCODE_ESCAPE
				) {
				quit = true;
			}
		}

		// UPDATE ENGINE

		engine.Update();

		float dt =
			engine.GetTime().GetDeltaTime();

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

		// PLAYER MOVEMENT

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

		float movementSpeed = 300.0f;

		player->SetVelocity(
			direction * movementSpeed
		);

		// UPDATE ALL ACTORS

		scene.Update(dt);

		// MOUSE DRAWING

		if (
			input.GetButtonPressed(
				Input::MouseButton::Left
			)
			) {
			Vector2 position =
				input.GetMousePosition();

			points.push_back(position);
			startsNewShape.push_back(true);
		}
		else if (
			input.GetMouseDown(
				Input::MouseButton::Left
			)
			) {
			Vector2 position =
				input.GetMousePosition();

			if (!points.empty()) {
				Vector2 difference =
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

		// Draw every actor in the Scene
		scene.Draw(renderer);

		renderer.Present();
	}

	// SHUTDOWN

	scene.RemoveAll();
	engine.Shutdown();

	return 0;
}