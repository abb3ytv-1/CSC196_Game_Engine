#include "../Engine/Engine.h"
#include "../Engine/Enemy.h"

#include <iostream>
#include <string>
#include <vector>

using namespace nu;

int main() {
	/*
	* Things to add later:
	* - Bullets
	* - Scene class
	* - Pending actors
	* - Update actors through Scene
	* - Remove destroyed actors
	* - Actor lifespan
	* - More comments and code cleanup
	*/

	// INITIALIZE
	if (!engine.Initialize()) {
		return 1;
	}

	Renderer& renderer = engine.GetRenderer();
	Input& input = engine.GetInput();

	// FILESYSTEM TESTING

	// Get current working directory
	std::cout << "Directory Operations:\n";
	std::cout
		<< "Working directory: "
		<< GetWorkingDirectory()
		<< '\n';

	// Change from Build to Build/Assets
	std::cout << "Setting directory to 'Assets'...\n";

	if (!SetWorkingDirectory("Assets")) {
		std::cerr << "Could not open the Assets directory.\n";
		engine.Shutdown();
		return 1;
	}

	std::cout
		<< "New directory: "
		<< GetWorkingDirectory()
		<< "\n\n";

	// Get files in Assets
	std::cout << "Files in Directory:\n";

	auto filenames =
		GetFilesInDirectory(GetWorkingDirectory());

	for (const auto& filename : filenames) {
		std::cout << filename << '\n';
	}

	std::cout << '\n';

	// Display filename information
	if (!filenames.empty()) {
		std::string value = GetFilename(filenames[0]);
		std::cout << "Filename: " << value << '\n';

		value = GetFileExtension(filenames[0]);
		std::cout << "Extension: " << value << '\n';

		value = GetFilenameNoExtension(filenames[0]);
		std::cout
			<< "Filename No Extension: "
			<< value
			<< "\n\n";
	}

	// Read test.txt
	std::cout << "Text File Reading:\n";

	std::string text;

	if (ReadTextFile("test.txt", text)) {
		std::cout << text << '\n';
	}
	else {
		std::cerr << "Could not read test.txt\n";
	}

	// Append to test.txt
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

	AudioSystem& audio = engine.GetAudio();

	audio.LoadSound("bass.wav");
	audio.LoadSound("snare.wav");
	audio.LoadSound("clap.wav");
	audio.LoadSound("open-hat.wav");
	audio.LoadSound("cowbell.wav");

	// PLAYER MODEL

	// Fish body
	std::vector<Vector2> bodyPoints{
		Vector2{ -6.0f, 0.0f },
		Vector2{ -3.0f, -3.0f },
		Vector2{ 3.0f, -3.0f },
		Vector2{ 7.0f, 0.0f },
		Vector2{ 3.0f, 3.0f },
		Vector2{ -3.0f, 3.0f },
		Vector2{ -6.0f, 0.0f }
	};

	// Fish tail
	std::vector<Vector2> tailPoints{
		Vector2{ -6.0f, 0.0f },
		Vector2{ -10.0f, -4.0f },
		Vector2{ -9.0f, 0.0f },
		Vector2{ -10.0f, 4.0f },
		Vector2{ -6.0f, 0.0f }
	};

	// Fish top fin
	std::vector<Vector2> topFinPoints{
		Vector2{ -2.0f, -3.0f },
		Vector2{ 0.0f, -6.0f },
		Vector2{ 2.0f, -3.0f },
		Vector2{ -2.0f, -3.0f }
	};

	// Fish side fin
	std::vector<Vector2> sideFinPoints{
		Vector2{ 0.0f, 1.0f },
		Vector2{ 2.0f, 4.0f },
		Vector2{ 3.0f, 1.0f },
		Vector2{ 0.0f, 1.0f }
	};

	// Fish eye
	std::vector<Vector2> eyePoints{
		Vector2{ 4.0f, -1.0f },
		Vector2{ 5.0f, -1.0f },
		Vector2{ 5.0f, 0.0f },
		Vector2{ 4.0f, 0.0f },
		Vector2{ 4.0f, -1.0f }
	};

	// Create the player meshes
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

	// Add the meshes to the player model
	Model fishModel;

	fishModel.AddMesh(bodyMesh);
	fishModel.AddMesh(tailMesh);
	fishModel.AddMesh(topFinMesh);
	fishModel.AddMesh(sideFinMesh);
	fishModel.AddMesh(eyeMesh);

	// ENEMY MODEL

	// The enemies use the same points but have different colors
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

	// Add the enemy meshes to the enemy model
	Model enemyModel;

	enemyModel.AddMesh(enemyBodyMesh);
	enemyModel.AddMesh(enemyTailMesh);
	enemyModel.AddMesh(enemyTopFinMesh);
	enemyModel.AddMesh(enemySideFinMesh);
	enemyModel.AddMesh(enemyEyeMesh);

	// CREATE PLAYER

	Actor player{
		Transform{
			Vector2{ 960.0f, 540.0f },
			0.0f,
			10.0f
		},
		fishModel
	};

	// CREATE ENEMIES

	std::vector<Enemy> enemies{
		Enemy{
			Transform{
				Vector2{ 200.0f, 200.0f },
				0.0f,
				8.0f
			},
			enemyModel,
			100.0f
		},
		Enemy{
			Transform{
				Vector2{ 1700.0f, 300.0f },
				0.0f,
				8.0f
			},
			enemyModel,
			125.0f
		},
		Enemy{
			Transform{
				Vector2{ 400.0f, 900.0f },
				0.0f,
				8.0f
			},
			enemyModel,
			75.0f
		}
	};

	// Give every enemy the player as its target
	for (Enemy& enemy : enemies) {
		enemy.SetTarget(player);
	}

	// Store recorded mouse positions
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
				event.key.scancode == SDL_SCANCODE_ESCAPE
				) {
				quit = true;
			}
		}

		// UPDATE ENGINE
		engine.Update();

		float dt = engine.GetTime().GetDeltaTime();

		// AUDIO INPUT

		if (input.GetKeyPress(SDL_SCANCODE_1)) {
			audio.PlaySound(0);
		}

		if (input.GetKeyPress(SDL_SCANCODE_2)) {
			audio.PlaySound(1);
		}

		if (input.GetKeyPress(SDL_SCANCODE_3)) {
			audio.PlaySound(2);
		}

		if (input.GetKeyPress(SDL_SCANCODE_4)) {
			audio.PlaySound(3);
		}

		if (input.GetKeyPress(SDL_SCANCODE_5)) {
			audio.PlaySound(4);
		}

		// PLAYER ROTATION

		float rotation = player.GetTransform().rotation;
		float rotationSpeed = 180.0f;

		if (input.GetKeyDown(SDL_SCANCODE_LEFT)) {
			rotation -= rotationSpeed * dt;
		}

		if (input.GetKeyDown(SDL_SCANCODE_RIGHT)) {
			rotation += rotationSpeed * dt;
		}

		player.SetRotation(rotation);

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

		player.SetVelocity(direction * movementSpeed);
		player.Update(dt);

		// UPDATE ENEMIES

		for (Enemy& enemy : enemies) {
			enemy.Update(dt);
		}

		// MOUSE DRAWING

		// Start a new mouse drawing
		if (input.GetButtonPressed(Input::MouseButton::Left)) {
			Vector2 position = input.GetMousePosition();

			points.push_back(position);
			startsNewShape.push_back(true);
		}

		// Continue drawing while the mouse is held
		else if (input.GetMouseDown(Input::MouseButton::Left)) {
			Vector2 position = input.GetMousePosition();

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

		// Draw mouse lines
		renderer.SetColor(255, 255, 255, 255);

		for (std::size_t i = 0; i + 1 < points.size(); i++) {
			if (!startsNewShape[i + 1]) {
				renderer.DrawLine(
					points[i].x,
					points[i].y,
					points[i + 1].x,
					points[i + 1].y
				);
			}
		}

		// Draw the player
		player.Draw(renderer);

		// Draw the enemies
		for (const Enemy& enemy : enemies) {
			enemy.Draw(renderer);
		}

		renderer.Present();
	}

	// SHUTDOWN
	engine.Shutdown();

	return 0;
}