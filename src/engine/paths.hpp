#pragma once

#include "singleton.hpp"

#include <string>

class Paths : public Singleton<Paths> {
public:
	Paths();
	std::string getResolutionGraphics() const;
	std::string getData() const;

	std::string getGraphics();
	void setGraphics(const std::string&);

	std::string getPrefix();
	void setPrefix(const std::string&);

	std::string getConfig();

	std::string getOriginalGfx() const;
	void setOriginalGfx(const std::string&);

	std::string getFonts() const;
private:
	std::string prefix;
	std::string configPath;
	std::string graphics;
	std::string originalGfx;
};

Paths& getPaths();
