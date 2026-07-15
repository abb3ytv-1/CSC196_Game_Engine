#include "../Engine/Engine.h"

#include <vector>

using namespace nu;

int main() {
	// INITIALIZE
	engine.Initialize();

	// Character model
	std::vector<Vector2> modelPoints{ Vector2{ -3.0f, 3.0f }, Vector2{ 3.0f, 3.0f }, Vector2{ 0.0f, -3.0f }, Vector2{ -3.0f, 3.0f } };

	Mesh mesh{ modelPoints, Color{ 0.0f, 0.0f, 1.0f } };
	Model model{ std::vector<Mesh>{ mesh } };
	Actor player{ Transform{ Vector2 { 640.0f, 512.0f }, 0.0f, 50.0f }, model };

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

			if ( event.type == SDL_EVENT_KEY_DOWN && event.key.scancode == SDL_SCANCODE_ESCAPE ) {
				quit = true;
			}
		}

		// engine
		engine.Update();

		player.SetRotation(player.GetTransform().rotation + (90.0f * engine.GetTime().GetDeltaTime()));
		player.Update(engine.GetTime().GetDeltaTime());
		float dt = engine.GetTime().GetDeltaTime();

		// Start a new drawing
		if (engine.GetInput().GetButtonPressed(Input::MouseButton::Left)) {
			Vector2 position = engine.GetInput().GetMousePosition();

			points.push_back(position);
			startsNewShape.push_back(true);
		}

		// Continue drawing while the mouse is held
		else if (engine.GetInput().GetMouseDown(Input::MouseButton::Left)) {
			Vector2 position = engine.GetInput().GetMousePosition();

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
	engine.GetInput().Shutdown();
	engine.GetRenderer().Shutdown();

	return 0;
}