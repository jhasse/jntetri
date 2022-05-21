#include "paths.hpp"
#include "options.hpp"
#include "../constants.hpp"

#ifdef _WIN32
	#include <windows.h>
	#include <shlobj.h>
#endif
#include <iostream>
#include <sstream>
#include <filesystem>
namespace fs = std::filesystem;
#include <jngl.hpp>

#ifndef __IPHONE_OS_VERSION_MIN_REQUIRED
Paths::Paths() {
#if defined(__linux__)
	std::stringstream path;
	path << getenv("HOME") << "/.config/" << programDisplayName << "/";
	configPath = path.str();
#else
	configPath = jngl::getConfigPath();
#endif
#ifndef EMSCRIPTEN
	fs::create_directory(configPath);
#endif
#else
Paths::Paths() : configPath(jngl::getConfigPath()) {
#endif
}

std::string Paths::getGraphics() {
	return graphics;
}

void Paths::setGraphics(const std::string& g) {
	graphics = g;
}

std::string Paths::getConfig() {
	return configPath;
}

void Paths::setOriginalGfx(const std::string& o) {
	originalGfx = o;
}

Paths& getPaths() {
	return *Paths::handle();
}
