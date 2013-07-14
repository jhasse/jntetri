#include "splitscreen.hpp"

#include "engine/screen.hpp"

#include <jngl/matrix.hpp>
#include <jngl/font.hpp>
#include <memory>
#include <sstream>

SplitScreen::SplitScreen() {
	reset();
}

void SplitScreen::reset() {
	field1.reset(new Field);
	field2.reset(new Field);
	field2->setControl(new Control{std::make_shared<GamepadControl>(1)});
}

void SplitScreen::step() {
	field1->step();
	field2->step();
	field2->addJunk(field1->getLinesCleared());
	field1->addJunk(field2->getLinesCleared());
	if (field1->GameOver()) {
		++wins2;
		reset();
	}
	if (field2->GameOver()) {
		++wins1;
		reset();
	}
}

void SplitScreen::draw() const {
	jngl::pushMatrix();
	jngl::translate(-660, -600);
	field1->draw();
	jngl::translate(1320, 0);
	field2->draw();
	jngl::popMatrix();
	jngl::setFontSize(60);
	jngl::pushMatrix();
	jngl::translate(-180, -500);
	field1->drawNextTetromino();
	jngl::translate(330, 0);
	field2->drawNextTetromino();
	jngl::popMatrix();
	std::stringstream sstream;
	sstream << wins1 << " : " << wins2;
	jngl::setFontSize(140);
	GetScreen().PrintCentered(sstream.str(), 0, 0);
}
