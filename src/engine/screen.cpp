#include "screen.hpp"
#include "paths.hpp"

#include <jngl.hpp>

void Screen::DrawCentered(const std::string& filename, const double x, const double y)
{
	const std::string filepath = "gfx/" + filename;
	jngl::draw(filepath,
	           x - jngl::getWidth(filepath) / 2.0,
	           y - jngl::getHeight(filepath) / 2.0);
}

Screen& GetScreen() {
	return *Screen::handle();
}

void Screen::printCentered(const std::string& text, const jngl::Vec2 center) {
	jngl::print(text,
	            static_cast<int>((center.x - jngl::getTextWidth(text) / 2)),
	            static_cast<int>(center.y) - jngl::getFontSize() / 2);
}
