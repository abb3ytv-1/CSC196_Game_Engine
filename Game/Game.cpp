#include "../Engine/Engine.h"

#include <vector>

using namespace nu;

int main() {
	// INITIALIZE
	Renderer renderer;

	if (!renderer.Initialize("Game Engine", 1920, 1080)) {
		return 1;
	}

	Input input;

	if (!input.Initialize()) {
		renderer.Shutdown();
		return 1;
	}

	Time time;

	// Character model
	std::vector<Vector2> modelPoints{
		Vector2{ -3.0f, 3.0f },
		Vector2{ 3.0f, 3.0f },
		Vector2{ 0.0f, -3.0f },
		Vector2{ -3.0f, 3.0f }
	};

	Mesh mesh{
		modelPoints,
		Color{ 0.0f, 0.0f, 1.0f }
	};

	Model model{
		std::vector<Mesh>{ mesh }
	};

	Actor player{
		Transform{
			Vector2{ 640.0f, 512.0f },
			0.0f,
			50.0f
		},
		model
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

		// UPDATE
		input.Update();
		time.Tick();

		float dt = time.GetDeltaTime();

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

		float movementSpeed = 300.0f;

		if (direction.LengthSqr() > 0.0f) {
			direction = direction.Normalized();
		}

		player.SetVelocity(direction * movementSpeed);
		player.Update(dt);

		// Start a new drawing
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

		// Draw the mouse lines
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

		// Draw the character
		player.Draw(renderer);

		renderer.Present();
	}

	// SHUTDOWN
	input.Shutdown();
	renderer.Shutdown();

	return 0;
}