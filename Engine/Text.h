#pragma once
#include "Font.h"

class Renderer;
struct Color;
struct SDL_Texture;


namespace nu {
	
	class Text {
	public:
		Text() = default;
		Text(Font* font) : a_font{ font } {}
		~Text();

		bool Create(Renderer& renderer, const std::string& text, const Color& color);
		void Draw(Renderer& renderer, float x, float y);

	private:
		Font* a_font{ nullptr };
		SDL_Texture* a_texture{ nullptr };
	};

}