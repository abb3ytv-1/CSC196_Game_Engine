#include "pch.h"
#include "Renderer.h"
#include <SDL3/SDL.h>

#include <iostream>

using namespace nu;

// Initialize
bool Renderer::Initialize(const char* name, int width, int height)
{
	SDL_Init(SDL_INIT_VIDEO);

	m_window = SDL_CreateWindow(name, width, height, 0);
	if (m_window == nullptr) {
		std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return false;
	}

	m_renderer = SDL_CreateRenderer(m_window, NULL);
	if (m_renderer == nullptr) {
		std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
		SDL_DestroyWindow(m_window);
		SDL_Quit();
		return false;
	}

	SDL_SetRenderVSync(m_renderer, 1);

	return true;
}

// Shutdown
void Renderer::Shutdown()
{
	SDL_DestroyRenderer(m_renderer);
	SDL_DestroyWindow(m_window);
	SDL_Quit();
}

// Clear
void Renderer::Clear() const
{
	SDL_RenderClear(m_renderer);
}

// Present
void Renderer::Present() const
{
	SDL_RenderPresent(m_renderer);
}

//Setcolor
void Renderer::SetColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a) const
{
	SDL_SetRenderDrawColor(m_renderer, r, g, b, a);
}

// Draw point
void Renderer::DrawPoint(float x, float y) const
{
	SDL_RenderPoint(m_renderer, x, y);
}

// Draw Filled Rectangle
void Renderer::DrawFillRect(float x, float y, float w, float h) const 
{
	SDL_FRect rect = { x, y, w, h };
	SDL_RenderFillRect(m_renderer, &rect);
}

// Draw Rectangle Outline
void Renderer::DrawRect(float x, float y, float w, float h) const
{
	SDL_FRect rect = { x, y, w, h };
	SDL_RenderRect(m_renderer, &rect);
}

// Draw Line
void Renderer::DrawLine(float x1, float y1, float x2, float y2) const
{
	SDL_RenderLine(m_renderer, x1, y1, x2, y2);
}