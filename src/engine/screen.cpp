#include "screen.hpp"
#include "paths.hpp"
#include "options.hpp"

#include <jngl.hpp>

Screen::Screen()
{
	xCenter_ = double(GetOptions().Get<int>("windowWidth")) / 2;
	yCenter_ = double(GetOptions().Get<int>("windowHeight")) / 2;
}

void Screen::StepCamera()
{
	cameraPosition_ += (targetCameraPosition_ - cameraPosition_) / 30.0;
}

void Screen::Draw(const std::string& filename, const double x, const double y)
{
	jngl::draw(GetPaths().Graphics() + filename, x * factor_, y * factor_);
}

void Screen::DrawScaled(const std::string& filename, const double x, const double y, const float xfactor, const float yfactor)
{
	jngl::drawScaled(GetPaths().Graphics() + filename, x * factor_, y * factor_, xfactor, yfactor);
}

void Screen::DrawCentered(const std::string& filename, const double x, const double y)
{
	const std::string filepath = GetPaths().Graphics() + filename;
	jngl::draw(filepath,
	           x*factor_ - jngl::getWidth(filepath) / 2.0,
	           y*factor_ - jngl::getHeight(filepath) / 2.0);
}

void Screen::DrawCentered(const std::string& filename, const Vector2d& position)
{
	GetScreen().DrawCentered(filename, position.X(), position.Y());
}

void Screen::DrawCenteredScaled(const std::string& filename, const double x, const double y, const float factor)
{
	const std::string filepath = GetPaths().Graphics() + filename;
	jngl::drawScaled(filepath,
	                 x*factor_ - jngl::getWidth(filepath) / 2.0 * factor,
					 y*factor_ - jngl::getHeight(filepath) / 2.0 * factor,
					 factor);
}

void Screen::DrawCenteredScaled(const std::string& filename, const double x, const double y, const float xfactor, const float yfactor)
{
	const std::string filepath = GetPaths().Graphics() + filename;
	jngl::drawScaled(filepath,
	                 x*factor_ - jngl::getWidth(filepath) / 2.0 * xfactor,
					 y*factor_ - jngl::getHeight(filepath) / 2.0 * yfactor,
					 xfactor,
					 yfactor);
}

void Screen::DrawLine(const double xstart, const double ystart, const double xend, const double yend)
{
	jngl::drawLine(xstart * factor_, ystart * factor_, xend * factor_, yend * factor_);
}

void Screen::DrawLine(const Vector2d& start, const Vector2d& end) {
	return DrawLine(start.X(), start.Y(), end.X(), end.Y());
}

Screen& GetScreen()
{
	return Screen::Handle();
}

void Screen::SetFontSize(const int size)
{
	jngl::setFontSize(static_cast<int>(size * factor_ + 0.5));
}

void Screen::DrawRect(const int x, const int y, const int width, const int height)
{
	jngl::drawRect(x * factor_, y * factor_, width * factor_, height * factor_);
}

void Screen::DrawCircle(const double x, const double y, const double radius)
{
	jngl::drawEllipse(x * factor_, y * factor_, radius * factor_, radius * factor_);
}

void Screen::DrawCircle(const Vector2d& position, const double radius) {
	return DrawCircle(position.X(), position.Y(), radius);
}

void Screen::Print(const std::string& text, double x, double y)
{
	jngl::print(text, static_cast<int>(x * factor_), static_cast<int>(y * factor_));
}

void Screen::PrintCentered(const std::string& text, double x, double y)
{
	jngl::print(text,
	            static_cast<int>((x - GetTextWidth(text) / 2) * factor_),
	            static_cast<int>(y * factor_) - jngl::getFontSize() / 2);
}

void Screen::DrawScaled(const std::string& filename, const double x, const double y, const float factor)
{
	jngl::drawScaled(GetPaths().Graphics() + filename, x * factor_, y * factor_, factor);
}

int Screen::GetWidth(const std::string& filename) const
{
	return jngl::getWidth(GetPaths().OriginalGfx() + filename);
}

int Screen::GetHeight(const std::string& filename) const
{
	return jngl::getHeight(GetPaths().OriginalGfx() + filename);
}

int Screen::GetWidth() const
{
	return static_cast<int>(double(GetOptions().Get<int>("windowWidth")) / factor_);
}

int Screen::GetHeight() const
{
	return static_cast<int>(double(GetOptions().Get<int>("windowHeight")) / factor_);
}

void Screen::Translate(double x, double y)
{
	jngl::translate(x * factor_, y * factor_);
}

int Screen::GetMouseX() const
{
	return static_cast<int>((jngl::getMouseX() - xCenter_) / factor_);
}

int Screen::GetMouseY() const
{
	return static_cast<int>((jngl::getMouseY() - yCenter_) / factor_);
}

double Screen::GetTextWidth(const std::string& text) const
{
	return jngl::getTextWidth(text) / factor_;
}

Vector2d Screen::GetMousePos() const
{
	return Vector2d(GetScreen().GetMouseX(), GetScreen().GetMouseY());
}

Vector2d Screen::GetAbsoluteMousePos() const
{
	return GetMousePos() + cameraPosition_;
}

inline Vector2d GetWindowVector() {
	return Vector2d(GetOptions().Get<int>("windowWidth"), GetOptions().Get<int>("windowHeight"));
}

void Screen::MoveCamera(const Vector2d& position)
{
	targetCameraPosition_ = position + (Vector2d(jngl::getMouseX(), jngl::getMouseY()) - GetWindowVector() / 2);
}

void Screen::CenterCameraPosition(const Vector2d& position)
{
	cameraPosition_ = position;
}

Vector2d Screen::GetCameraPosition() const
{
	return cameraPosition_;
}

Vector2d Screen::GetCameraSpeed() const
{
	return targetCameraPosition_ - cameraPosition_;
}

void Screen::TranslateCamera() {
	Translate(-cameraPosition_.X(), -cameraPosition_.Y());
}

double Screen::GetFactor() const
{
	return factor_;
}

void Screen::SetFactor(double factor) {
	factor_ = factor;
}
