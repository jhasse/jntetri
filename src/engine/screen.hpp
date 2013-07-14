#pragma once

#include "singleton.hpp"
#include "vector2d.hpp"

#include <string>
#include <boost/noncopyable.hpp>

class Screen : public Singleton<Screen>
{
public:
	Screen();
	void DrawCentered(const std::string& filename, double x, double y);
	void DrawCentered(const std::string& filename, const Vector2d& position);
	void DrawCenteredScaled(const std::string& filename, double x, double y, float factor);
	void DrawCenteredScaled(const std::string& filename, double x, double y, float xfactor, float yfactor);
	void DrawScaled(const std::string& filename, double x, double y, float factor);
	int GetMouseX() const;
	int GetMouseY() const;
	void PrintCentered(const std::string& text, double x, double y);
	int GetWidth() const;
	int GetHeight() const;
	Vector2d GetMousePos() const;
	Vector2d GetAbsoluteMousePos() const;
private:
	double xCenter_;
	double yCenter_;
};

Screen& GetScreen();
