#include "pch.h"
#include "Text.h"
#include "Vector3.h"
#include "Renderer.h"

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <iostream>

namespace nu {
	Text::~Text() {
		if (a_texture != nullptr) {
			SDL_DestroyTexture(a_texture);
		}
	}

	bool Text::Create(Renderer& renderer, const std::string& text, const Color& color) {
		// create a surface using the font, text string and color
		SDL_Color c{ (uint8_t)(color.r * 255), (uint8_t)(color.g * 255), (uint8_t)(color.b * 255), 255 };
		SDL_Surface* surface = TTF_RenderText_Solid(a_font->a_ttfFont, text.c_str(), text.size(), c);
		if (surface == nullptr) {
			std::cerr << "Could not create surface.\n";
			return false;
		}

		// create a texture from the surface, only textures can render to the renderer
		a_texture = SDL_CreateTextureFromSurface(renderer.a_renderer, surface);
		if (a_texture == nullptr) {
			SDL_DestroySurface(surface);
			std::cerr << "Could not create texture" << SDL_GetError() << std::endl;
			return false;
		}

		// free the surface, no longer needed after creating the texture
		SDL_DestroySurface(surface);

		return true;
	}

	void Text::Draw(Renderer& renderer, float x, float y) {
		// get the texture width and height
		float width, height;
		bool success = SDL_GetTextureSize(a_texture, &width, &height);
		assert(success);

		// set the texture into the renderer at rect 
		SDL_FRect rect{ x, y, width, height };
		success = SDL_RenderTexture(renderer.a_renderer, a_texture, NULL, &rect);
		assert(success);
	}

}