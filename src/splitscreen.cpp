#include "splitscreen.hpp"

#include <jngl/matrix.hpp>

SplitScreen::SplitScreen() {
}

void SplitScreen::step() {
	field1.step();
	field2.step();
}

void SplitScreen::draw() const {
	jngl::pushMatrix();
	jngl::translate(-600, -600);
	field1.draw();
	jngl::translate(1200, 0);
	field2.draw();
	jngl::popMatrix();
}