#include "../Engine/Engine.h"

#include <vector>

using namespace nu;

int main() {
	// INITIALIZE
	if (!engine.Initialize()) {
		return 1;
	}

	Renderer& renderer = engine.GetRenderer();
	Input& input = engine.GetInput();

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

	// Create meshes with different colors
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

	// Add all meshes to the model
	Model fishModel;

	fishModel.AddMesh(bodyMesh);
	fishModel.AddMesh(tailMesh);
	fishModel.AddMesh(topFinMesh);
	fishModel.AddMesh(sideFinMesh);
	fishModel.AddMesh(eyeMesh);

	Actor player{
		Transform{
			Vector2{ 960.0f, 540.0f },
			0.0f,
			30.0f
		},
		fishModel
	};

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

		// Character movement
		Vector2 direction{ 0.0f, 0.0f };

		if (
			input.GetKeyDown(SDL_SCANCODE_W) ||
			input.GetKeyDown(SDL_SCANCODE_UP)
			) {
			direction.y -= 1.0f;
		}

		if (
			input.GetKeyDown(SDL_SCANCODE_S) ||
			input.GetKeyDown(SDL_SCANCODE_DOWN)
			) {
			direction.y += 1.0f;
		}

		if (
			input.GetKeyDown(SDL_SCANCODE_A) ||
			input.GetKeyDown(SDL_SCANCODE_LEFT)
			) {
			direction.x -= 1.0f;
		}

		if (
			input.GetKeyDown(SDL_SCANCODE_D) ||
			input.GetKeyDown(SDL_SCANCODE_RIGHT)
			) {
			direction.x += 1.0f;
		}

		if (direction.LengthSqr() > 0.0f) {
			direction = direction.Normalized();
		}

		float movementSpeed = 300.0f;

		player.SetVelocity(direction * movementSpeed);
		player.Update(dt);

		// This was added in class, so it can stay.
		// Leave it commented while taking the right-facing screenshot.
		// player.SetRotation(
		// 	player.GetTransform().rotation + (90.0f * dt)
		// );

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
				Vector2 difference = position - points.back();

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

		for (size_t i = 0; i + 1 < points.size(); i++) {
			if (!startsNewShape[i + 1]) {
				renderer.DrawLine(
					points[i].x,
					points[i].y,
					points[i + 1].x,
					points[i + 1].y
				);
			}
		}

		// Draw the fish model
		player.Draw(renderer);

		renderer.Present();
	}

	// SHUTDOWN
	engine.Shutdown();

	return 0;
}