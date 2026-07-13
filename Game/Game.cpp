#include "../Engine/Engine.h"

#include <vector>
using namespace nu;

int main() {
    
    // INITIALIZE
    Renderer renderer;

    if (!renderer.Initialize("Drawing Program", 1920, 1080)) {
        return 1;
    }

    Input input;

    if (!input.Initialize()) {
        renderer.Shutdown();
        return 1;
    }

    // Store all recorded mouse positions
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

        // UPDATE
        input.Update();

        // Start a new shape when the mouse is first pressed
        if (input.GetButtonPressed(Input::MouseButton::Left)) {
            Vector2 position = input.GetMousePosition();

            points.push_back(position);
            startsNewShape.push_back(true);
        }

        // Continue the current shape while the mouse is held down
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

        // Draw white lines between each pair of points
        renderer.SetColor(255, 255, 255, 255);

        for (size_t i = 0; i + 1 < points.size(); i++) {
            // Do not connect to a point that starts a new shape
            if (!startsNewShape[i + 1]) {
                renderer.DrawLine(
                    points[i].x,
                    points[i].y,
                    points[i + 1].x,
                    points[i + 1].y
                );
            }
        }

        renderer.Present();
    }

    // SHUTDOWN
    input.Shutdown();
    renderer.Shutdown();

    return 0;
}