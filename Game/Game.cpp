#include "../Engine/Engine.h"

int main()
{
    // INITTIALIZEATION
    nu::Renderer renderer;
    renderer.Initialize("Game Engine", 1920, 1080);

    std::vector<nu::Vector2> v;

    for (int i = 0; i < 3000; i++) {
        v.push_back({ nu::RandomFloat(1920), nu::RandomFloat(1080) });
    }

    // handle events
    bool quit = false;

    while (!quit) {
        // UPDATE
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT)
            {
                quit = true;
            }
        }

        // RENDER
        renderer.SetColor(0, 0, 0, 255);
        renderer.Clear();

        // Draw 3000 random points
        for (int i = 0; i < 3000; i++) {
            renderer.SetColor(nu::RandomInt(256), nu::RandomInt(256), nu::RandomInt(256), 255);

            renderer.DrawPoint(
                v[i].x,
                v[i].y
            );
        }

        //// Draw 10 random lines
        //for (int i = 0; i < 10; i++) {
        //    renderer.SetColor(nu::RandomInt(256), nu::RandomInt(256), nu::RandomInt(256), 255);

        //    renderer.DrawLine(
        //        nu::RandomFloat(1280),
        //        nu::RandomFloat(1024),
        //        nu::RandomFloat(1280),
        //        nu::RandomFloat(1024)
        //    );
        //}

        //// Draw 10 random rectangles
        //for (int i = 0; i < 10; i++) {
        //    renderer.SetColor(nu::RandomInt(256), nu::RandomInt(256), nu::RandomInt(256), 255);

        //    renderer.DrawRect(
        //        nu::RandomInt(1180),
        //        nu::RandomInt(924),
        //        20 + nu::RandomInt(100),
        //        20 + nu::RandomInt(100)
        //    );
        //}

        renderer.Present();
    }

    // SHUTDOWN
    renderer.Shutdown();
    return 0;
};