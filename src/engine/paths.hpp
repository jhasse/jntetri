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
private:
	std::string prefix_;
	std::string configPath_; // %AppData% or $HOME
};

Paths& GetPaths();
