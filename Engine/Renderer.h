#pragma once

#include <SDL3/SDL.h>

namespace nu
{
    class Renderer
    {
    public:
        bool Initialize(const char* name, int width, int height);
        void Shutdown();

        void Clear() const;
        void Present() const;

        void SetColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255) const;

        void DrawPoint(float x, float y) const;
        void DrawFillRect(float x, float y, float w, float h) const;
        void DrawRect(float x, float y, float w, float h) const;
        void DrawLine(float x1, float y1, float x2, float y2) const;

    private:
        SDL_Window* m_window = nullptr;
        SDL_Renderer* m_renderer = nullptr;
    };
} // namespace nu