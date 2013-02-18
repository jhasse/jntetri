#include "splitscreen.hpp"

#include <jngl/matrix.hpp>

SplitScreen::SplitScreen() {
	field2.setControl(new Control{std::make_shared<GamepadControl>(1)});
}

void SplitScreen::step() {
	field1.step();
	field2.step();	
	field2.addJunk(field1.getLinesCleared());
	field1.addJunk(field2.getLinesCleared());
}

void SplitScreen::draw() const {
	jngl::pushMatrix();
	jngl::translate(-600, -600);
	field1.draw();
	jngl::translate(1200, 0);
	field2.draw();
	jngl::popMatrix();
}