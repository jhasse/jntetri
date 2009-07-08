#pragma once

#include "singleton.hpp"

#include <string>

class Paths : public Singleton<Paths>
{
public:
	Paths();
	std::string Data();
	std::string Graphics();
	std::string Prefix();
	std::string Config();
	void SetGraphics(const std::string&);
private:
	std::string prefix_;
	std::string configPath_; // %AppData% or $HOME
	std::string graphics_;
};

Paths& GetPaths();
