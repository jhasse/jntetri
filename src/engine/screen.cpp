#include "screen.hpp"
#include "paths.hpp"
#include "options.hpp"

#include <jngl.hpp>

Screen::Screen()
{
	const int maxScreenHeight = 1200;
	factor_ = double(GetOptions().GetWindowHeight()) / maxScreenHeight;
	xCenter_ = double(GetOptions().GetWindowWidth()) / 2;
}

void Screen::BeginDraw()
{
	jngl::Translate(xCenter_, 0);
}

void Screen::DrawCentered(const std::string& filename, const double x, const double y)
{
	const std::string filepath = GetPaths().Graphics() + filename;
	jngl::Draw(filepath,
	           x*factor_ - jngl::GetWidth(filepath) / 2.0,
	           y*factor_ - jngl::GetHeight(filepath) / 2.0);
}

void Screen::DrawCenteredScaled(const std::string& filename, const double x, const double y, const double factor)
{
	const std::string filepath = GetPaths().Graphics() + filename;
	jngl::DrawScaled(filepath,
	                 x*factor_ - jngl::GetWidth(filepath) / 2.0 * factor,
					 y*factor_ - jngl::GetHeight(filepath) / 2.0 * factor,
					 factor);
}

void Screen::DrawLine(const double xstart, const double ystart, const double xend, const double yend)
{
	jngl::DrawLine(xstart * factor_, ystart * factor_, xend * factor_, yend * factor_);
}

Screen& GetScreen()
{
	return Screen::Handle();
}

void Screen::SetFontSize(const int size)
{
	jngl::SetFontSize(static_cast<int>(size * factor_ + 0.5));
}

void Screen::DrawRect(const int x, const int y, const int width, const int height)
{
	jngl::DrawRect(x * factor_, y * factor_, width * factor_, height * factor_);
}

void Screen::Print(const std::string& text, double x, double y)
{
	jngl::Print(text, static_cast<int>(x * factor_), static_cast<int>(y * factor_));
}

void Screen::PrintCentered(const std::string& text, double x, double y)
{
	jngl::Print(text,
	            static_cast<int>((x - GetTextWidth(text) / 2) * factor_),
	            static_cast<int>(y * factor_) - jngl::GetFontSize() / 2);
}

void Screen::DrawScaled(const std::string& filename, const double x, const double y, const double factor)
{
	jngl::DrawScaled(GetPaths().Graphics() + filename, x, y, factor);
}

int Screen::GetWidth(const std::string& filename) const
{
	return jngl::GetWidth(GetPaths().Data() + "gfx/x1200/" + filename);
}

int Screen::GetHeight(const std::string& filename) const
{
	return jngl::GetHeight(GetPaths().Data() + "gfx/x1200/" + filename);
}

int Screen::GetWidth() const
{
	return static_cast<int>(double(GetOptions().GetWindowWidth()) / factor_);
}

int Screen::GetHeight() const
{
	return static_cast<int>(double(GetOptions().GetWindowHeight()) / factor_);
}

void Screen::Translate(double x, double y)
{
	jngl::Translate(x * factor_, y * factor_);
}

int Screen::GetMouseX() const
{
	return static_cast<int>((jngl::GetMouseX() - xCenter_) / factor_);
}

int Screen::GetMouseY() const
{
	return static_cast<int>(jngl::GetMouseY() / factor_);
}

double Screen::GetTextWidth(const std::string& text) const
{
	return jngl::GetTextWidth(text) / factor_;
}
