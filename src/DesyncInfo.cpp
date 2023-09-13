#include "DesyncInfo.hpp"

#include "fonts.hpp"

#include <jngl/matrix.hpp>

DesyncInfo::DesyncInfo(std::function<void()> userQuitCallback)
: text(fonts::label(), "no connection"), cancel("Disconnect", std::move(userQuitCallback)) {
	text.setCenter(0, -300);
	cancel.setCenter(-660, -100);
}

void DesyncInfo::step() {
	cancel.step();
	++stepsAlive;
}

void DesyncInfo::draw() const {
	text.draw();
	jngl::pushMatrix();
	jngl::translate(660, 0); // FIXME: This undos the translation in SplitScreen.cpp
	cancel.draw();
	jngl::popMatrix();
}

uint32_t DesyncInfo::getStepsAlive() const {
	return stepsAlive;
}
