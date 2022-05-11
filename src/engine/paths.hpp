#pragma once

#include "singleton.hpp"

#include <string>

class Paths : public Singleton<Paths> {
public:
	Paths();

	std::string getGraphics();
	void setGraphics(const std::string&);

	std::string getConfig();

	void setOriginalGfx(const std::string&);

private:
	std::string configPath;
	std::string graphics;
	std::string originalGfx;
};

Paths& getPaths();
