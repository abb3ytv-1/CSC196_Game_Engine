#include "pch.h"
#include "Text.h"

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <cassert>
#include <cstdint>
#include <iostream>

namespace nu {
	Text::~Text() {
		if (a_texture != nullptr) {
			SDL_DestroyTexture(a_texture);
			a_texture = nullptr;
		}
	}

	bool Text::Create(
		Renderer& renderer,
		const std::string& text,
		const Color& color
	) {
		if (
			a_font == nullptr ||
			a_font->a_ttfFont == nullptr
			) {
			std::cerr << "Text does not have a valid font.\n";
			return false;
		}

		// Destroy the previous texture before creating another.
		if (a_texture != nullptr) {
			SDL_DestroyTexture(a_texture);
			a_texture = nullptr;
		}

		SDL_Color textColor{
			static_cast<std::uint8_t>(color.r * 255.0f),
			static_cast<std::uint8_t>(color.g * 255.0f),
			static_cast<std::uint8_t>(color.b * 255.0f),
			255
		};

		SDL_Surface* surface = TTF_RenderText_Solid(
			a_font->a_ttfFont,
			text.c_str(),
			text.size(),
			textColor
		);

		if (surface == nullptr) {
			std::cerr
				<< "Could not create text surface: "
				<< SDL_GetError()
				<< '\n';

			return false;
		}

		a_texture = SDL_CreateTextureFromSurface(
			renderer.a_renderer,
			surface
		);

		SDL_DestroySurface(surface);

		if (a_texture == nullptr) {
			std::cerr
				<< "Could not create text texture: "
				<< SDL_GetError()
				<< '\n';

			return false;
		}

		return true;
	}

	void Text::Draw(
		Renderer& renderer,
		float x,
		float y
	) {
		if (a_texture == nullptr) {
			return;
		}

		float width = 0.0f;
		float height = 0.0f;

		bool success = SDL_GetTextureSize(
			a_texture,
			&width,
			&height
		);

		assert(success);

		SDL_FRect destination{
			x,
			y,
			width,
			height
		};

		success = SDL_RenderTexture(
			renderer.a_renderer,
			a_texture,
			nullptr,
			&destination
		);

		assert(success);
	}
}