#include "../Engine/Engine.h"

#include <vector>

int main()
{
    // INITIALIZE
    nu::Renderer renderer;

    if (!renderer.Initialize("Drawing Program", 1920, 1080))
    {
        return 1;
    }

    nu::Input input;

    if (!input.Initialize())
    {
        renderer.Shutdown();
        return 1;
    }

    // Store all recorded mouse positions
    std::vector<nu::Vector2> points;

    bool quit = false;

    // MAIN LOOP
    while (!quit)
    {
        // EVENTS
        SDL_Event event;

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                quit = true;
            }

            if (
                event.type == SDL_EVENT_KEY_DOWN &&
                event.key.scancode == SDL_SCANCODE_ESCAPE
                )
            {
                quit = true;
            }
        }

        // UPDATE
        input.Update();

        // Add points while the left mouse button is held down
        if (input.GetMouseDown(nu::Input::MouseButton::Left))
        {
            nu::Vector2 position = input.GetMousePosition();

            // Always add the first point
            if (points.empty())
            {
                points.push_back(position);
            }
            else
            {
                // Find the distance between the mouse and the last point
                nu::Vector2 difference = position - points.back();

                // Only add another point when the mouse has moved far enough
                if (difference.Length() > 10.0f)
                {
                    points.push_back(position);
                }
            }
        }

        // RENDER
        renderer.SetColor(0, 0, 0, 255);
        renderer.Clear();

        // Draw white lines between each pair of points
        renderer.SetColor(255, 255, 255, 255);

        for (size_t i = 0; i + 1 < points.size(); i++)
        {
            renderer.DrawLine(
                points[i].x,
                points[i].y,
                points[i + 1].x,
                points[i + 1].y
            );
        }

        renderer.Present();
    }

    // SHUTDOWN
    input.Shutdown();
    renderer.Shutdown();

    return 0;
}