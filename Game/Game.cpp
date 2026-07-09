#include "../Engine/Engine.h"

using namespace nu;

struct Transform {
    Vector2 position;
    float rotation;
    float scale;
};

class Actor {
    public:
        Actor() = default;
        Actor(const Transform& transform) : a_transform{ transform } {}

        void Update(float dt) {
            a_transform.position += (a_velocity * dt);
            a_velocity *= 0.997f;

            a_transform.position.x = Wrap(0.0f, 1280.0f, a_transform.position.x);
            a_transform.position.y = Wrap(0.0f, 1024.0f, a_transform.position.y);
        }

        void Draw(const Renderer& renderer) {
            // Character
            renderer.SetColor(0.0f, 255.0f, 255.0f, 255.0f);
            renderer.DrawFillRect(a_transform.position.x - (a_transform.scale * 0.5f), a_transform.position.y - (a_transform.scale * 0.5f), a_transform.scale, a_transform.scale);
        }

        const Transform& GetTransform() { return a_transform; }
        void SetPosition(const Vector2& position) { a_transform.position = position; }
        void SetRotation(float rotation) { a_transform.rotation = rotation; }
        void SetScale(float scale) { a_transform.scale = scale; }

        const Vector2& GetVelocity() const { return a_velocity; }
        void SetVelocity(const Vector2& velocity) { a_velocity = velocity; }


    protected:
        Transform a_transform;
        Vector2 a_velocity{ 0,0 };
};

    int main()
    {
        // INITTIALIZEATION
        Renderer renderer;
        renderer.Initialize("Game Engine", 1920, 1080);

        Input input;
        input.Initialize();

        Time time;

        // player
        Actor player{ Transform{Vector2{645.0f, 512.0f}, 0.0f, 50.0f } };

        float speed = 400.0f;
        std::vector<nu::Vector2> points;

        //std::vector<Vector2>;

        //uint16_t ticks = SDL_GetTicksNS();
        //uint16_t prevTicks = ticks;

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

            // Draw points -> lines
            if (input.GetButtonPressed(nu::Input::MouseButton::Left))
            {
                if (points.empty())
                {
                    points.push_back(input.GetMousePosition());
                }
                else
                {
                    Vector2 v = points.back() - input.GetMousePosition();

                    if (v.Length() > 10.0f)
                    {
                        points.push_back(input.GetMousePosition());
                    }
                }
            }

            // Undo point
            if (input.GetButtonPressed(nu::Input::MouseButton::Right))
            {
                if (!points.empty())
                {
                    points.pop_back();
                }
            }

            // Draw lines
            renderer.SetColor(255.0f, 255.0f, 255.0f, 255.0f);

            for (size_t i = 0; i + 1 < points.size(); i++)
            {
                renderer.DrawLine(
                    points[i].x,
                    points[i].y,
                    points[i + 1].x,
                    points[i + 1].y
                );
            }

            Vector2 force{ 0.0f, 0.0f };
            if (input.GetKeyDown(SDL_SCANCODE_A)) force.x = -speed;
            if (input.GetKeyDown(SDL_SCANCODE_D)) force.x = +speed;
            if (input.GetKeyDown(SDL_SCANCODE_W)) force.y = -speed;
            if (input.GetKeyDown(SDL_SCANCODE_S)) force.y = +speed;

            player.SetVelocity(player.GetVelocity() + (force * time.GetDeltaTime()));
            player.Update(time.GetDeltaTime());

            // RENDER
            renderer.SetColor(0.0f, 0.0f, 0.0f, 255.0f);
            renderer.Clear();

            for (int i = 0; i < (int)points.size() - 1; i++) {
                renderer.SetColor(255.0f, 255.0f, 255.0f, 255.0f);
                renderer.DrawLine(
                    points[i].x, points[i].y,
                    points[i + 1].x, points[i + 1].y
                );
            }

            // Character
            player.Draw(renderer);
            //renderer.SetColor(0.0f, 255.0f, 255.0f, 255.0f);
            //renderer.DrawFillRect(position.x - 20, position.y - 20, 40, 40);

            //// Draw 3000 random points
            //for (int i = 0; i < 3000; i++) {
            //    renderer.SetColor(nu::RandomInt(256), nu::RandomInt(256), nu::RandomInt(256), 255);

            //    renderer.DrawPoint(
            //        points[i].x,
            //        points[i].y
            //    );
            //}

            renderer.Present();
        }

        // SHUTDOWN
        renderer.Shutdown();
        return 0;
    };
