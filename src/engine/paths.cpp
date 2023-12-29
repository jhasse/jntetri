#include "paths.hpp"
#include "../constants.hpp"

#include <jngl.hpp>

Paths::Paths() {
}

std::string Paths::getGraphics() {
	return graphics;
}

void Paths::setGraphics(const std::string& g) {
	graphics = g;
}

void Paths::setOriginalGfx(const std::string& o) {
	originalGfx = o;
}

Paths& getPaths() {
	return *Paths::handle();
}
