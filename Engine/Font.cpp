#include "pch.h"
#include "Font.h"

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <iostream>

using namespace nu;

Font::~Font() {
	if (a_ttfFont != nullptr) {
		TTF_CloseFont(a_ttfFont);
		a_ttfFont = nullptr;
	}
}

bool Font::Load( const std::string& filename, float fontSize ) {
	TTF_Font* newFont = TTF_OpenFont( filename.c_str(), fontSize );

	if (newFont == nullptr) {
		std::cerr
			<< "Could not load font: "
			<< filename
			<< ": "
			<< SDL_GetError()
			<< '\n';

		return false;
	}

	// Close the previous font if Load is called again.
	if (a_ttfFont != nullptr) {
		TTF_CloseFont(a_ttfFont);
	}

	a_ttfFont = newFont;

	return true;
}