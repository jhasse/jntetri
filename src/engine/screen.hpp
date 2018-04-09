#pragma once

#include "singleton.hpp"
#include "vector2d.hpp"

#include <string>

class Screen : public Singleton<Screen> {
public:
	void DrawCentered(const std::string& filename, double x, double y);
	void DrawCentered(const std::string& filename, const Vector2d& position);
	void DrawCenteredScaled(const std::string& filename, double x, double y, float factor);
	void DrawCenteredScaled(const std::string& filename, double x, double y, float xfactor,
	                        float yfactor);
	void DrawScaled(const std::string& filename, double x, double y, float factor);
	void PrintCentered(const std::string& text, double x, double y);
};

Screen& GetScreen();
