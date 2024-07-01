#pragma once

#include "singleton.hpp"

#include <jngl/Vec2.hpp>
#include <string>

class Screen : public Singleton<Screen> {
public:
	void DrawCentered(const std::string& filename, double x, double y);
	void printCentered(const std::string& text, jngl::Vec2);
};

Screen& GetScreen();
