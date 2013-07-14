#include "screen.hpp"
#include "paths.hpp"
#include "options.hpp"

#include <jngl/all.hpp>

Screen::Screen()
{
	xCenter_ = double(GetOptions().Get<int>("windowWidth")) / 2;
	yCenter_ = double(GetOptions().Get<int>("windowHeight")) / 2;
}

void Screen::DrawCentered(const std::string& filename, const double x, const double y)
{
	const std::string filepath = GetPaths().Graphics() + filename;
	jngl::draw(filepath,
	           x - jngl::getWidth(filepath) / 2.0,
	           y - jngl::getHeight(filepath) / 2.0);
}

void Screen::DrawCentered(const std::string& filename, const Vector2d& position)
{
	GetScreen().DrawCentered(filename, position.X(), position.Y());
}

void Screen::DrawCenteredScaled(const std::string& filename, const double x, const double y, const float factor)
{
	const std::string filepath = GetPaths().Graphics() + filename;
	jngl::drawScaled(filepath,
	                 x - jngl::getWidth(filepath) / 2.0 * factor,
					 y - jngl::getHeight(filepath) / 2.0 * factor,
					 factor);
}

void Screen::DrawCenteredScaled(const std::string& filename, const double x, const double y, const float xfactor, const float yfactor)
{
	const std::string filepath = GetPaths().Graphics() + filename;
	jngl::drawScaled(filepath,
	                 x - jngl::getWidth(filepath) / 2.0 * xfactor,
					 y - jngl::getHeight(filepath) / 2.0 * yfactor,
					 xfactor,
					 yfactor);
}

Screen& GetScreen()
{
	return Screen::Handle();
}

void Screen::PrintCentered(const std::string& text, double x, double y)
{
	jngl::print(text,
	            static_cast<int>((x - jngl::getTextWidth(text) / 2)),
	            static_cast<int>(y) - jngl::getFontSize() / 2);
}

void Screen::DrawScaled(const std::string& filename, const double x, const double y, const float factor)
{
	jngl::drawScaled(GetPaths().Graphics() + filename, x, y, factor);
}

int Screen::GetWidth() const
{
	return static_cast<int>(double(GetOptions().Get<int>("windowWidth")) / jngl::getScaleFactor());
}

int Screen::GetHeight() const
{
	return static_cast<int>(double(GetOptions().Get<int>("windowHeight")) / jngl::getScaleFactor());
}

int Screen::GetMouseX() const
{
	return static_cast<int>((jngl::getMouseX() - xCenter_) / jngl::getScaleFactor());
}

int Screen::GetMouseY() const
{
	return static_cast<int>((jngl::getMouseY() - yCenter_) / jngl::getScaleFactor());
}

Vector2d Screen::GetMousePos() const
{
	return Vector2d(GetScreen().GetMouseX(), GetScreen().GetMouseY());
}

inline Vector2d GetWindowVector() {
	return Vector2d(GetOptions().Get<int>("windowWidth"), GetOptions().Get<int>("windowHeight"));
}
