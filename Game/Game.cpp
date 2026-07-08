#include "../Engine/Engine.h"

    int main()
    {
        // INITTIALIZEATION
        nu::Renderer renderer;
        renderer.Initialize("Game Engine", 1920, 1080);

        nu::Input input;
        input.Initialize();

        nu::Time time;

        nu::Vector2 position{ 640.0f, 512.0f };
        float speed = 400.0f;
        std::vector<nu::Vector2> points;


        uint16_t ticks = SDL_GetTicksNS();
        uint16_t prevTicks = ticks;

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
                if (event.type == SDL_EVENT_KEY_DOWN && event.key.scancode == SDL_SCANCODE_ESCAPE) {
                    quit = true;
                }
            }

            //engine
            input.Update();
            time.Tick();

            //std::cout << input.GetMousePotiion().x << ", "
            //    << input.GetMousePotiion().y << '\n';

            //if (input.GetKeyPress(SDL_SCANCODE_Q)) std::cout << "pressed\n";
            //if (input.GetKeyPress(SDL_SCANCODE_Q)) std::cout << "down\n";
            //if (input.GetKeyPress(SDL_SCANCODE_Q)) std::cout << "\n";

            //if (input.GetButtonPressed(nu::Input::MouseButton::Left)) std::cout << "Button pressed\n";
            //if (input.GetButtonDown(nu::Input::MouseButton::Left)) std::cout << "Button pressed\n";
            //if (input.GetButtonReleased(nu::Input::MouseButton::Left)) std::cout << "Button pressed\n";
            
            if (input.GetButtonPressed(nu::Input::MouseButton::Left)) {
                points.push_back(input.GetMousePotiion());
            }

            nu::Vector2 velocity{ 0.0f, 0.0f };
            if (input.GetKeyDown(SDL_SCANCODE_A)) velocity.x = -speed;
            if (input.GetKeyDown(SDL_SCANCODE_D)) velocity.x = +speed;
            if (input.GetKeyDown(SDL_SCANCODE_W)) velocity.y = -speed;
            if (input.GetKeyDown(SDL_SCANCODE_S)) velocity.y = +speed;
            position += (velocity * time.GetDeltaTime());

            // RENDER
            renderer.SetColor(0, 0, 0, 255);
            renderer.Clear();

            // Character
            renderer.SetColor(0.0f, 255.0f, 255.0f, 255.0f);
            renderer.DrawFillRect(position.x - 20, position.y - 20, 40, 40);

            renderer.Present();


            //// Draw 3000 random points
            //for (int i = 0; i < 3000; i++) {
            //    renderer.SetColor(nu::RandomInt(256), nu::RandomInt(256), nu::RandomInt(256), 255);

            //    renderer.DrawPoint(
            //        points[i].x,
            //        points[i].y
            //    );
            //}

            //renderer.SetColor(255, 255, 255, 255);
            //renderer.DrawRect(100, 100, 300, 300);


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
