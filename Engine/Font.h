#pragma once

#include <string>

struct TTF_Font;

namespace nu {
	class Font {
	public:
		Font() = default;
		~Font();

		Font(const Font&) = delete;
		Font& operator=(const Font&) = delete;

		bool Load( const std::string& filename, float fontSize );

	private:
		friend class Text;

		TTF_Font* a_ttfFont{ nullptr };
	};
}