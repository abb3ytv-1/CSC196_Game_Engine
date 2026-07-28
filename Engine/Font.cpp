#include "pch.h"
#include "Font.h"

#include <SDL3_ttf/SDL_ttf.h>
#include <iostream>

namespace nu {
	Font::~Font() {
		if (a_ttfFont != nullptr) {
			TTF_CloseFont(a_ttfFont);
		}
	}

	bool Font::Load(const std::string& name, float fontSize) {
		a_ttfFont = TTF_OpenFont(name.c_str(), fontSize);
		if (a_ttfFont == nullptr) {
			std::cerr << "Could not load font: " << name << std::endl;
			return false;
		}

		return true;
	}

}